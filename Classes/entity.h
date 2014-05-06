//
//  Entity.h
//  AngryHead
//
//  Created by Andrey Naboka on 10/04/14.
//
//

#ifndef __AngryHead__Entity__
#define __AngryHead__Entity__

#include <string>
#include "cocos2d.h"

class entity {
public:
    entity(const std::string& name);
    std::string        get_id() const;
    cocos2d::Sprite*   get_sprite();
    virtual void       update(const float delta);
    void               set_scale(const float x, const float y);
    void               set_scale(const float scale);
    void               set_position(const float x, const float y);
    float              get_width() const;
    float              get_height() const;
    float              get_position_y() const;
    float              get_position_x() const;
    void               rotate_object(const float angle);
    void               set_opacity(const int opacity);
    bool               is_mark_for_remove() const                 { return m_need_remove; }
    void               mark_for_remove()                          { m_need_remove = true; }
    void               set_move_to(const float x, const float y);
    float              get_move_to_x() const                      { return m_move_to_x; }
    float              get_move_to_y() const                      { return m_move_to_y; }
    void               rotate_sprite(const float angle);
    void               set_anchor_point(const float x, const float y);
    void               set_new_sprite(const std::string& name);
    GLuint             get_opacity() const;
protected:
    std::string        m_name;
    std::string        m_full_id;
    int                m_id;
    cocos2d::Sprite*   m_sprite;
    bool               m_need_remove;
    float              m_x, m_y;
    float              m_move_to_x, m_move_to_y;
    
    static int id;
};

typedef std::shared_ptr<entity> entity_ptr;

#endif /* defined(__AngryHead__Entity__) */
