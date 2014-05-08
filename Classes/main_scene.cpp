#include "main_scene.h"
#include <iostream>

USING_NS_CC;

const std::string main_scene::UP_GUN_DAMAGE      = "GunDamage";
const std::string main_scene::UP_GUN_FIRE_RATE   = "GunFireRate";
const std::string main_scene::UP_SCORE           = "Score";

Scene* main_scene::createScene() {
    auto scene = Scene::create();
    auto layer = main_scene::create();
    scene->addChild(layer);
    return scene;
}

bool main_scene::init() {
    create_world();
    return true;
}

main_scene::~main_scene() {
    if (m_field_size) {
        delete[] m_field;
    }
}

void main_scene::update_game_objects(const float delta) {
//
//    std::cout << "Start update game objects" << std::endl;
//
    m_update_objects_now = true;
    
    for (auto enemy = m_enemies.begin(); enemy != m_enemies.end(); ++enemy) {
//
//        std::cout << "\tupdate enemy " << (*enemy)->get_id() << std::endl;
//
        (*enemy)->update(delta);
        if ((*enemy)->get_position_y() > m_origin.y + m_visible_size.height - 10.0f) {
//
//            std::cout << "\tenemy " << (*enemy)->get_id() << " kill head, gameOver()" << std::endl;
//
            (*enemy)->mark_for_remove();
            if ((*enemy)->get_id() == m_selected_enemy_aim->get_id()) {
                m_selected_enemy_aim.reset();
                m_selected_enemy_aim->set_position(-1000.0f, -1000.0f);
            }
            game_over();
            return;
        }
    }
    
    if (m_selected_enemy) {
        Point selected_enemy_pos = Point(m_selected_enemy->get_position_x(), m_selected_enemy->get_position_y());
        rotate_ray(selected_enemy_pos);
        m_gun->set_new_aim(m_selected_enemy->get_position_x(), m_selected_enemy->get_position_y());
        m_selected_enemy_aim->set_position(m_selected_enemy->get_position_x(), m_selected_enemy->get_position_y());
        m_selected_enemy_aim->set_scale(m_selected_enemy->get_scale()*1.2);
    }
    
    
    m_gun->update(delta);
    
    m_update_objects_now = false;

//
//    std::cout << "\tGun update" << std::endl;
//    std::cout << "End update game objects";
//

}

void main_scene::check_collision_enemies_with_bullets() {
//
//    std::cout << "Start check collision enemies with bullets" << std::endl;
//
    
    for (auto bullet = m_gun->first_bullet(); bullet != m_gun->end_bullet(); ++bullet) {
//
//        std::cout << "\tcheck bullet " << (*bullet)->get_id() << std::endl;
//
        for (auto enemy = m_enemies.begin(); enemy != m_enemies.end(); ++enemy) {
            
//
//            std::cout << "\tcheck enemy " << (*enemy)->get_id() << std::endl;
//
            if ((*enemy)->is_mark_for_remove() || (*enemy)->is_was_killed()) {
//
//                std::cout << "\tenemy" << (*enemy)->get_id() << " already mark for remove, continue" << std::endl;
//
                continue;
            }
            
            const bool is_bullet_intersect_enemy =  (*bullet)->get_position_x() > (*enemy)->get_position_x()-(*enemy)->get_width()/2 &&
                                                    (*bullet)->get_position_x() < (*enemy)->get_position_x()+(*enemy)->get_width()/2 &&
                                                    (*bullet)->get_position_y() > (*enemy)->get_position_y()-(*enemy)->get_height()/2 &&
                                                    (*bullet)->get_position_y() < (*enemy)->get_position_y()+(*enemy)->get_height()/2;
            if (is_bullet_intersect_enemy) {
//
//                std::cout << "\tBullet " << (*bullet)->get_id() << " intersect with enemy " << (*enemy)->get_id() << std::endl;
//
                (*bullet)->mark_for_remove();
//
//                std::cout << "\tBullet " << (*bullet)->get_id() << " mark for remove" << std::endl;
//                std::cout << "\tEnemy " << (*enemy)->get_id() << " current life: " << (*enemy)->get_life() << std::endl;
  
                (*enemy)->set_life((*enemy)->get_life() - m_gun->get_damage());
//
//                std::cout << "\tEnemy " << (*enemy)->get_id() << " new life: " << (*enemy)->get_life() << std::endl;
//
                break;
            }
        }
    }
    
//
//    std::cout << "End check collision enemies with bullets" << std::endl;
//
}

