//
//  Entity.cpp
//  AngryHead
//
//  Created by Andrey Naboka on 10/04/14.
//
//

#include "Entity.h"
#include <sstream>

int Entity::ID = 0;
const std::string KIND = ".png";

Entity::Entity(const std::string& name)
    :mName(name)
    ,mID(ID++)
    ,mNeedRemove(false)
    ,mMoveToY(0.0f)
    ,mMoveToX(0.0f)
{
    mSprite = cocos2d::Sprite::create(mName+KIND);
    assert(mSprite);
    
    //
    // Set new id for entity
    //
    std::ostringstream oss;
    oss << mName << "_" << mID;
    mFullID = oss.str();
}

void Entity::setScale(const float x, const float y) {
    mSprite->setScale(x, y);
}

void Entity::setPosition(const float x, const float y) {
    mX = x;
    mY = y;
    mSprite->setPosition(x, y);
}

std::string Entity::getID() const {
    return mFullID;
}

float Entity::getWidth() const {
    return mSprite->getContentSize().width * mSprite->getScaleX();
}

float Entity::getHeight() const {
    return mSprite->getContentSize().height * mSprite->getScaleY();
}

cocos2d::Sprite* Entity::getSprite() {
    return mSprite;
}

void Entity::setScale(const float scale) {
    mSprite->setScale(scale);
}

float Entity::getPositionX() const {
    return mSprite->getPositionX();
}

float Entity::getPositionY() const {
    return mSprite->getPositionY();
}

void Entity::rotateObject(const float angle) {
    mSprite->setRotation(angle*180/M_PI);
}

void Entity::setOpacity(const int opacity) {
    mSprite->setOpacity(opacity);
}

void Entity::update(const float delta) {
    //
    // Not need pure entity object
    //
}

void Entity::setMoveTo(const float x, const float y) {
    mMoveToX = x;
    mMoveToY = y;
}

void Entity::rotateSprite(const float angle) {
    mSprite->setRotation(angle*180/M_PI);
}
void Entity::setAnchorPoint(const float x, const float y) {
    mSprite->setAnchorPoint(cocos2d::Point(x,y));
}

void Entity::setNewSprite(const std::string &name) {
    mSprite->setTexture(name+KIND);
}

GLuint Entity::getOpacity() const {
    mSprite->getOpacity();
}

