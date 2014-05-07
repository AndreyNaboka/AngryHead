#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "entity.h"
#include "enemy.h"
#include "gun.h"
#include <list>


class main_scene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void update(const float delta);
    
    virtual void onTouchesEnded(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event *event);
    virtual void onTouchesMoved(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event *event);
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>&, cocos2d::Event*);
    
private:
    void create_world();
    void add_enemy(const int count);
    CREATE_FUNC(main_scene);
    void rotate_ray(const cocos2d::Point& to_point);
    std::string get_score() const;
    void update_score();
    void start_new_game();
    void game_over();
    void update_game_objects(const float delta);
    void show_debug_info();
    void check_collision_enemies_with_bullets();
    void remove_objects_from_scene();
    void show_level_up();
    void proceed_touches(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event *event);
    void hide_level_up();
    void hide_game_over();
    void get_enemy_position(float& x, float& y, const float enemy_width);
    
private:
    //
    // Level objects
    entity_ptr m_background;
    entity_ptr m_earth;
    entity_ptr m_head;
    entity_ptr m_eye;
    entity_ptr m_pupil;
    entity_ptr m_ray;
    
    
    //
    // Upgrade window objects
    entity_ptr m_upgrade_button;
    entity_ptr m_start_after_upgrade_button;
    entity_ptr m_upgrade_background;
    entity_ptr m_upgrade_window;
    entity_ptr m_upgrade_damage_button;
    cocos2d::Label* m_damage_cost_label;
    cocos2d::Label* m_fire_rate_cost_label;
    cocos2d::Label* m_fire_rate_level_label;
    cocos2d::Label* m_damage_level_label;
    
    
    gun_ptr m_gun;
    
    bool m_button_pressed;
    size_t m_max_objects_on_scene;
    cocos2d::Size m_visible_size;
    cocos2d::Point m_origin;
    cocos2d::Label* m_score_label;
    cocos2d::Label* m_last_label;
    unsigned int m_score;
    
    std::list<enemy_ptr> m_enemies;
    enemy_ptr m_selected_enemy;
    entity_ptr m_selected_enemy_aim;
    
    //
    // Main consts
    static const int ENEMIES_COUNT   = 7;
    static const int FIRE_RATE_COST  = 100;
    static const int DAMAGE_COST     = 100;
    static const int ENEMY_BASE_LIFE = 40;
    static const std::string UP_GUN_DAMAGE;
    static const std::string UP_GUN_FIRE_RATE;
    static const std::string UP_SCORE;
    
    enum GAME_STATE {
        MAIN_GAME_STATE,
        GAME_OVER,
        LEVEL_UP
    };
    
    bool m_update_objects_now;
    
    GAME_STATE m_game_state;
};

#endif // __HELLOWORLD_SCENE_H__