void main_scene::remove_objects_from_scene() {
//    
//    std::cout << "Start remove objects from scene" << std::endl;
//    
    unsigned int add_enemies_count = 0;
    for (auto enemy = m_enemies.begin(); enemy != m_enemies.end(); ++enemy) {
        if ((*enemy)->is_mark_for_remove()) {
//            
//            std::cout << "\tenemy " << (*enemy)->get_id() << " is mark for remove" << std::endl;
//
            
            if (m_selected_enemy && (*enemy)->get_id() == m_selected_enemy->get_id()) {
                m_selected_enemy.reset();
                m_selected_enemy_aim->set_position(-1000.0f, -1000.0f);
            }
            
            m_field[(*enemy)->get_field_index()] = false;
            
            removeChild((*enemy)->get_sprite());
            m_enemies.erase(enemy);
            add_enemies_count++;
            m_score++;
        }
    }
    
//    
//    std::cout << "End remove objects from scene" << std::endl;
//
    
    update_score();
    add_enemy(add_enemies_count);
    
    for (auto bullet = m_gun->first_bullet(); bullet != m_gun->end_bullet(); ++bullet) {
        if (((*bullet)->is_mark_for_remove()) ||
            (*bullet)->get_position_x() > m_visible_size.width ||
            (*bullet)->get_position_x() <= 10.0f ||
            (*bullet)->get_position_y() > m_visible_size.height ||
            (*bullet)->get_position_y() <= 10.0f)
            m_gun->remove_bullets(bullet);
    }
}

void main_scene::update(const float delta) {
    
    show_debug_info();
    
    switch (m_game_state) {
        case MAIN_GAME_STATE:
            update_game_objects(delta);
            check_collision_enemies_with_bullets();
            remove_objects_from_scene();
            break;
        case LEVEL_UP:
        case GAME_OVER:
        default:
            break;
    }
    

}

