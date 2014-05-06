//
//  DynamicObject.cpp
//  AngryHead
//
//  Created by Andrey Naboka on 11/04/14.
//
//

#include "enemy.h"
#include "cocos2d.h"

enemy::enemy(const std::string& name, const float min_distance)
    :entity(name)
    ,m_min_distance(min_distance)
    ,m_time_to_show_coin(TIME_TO_SHOW_COIN)
    ,m_time_to_show_explosion(TIME_TO_SHOW_EXLOSION)
    ,m_state(NORMAL)
    ,m_was_killed(false)
    ,m_speed(0.0f)
{
}

void enemy::update(const float delta) {
    if (m_was_killed) {
        if (m_time_to_show_explosion > 0.0f) {
            m_time_to_show_explosion -= delta;
            
            if (m_state == NORMAL) {
                set_new_sprite("enemy_death");
                m_state = EXPLOSION;
            }
            
            GLuint newOpacity = 255 * m_time_to_show_explosion;
            set_opacity(newOpacity);
            
        } else if (m_time_to_show_explosion <= 0.0f && m_time_to_show_coin > 0.0f) {
            m_time_to_show_coin -= delta;
            
            if (m_state == EXPLOSION) {
                m_state = COIN;
                set_new_sprite("coin");
            }
            
            GLuint newOpacity = 255 * m_time_to_show_coin;
            set_opacity(newOpacity);
            
        } else if (m_time_to_show_explosion <= 0.0f && m_time_to_show_coin <= 0.0f) {
            mark_for_remove();
        }
        return;
    }

    m_moving_vector = cocos2d::Point(m_move_to_x-m_x, m_move_to_y-m_y);
    if (m_moving_vector.getLength()==0) {
        mark_for_remove();
    } else {    
        cocos2d::Point moving_vector = m_moving_vector/m_moving_vector.getLength();
        const float new_x = m_x + (delta*m_speed*moving_vector.x);
        const float new_y = m_y + (delta*m_speed*moving_vector.y);
        set_position(new_x, new_y);
    }
}

void enemy::set_life(const float new_life) {
    m_life = new_life;
    if (m_life <= 0.0f) {
        was_killed();
    }
}

void enemy::set_speed(const float speed) {
    m_speed = speed;
}