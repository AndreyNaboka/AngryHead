//
//  Entity.cpp
//  AngryHead
//
//  Created by Andrey Naboka on 10/04/14.
//
//

#include "entity.h"
#include <sstream>

int entity::id = 0;
const std::string KIND = ".png";

entity::entity(const std::string& name)
    :m_name(name)
    ,m_id(id++)
    ,m_need_remove(false)
    ,m_move_to_y(0.0f)
    ,m_move_to_x(0.0f)
{
    m_sprite = cocos2d::Sprite::create(m_name+KIND);
    assert(m_sprite);
    
    //
    // Set new id for entity
    //
    std::ostringstream oss;
    oss << m_name << "_" << m_id;
    m_full_id = oss.str();
}

void entity::set_scale(const float x, const float y) {
    m_sprite->setScale(x, y);
}

void entity::set_position(const float x, const float y) {
    m_x = x;
    m_y = y;
    m_sprite->setPosition(x, y);
}

std::string entity::get_id() const {
    return m_full_id;
}

float entity::get_width() const {
    return m_sprite->getContentSize().width * m_sprite->getScaleX();
}

float entity::get_height() const {
    return m_sprite->getContentSize().height * m_sprite->getScaleY();
}

cocos2d::Sprite* entity::get_sprite() {
    return m_sprite;
}

void entity::set_scale(const float scale) {
    m_sprite->setScale(scale);
}

float entity::get_position_x() const {
    return m_sprite->getPositionX();
}

float entity::get_position_y() const {
    return m_sprite->getPositionY();
}

void entity::rotate_object(const float angle) {
    m_sprite->setRotation(angle*180/M_PI);
}

void entity::set_opacity(const int opacity) {
    m_sprite->setOpacity(opacity);
}

void entity::update(const float delta) {
    //
    // Not need pure entity object
    //
}

void entity::set_move_to(const float x, const float y) {
    m_move_to_x = x;
    m_move_to_y = y;
}

void entity::rotate_sprite(const float angle) {
    m_sprite->setRotation(angle*180/M_PI);
}
void entity::set_anchor_point(const float x, const float y) {
    m_sprite->setAnchorPoint(cocos2d::Point(x,y));
}

void entity::set_new_sprite(const std::string &name) {
    m_sprite->setTexture(name+KIND);
}

GLuint entity::get_opacity() const {
    return m_sprite->getOpacity();
}