void main_scene::show_level_up() {
    m_game_state = LEVEL_UP;

    update_score();
    

    m_upgrade_background.reset(new entity("upgrade_background"));
    m_upgrade_background->set_scale(m_visible_size.width/m_upgrade_background->get_width(), m_visible_size.height/m_upgrade_background->get_height());
    m_upgrade_background->set_position(m_origin.x+(m_upgrade_background->get_width()/2), m_origin.y+(m_upgrade_background->get_height()/2));
    m_upgrade_background->set_opacity(100);

    m_upgrade_window.reset(new entity("upgrade_window"));
    const float upgrade_window_width_scale_factor = (m_visible_size.width/m_upgrade_window->get_width())*0.9;
    const float upgrade_window_height_scale_factor = (m_visible_size.height/m_upgrade_window->get_height())*0.8;
    m_upgrade_window->set_scale(upgrade_window_width_scale_factor, upgrade_window_height_scale_factor);
    m_upgrade_window->set_position(m_visible_size.width/2, m_visible_size.height/2);
    
    m_start_after_upgrade_button.reset(new entity("start_after_upgrade_button"));
    m_start_after_upgrade_button->set_position(m_origin.x + (m_visible_size.width/2), m_origin.y + m_start_after_upgrade_button->get_height()/2);

    m_upgrade_button.reset(new entity("upgrade_text"));
    m_upgrade_button->set_position(m_visible_size.width*0.3, m_visible_size.height*0.38);
    m_upgrade_button->set_scale(upgrade_window_width_scale_factor, upgrade_window_height_scale_factor);
    
    m_upgrade_damage_button.reset(new entity("upgrade_damage"));
    m_upgrade_damage_button->set_position(m_visible_size.width*0.3, m_visible_size.height*0.65);
    m_upgrade_damage_button->set_scale(upgrade_window_width_scale_factor, upgrade_window_height_scale_factor);
    
    
    char fire_rate_cost[512];
    snprintf(fire_rate_cost, 512, "%i$", FIRE_RATE_COST);
    m_fire_rate_cost_label = Label::create(std::string(fire_rate_cost), "", 50);
    m_fire_rate_cost_label->setColor(cocos2d::Color3B(0.0,0.0,0.0));
    m_fire_rate_cost_label->setPosition(m_visible_size.width*0.28, m_visible_size.height*0.45);
    
    char damage_cost[512];
    snprintf(damage_cost, 512, "%i$", DAMAGE_COST);
    m_damage_cost_label = Label::create(std::string(damage_cost), "", 50);
    m_damage_cost_label->setColor(cocos2d::Color3B(0.0,0.0,0.0));
    m_damage_cost_label->setPosition(m_visible_size.width*0.28, m_visible_size.height*0.72);
    
    char fire_rate_level[64];
    snprintf(fire_rate_level, 64, "%i", m_gun->get_fire_rate_level());
    m_fire_rate_level_label = Label::create(std::string(fire_rate_level), "", 50);
    m_fire_rate_level_label->setColor(cocos2d::Color3B(0.0,0.0,0.0));
    m_fire_rate_level_label->setPosition(m_visible_size.width - (m_visible_size.width*0.28), m_visible_size.height*0.35);
    
    char damage_level[64];
    snprintf(damage_level, 64, "%i", m_gun->get_damage_level());
    m_damage_level_label = Label::create(std::string(damage_level), "", 50);
    m_damage_level_label->setColor(cocos2d::Color3B(0.0,0.0,0.0));
    m_damage_level_label->setPosition(m_visible_size.width - (m_visible_size.width*0.28), m_visible_size.height*0.62);
    
    addChild(m_upgrade_background->get_sprite());
    addChild(m_upgrade_window->get_sprite());
    addChild(m_upgrade_button->get_sprite());
    addChild(m_start_after_upgrade_button->get_sprite());
    addChild(m_upgrade_damage_button->get_sprite());
    addChild(m_fire_rate_cost_label);
    addChild(m_damage_cost_label);
    addChild(m_fire_rate_level_label);
    addChild(m_damage_level_label);
}

void main_scene::get_enemy_position(float &x, float &y, int& field_index, const float enemy_width) {
    y = 10.0f;
    
    if (m_field_size == 0) {
        m_field_size = m_visible_size.width / enemy_width;
        m_field = new bool[m_field_size];
    }
    
    for (int i = 0; i < m_field_size; ++i) {
        if (m_field[i] == false) {
            m_field[i] = true;
            x = (i * enemy_width) + (enemy_width/2.0f);
            field_index = i;
            return;
        }
    }
    
    const int random_index = (rand() % m_field_size + 1);
    x = (random_index * enemy_width) + (enemy_width/2.0f);
    field_index = random_index;
    
//    bool position_found = true;
//    int tryCounter = 0;
//    float farX = 0.0f;
//    do {
//        x = (float)rand()/((float)RAND_MAX/(m_visible_size.width));
//        for (auto enemy = m_enemies.begin(); enemy != m_enemies.end(); ++enemy) {
//            const float distance = (*enemy)->get_sprite()->getPosition().getDistance(cocos2d::Point(x,y));
//            if (distance < enemy_width) {
//                if (farX < x)
//                    farX = x;
//                position_found = false;
//                tryCounter++;
//                break;
//            }
//            
//        }
//        if (tryCounter >= 50) break;
//        
//    } while (!position_found);
//    if (tryCounter >= 50)
//        x = farX;
}

