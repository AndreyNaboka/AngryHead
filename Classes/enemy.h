//
//  DynamicObject.h
//  AngryHead
//
//  Created by Andrey Naboka on 11/04/14.
//
//

#ifndef __AngryHead__DynamicObject__
#define __AngryHead__DynamicObject__

#include "entity.h"

class enemy : public entity {
public:
    enemy(const std::string& name, const float min_distance);
    virtual void   update(const float delta);
    void           was_killed()                    { m_was_killed = true; }
    bool           is_was_killed() const           { return m_was_killed; }
    float          get_speed() const               { return m_speed; }
    float          get_life() const                { return m_life; }
    void           set_life(const float new_life);
    void           set_speed(const float speed);

private:
    float m_min_distance;
    float m_speed;
    bool  m_was_killed;
    float m_life;
    
    const float TIME_TO_SHOW_EXLOSION = 1.0f;
    const float TIME_TO_SHOW_COIN     = 1.0f;
    float m_time_to_show_explosion;
    float m_time_to_show_coin;
    
    enum ENEMY_STATE {
        NORMAL,
        EXPLOSION,
        COIN
    };
    
    cocos2d::Point m_moving_vector;
        
        
    ENEMY_STATE m_state;

};

typedef std::shared_ptr<enemy> enemy_ptr;

#endif /* defined(__AngryHead__DynamicObject__) */
