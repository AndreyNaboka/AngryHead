//
//  Gun.h
//  AngryHead
//
//  Created by Andrey Naboka on 12/04/14.
//
//

#ifndef __AngryHead__Gun__
#define __AngryHead__Gun__

#include "entity.h"
#include "cocos2d.h"

class gun {
public:
    gun(cocos2d::Layer* const parent,
        const float aim_x,
        const float aim_y,
        const float start_x,
        const float start_y);
    
    //
    // Common
    void    update(const float delta);
    void    set_new_aim(const float x, const float y);
    void    remove_bullets(const std::list<entity_ptr>::iterator& it);
    
    //
    // Fire power controls
    void    set_fire_rate_level(const int level);
    int     get_fire_rate_level() const             { return m_fire_rate_level; }
    float   get_damage() const                      { return DAMAGE_BASE * get_damage_level(); }
    int     get_damage_level() const                { return m_damage_level; }
    void    set_damage_level(const int level)       { m_damage_level = level; }
    
    //
    // Bullets controls
    std::list<entity_ptr>::iterator first_bullet() { return m_bullets.begin(); }
    std::list<entity_ptr>::iterator end_bullet()   { return m_bullets.end(); }
    
private:
    void add_new_bullets(const int count);
    void update_bullets(const float delta);
    
private:
    cocos2d::Layer* m_parent;
    std::list<entity_ptr> m_bullets;

    float m_time_to_next_shoot;
    float m_aim_x, m_aim_y;
    float m_start_x, m_start_y;
    float m_speed_power;
    int   m_fire_rate_level;
    int   m_damage_level;
    float m_shoot_interval;
    
    static const float SHOOT_INTERVAL_STEP;
    static const float DAMAGE_BASE;
    static const float BASE_SPEED;//        = 3000.0f;
};

typedef std::shared_ptr<gun> gun_ptr;

#endif /* defined(__AngryHead__Gun__) */