void main_scene::add_enemy(const int count) {
    const float min_distance_to_head = 10.0f;
    const float move_to_y = m_origin.x + m_visible_size.height;
    for (int i = 0; i < count; ++i) {
        const float enemy_life = (rand() % 3 + 1) * ENEMY_BASE_LIFE;
        std::string enemy_type;
        float enemy_speed = 0.0f;
        float enemy_scale_factor = 1.0f;
        if (enemy_life <= ENEMY_BASE_LIFE) {
            enemy_type = "enemy";
            enemy_speed = 60.0f;
            enemy_scale_factor = 0.65f;
        } else if (enemy_life > ENEMY_BASE_LIFE && enemy_life <= ENEMY_BASE_LIFE*2) {
            enemy_type = "enemy_mid";
            enemy_speed = 50.0f;
            enemy_scale_factor = 1.0f;
        } else if (enemy_life > ENEMY_BASE_LIFE*2) {
            enemy_type = "enemy_hard";
            enemy_speed = 35.0f;
            enemy_scale_factor = 1.5f;
        }
        
        auto new_enemy = enemy_ptr(new enemy(enemy_type, min_distance_to_head));
        float x_position, y_position;
        int field_index;
        get_enemy_position(x_position, y_position, field_index, std::max(new_enemy->get_width(), new_enemy->get_height()));
        new_enemy->set_position(x_position, y_position);
        new_enemy->set_move_to(x_position, move_to_y);
        new_enemy->set_life(enemy_life);
        new_enemy->set_speed(enemy_speed);
        new_enemy->set_scale(enemy_scale_factor);
        new_enemy->set_field_index(field_index);
        addChild(new_enemy->get_sprite());
        m_enemies.insert(m_enemies.end(), new_enemy);
//        
//        std::cout << "Add new enemy " << new_enemy->get_id() << ", type: " << enemy_type << std::endl;
//        
    }
}

void main_scene::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) {
    proceed_touches(touches, event);
}

void main_scene::onTouchesMoved(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event) {
    proceed_touches(touches, event);
}

void main_scene::onTouchesEnded(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event) {
    proceed_touches(touches, event);
    m_button_pressed = false;
}

void main_scene::hide_game_over() {
    removeChild(m_last_label);
}

void main_scene::proceed_touches(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event) {
    switch (m_game_state) {
        case GAME_OVER:
            hide_game_over();
            show_level_up();
            break;
        case MAIN_GAME_STATE:
            m_selected_enemy.reset();
            for (auto touch = touches.begin(); touch != touches.end(); ++touch) {

                for (auto enemy = m_enemies.begin(); enemy != m_enemies.end(); ++enemy) {
                    if ((*enemy)->is_mark_for_remove() || (*enemy)->is_was_killed()) continue;
                    
                    if ((*enemy)->get_sprite()->getBoundingBox().containsPoint((*touch)->getLocation())) {
                        m_selected_enemy = (*enemy);
                        break;
                    }
                }
                
                m_gun->set_new_aim((*touch)->getLocation().x, (*touch)->getLocation().y);
                rotate_ray((*touch)->getLocation());
            }
            
            if (!m_selected_enemy)
                m_selected_enemy_aim->set_position(-1000.0f, -1000.0f);
            
            break;
        case LEVEL_UP:
            if (m_button_pressed) return;
            m_button_pressed = true;
            for (auto touch = touches.begin(); touch != touches.end(); ++touch) {
                if (!(*touch) || !m_upgrade_button->get_sprite() || !m_start_after_upgrade_button->get_sprite()) continue;
                if (m_start_after_upgrade_button->get_sprite()->getBoundingBox().containsPoint((*touch)->getLocation())) {
                    hide_level_up();
                    break;
                }
                
                if (m_upgrade_button->get_sprite()->getBoundingBox().containsPoint((*touch)->getLocation())) {
                    if (m_score < FIRE_RATE_COST) return;
                    m_gun->set_fire_rate_level(m_gun->get_fire_rate_level()+1);
                    m_score -= FIRE_RATE_COST;
                    
                    UserDefault::getInstance()->setIntegerForKey(UP_GUN_FIRE_RATE.c_str(), m_gun->get_fire_rate_level());
                    char fireRateLevel[64];
                    snprintf(fireRateLevel, 64, "%i", m_gun->get_fire_rate_level());
                    m_fire_rate_level_label->setString(fireRateLevel);
                    update_score();
                    break;
                }
                
                if (m_upgrade_damage_button->get_sprite()->getBoundingBox().containsPoint((*touch)->getLocation())) {
                    if (m_score < DAMAGE_COST) return;
                    m_gun->set_damage_level(m_gun->get_damage_level()+1);
                    UserDefault::getInstance()->setIntegerForKey(UP_GUN_DAMAGE.c_str(), m_gun->get_damage());
                    m_score -= DAMAGE_COST;
                    update_score();
                    char damage_level[64];
                    snprintf(damage_level, 64, "%i", m_gun->get_damage_level());
                    m_damage_level_label->setString(damage_level);
                    break;
                }
            }
            break;
        default:
            break;
    }
}

