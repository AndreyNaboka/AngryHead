//
//  Gun.cpp
//  AngryHead
//
//  Created by Andrey Naboka on 12/04/14.
//
//

#include "gun.h"
#include <iostream>

const float gun::BASE_SPEED     = 1250.0f;
const float gun::DAMAGE_BASE    = 35.0f;
const float gun::SHOOT_INTERVAL_STEP = 0.05f;

gun::gun(cocos2d::Layer* const parent,
         const float aim_x,
         const float aim_y,
         const float start_x,
         const float start_y)
    :m_aim_x(aim_x)
    ,m_aim_y(aim_y)
    ,m_start_x(start_x)
    ,m_start_y(start_y)
    ,m_parent(parent)
    ,m_fire_rate_level(1)
    ,m_shoot_interval(0.5)
{
    m_time_to_next_shoot = m_shoot_interval;
    add_new_bullets(1);
}

void gun::update(const float delta) {
    m_time_to_next_shoot -= delta;
    if (m_time_to_next_shoot <= 0.0f) {
        add_new_bullets(1);
        m_time_to_next_shoot = m_shoot_interval - (get_fire_rate_level()*SHOOT_INTERVAL_STEP);
    }
    update_bullets(delta);
}

void gun::update_bullets(const float delta) {
    for (auto bullet = m_bullets.begin(); bullet != m_bullets.end(); ++bullet) {
        cocos2d::Point bullet_direction((*bullet)->get_move_to_x(), (*bullet)->get_move_to_y());
        cocos2d::Point diff_vector = cocos2d::Point(m_start_x,m_start_y)-bullet_direction;
        float angle = M_PI_2 - atan2f(diff_vector.x,diff_vector.y);
        cocos2d::Point direction_vector = cocos2d::Point(cosf(-1*angle), sinf(-1*angle));
        direction_vector.normalize();
        float new_x = (*bullet)->get_position_x() - ((delta*BASE_SPEED)*direction_vector.x);
        float new_y = (*bullet)->get_position_y() + ((delta*BASE_SPEED)*direction_vector.y);
        (*bullet)->set_position(new_x, new_y);
    }
}

void gun::add_new_bullets(const int count) {
    for (int i = 0; i < count; ++i) {
        auto bullet = m_bullets.insert(m_bullets.end(), entity_ptr(new entity("bullet")));
        (*bullet)->set_position(m_start_x, m_start_y);
        (*bullet)->set_move_to(m_aim_x, m_aim_y);
        m_parent->addChild((*bullet)->get_sprite());
    }
}

void gun::set_new_aim(const float x, const float y) {
    m_aim_x = x;
    m_aim_y = y;
}

void gun::remove_bullets(const std::list<entity_ptr>::iterator &it) {
    m_parent->removeChild((*it)->get_sprite());
    m_bullets.erase(it);
}

void gun::set_fire_rate_level(const int level) {
    m_fire_rate_level = level;
}