void main_scene::hide_level_up() {
    removeChild(m_upgrade_button->get_sprite());
    removeChild(m_start_after_upgrade_button->get_sprite());
    removeChild(m_upgrade_damage_button->get_sprite());
    removeChild(m_upgrade_background->get_sprite());
    removeChild(m_upgrade_window->get_sprite());
    removeChild(m_fire_rate_cost_label);
    removeChild(m_damage_cost_label);
    removeChild(m_fire_rate_level_label);
    removeChild(m_damage_level_label);
    
    for (auto enemy = m_enemies.begin(); enemy != m_enemies.end(); ++enemy) {
        removeChild((*enemy)->get_sprite());
        m_enemies.erase(enemy);
    }
    add_enemy(ENEMIES_COUNT);
    
    m_game_state = MAIN_GAME_STATE;
}

void main_scene::start_new_game() {
    for (auto enemy = m_enemies.begin(); enemy != m_enemies.end(); ++enemy) {
        removeChild((*enemy)->get_sprite());
        m_enemies.erase(enemy);
    }
    add_enemy(ENEMIES_COUNT);

    m_game_state = MAIN_GAME_STATE;

    m_score = 0;
    m_gun->set_fire_rate_level(1);
    m_gun->set_damage_level(1);
    
    update_score();
}

void main_scene::rotate_ray(const cocos2d::Point& to_point) {
    const Point diff_vector = Point(m_ray->get_position_x(),m_ray->get_position_y()) - to_point;
    const float ray_angle   = atan2f(diff_vector.x, diff_vector.y);
    m_ray->rotate_sprite(ray_angle);
}

std::string main_scene::get_score() const {
    std::ostringstream oss;
    oss << "Score: " << m_score;
    return oss.str();
}

void main_scene::update_score() {
    UserDefault::getInstance()->setIntegerForKey(UP_SCORE.c_str(), m_score);
    m_score_label->setString(get_score());
    m_score_label->setPosition(m_origin.x+m_visible_size.width-m_score_label->getContentSize().width, m_origin.y+m_visible_size.height-m_score_label->getContentSize().height);
}

void main_scene::game_over() {
    m_game_state = GAME_OVER;
    auto fontFile = FileUtils::getInstance()->fullPathForFilename("Marker Felt");
    char byebyeText[1024];
    snprintf(byebyeText, 1024, "You are dead, you kill %i enemy. Touch screen to upgrade and restart", m_score);

    m_last_label = Label::create(std::string(byebyeText), fontFile, 40);
    addChild(m_last_label);


    m_last_label->setPosition(m_origin.x + m_visible_size.width/2 - (m_last_label->getWidth()/2),
                              m_origin.y + m_visible_size.height/2 - (m_last_label->getHeight()/2));
}

void main_scene::show_debug_info() {
    if (getChildrenCount() > m_max_objects_on_scene) {
        m_max_objects_on_scene = getChildrenCount();
        std::cout << "Max objects count: " << m_max_objects_on_scene << std::endl;
    }
}

void main_scene::create_world() {
    m_visible_size = Director::getInstance()->getVisibleSize();
    m_origin = Director::getInstance()->getVisibleOrigin();
    
    m_background.reset(new entity("background"));
    m_background->set_scale(m_visible_size.width / m_background->get_width(), m_visible_size.height / m_background->get_height());
    m_background->set_position(m_origin.x + (m_visible_size.width/2), m_origin.y + (m_visible_size.height/2));
    addChild(m_background->get_sprite());
    
    
    m_earth.reset(new entity("earth"));
    m_earth->set_scale(m_visible_size.width / m_earth->get_width(), (m_visible_size.height * 0.3f) / m_earth->get_height());
    m_earth->set_position(m_origin.x + (m_visible_size.width/2), m_origin.y + m_earth->get_height()/2);
    addChild(m_earth->get_sprite());
    
    
    m_head.reset(new entity("head"));
    m_head->set_scale( ((m_visible_size.height - (m_origin.y+m_earth->get_height()/2))*0.8) / m_head->get_height() );
    m_head->set_position(m_origin.x + (m_visible_size.width/2), m_origin.y + m_earth->get_height() + m_head->get_height()/2);
    addChild(m_head->get_sprite());
    
    
    m_eye.reset(new entity("eye"));
    m_eye->set_scale(m_head->get_height()*0.2 / m_eye->get_height());
    m_eye->set_position(m_origin.x + (m_visible_size.width/2), m_origin.y + m_head->get_position_y() + (m_head->get_height()*0.15));
    addChild(m_eye->get_sprite());
    
    
    m_pupil.reset(new entity("pupil"));
    m_pupil->set_scale(m_eye->get_height()*0.6 / m_pupil->get_height());
    m_pupil->set_position(m_origin.x + (m_visible_size.width/2), m_origin.y + m_eye->get_position_y());
    addChild(m_pupil->get_sprite());
    
    
    m_ray.reset(new entity("ray"));
    m_ray->set_scale(1.0f, m_visible_size.width/m_ray->get_height());
    m_ray->set_anchor_point(0.5f, 1.0f);
    m_ray->set_position(m_pupil->get_position_x(), m_pupil->get_position_y());
    m_ray->set_opacity(40);
    addChild(m_ray->get_sprite());
    
    
    const float gunShootToX = m_visible_size.width/2;
    const float gunShootToY = 0.0f;
    const float gunPositionX = m_eye->get_position_x();
    const float gunPositionY = m_eye->get_position_y();
    m_gun.reset(new gun(this, gunShootToX, gunShootToY, gunPositionX, gunPositionY));
    
    m_score = UserDefault::getInstance()->getIntegerForKey(UP_SCORE.c_str());
    m_gun->set_fire_rate_level(UserDefault::getInstance()->getIntegerForKey(UP_GUN_FIRE_RATE.c_str(), 1));
    const int savedGunDamage = UserDefault::getInstance()->getIntegerForKey(UP_GUN_DAMAGE.c_str());
    m_gun->set_damage_level( savedGunDamage ? savedGunDamage : 1 );
    
    auto fontFile = FileUtils::getInstance()->fullPathForFilename("fonts/Marker Felt");
    m_score_label = cocos2d::Label::create(get_score(), fontFile, 40);
    m_score_label->setAnchorPoint(Point(0.0f,0.0f));
    m_score_label->setAlignment(TextHAlignment::RIGHT);
    update_score();
    addChild(m_score_label);
 
    m_button_pressed = false;

    m_selected_enemy_aim.reset(new entity("round"));
    m_selected_enemy_aim->set_position(-1000.0f, -1000.0f);
    addChild(m_selected_enemy_aim->get_sprite());
    
    
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan=std::bind(&main_scene::onTouchesBegan, this, std::placeholders::_1, std::placeholders::_2);
    listener->onTouchesMoved=std::bind(&main_scene::onTouchesMoved, this, std::placeholders::_1, std::placeholders::_2);
    listener->onTouchesEnded=std::bind(&main_scene::onTouchesEnded, this, std::placeholders::_1, std::placeholders::_2);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 100);
    
    m_max_objects_on_scene = 0;
    m_damage_cost_label = NULL;
    m_fire_rate_cost_label = NULL;
    
    srand(static_cast<unsigned int>(time(0)));
    schedule(schedule_selector(main_scene::update));
    
    add_enemy(ENEMIES_COUNT);
    
    m_update_objects_now = false;
 
    m_field_size = 0;
    
    show_level_up();
}