//
//  Gun.cpp
//  AngryHead
//
//  Created by Andrey Naboka on 12/04/14.
//
//

#include "Gun.h"
#include <iostream>

const float Gun::SHOOT_INTERVAL = 0.5f;
const float Gun::BASE_SPEED = 3000.0f;

Gun::Gun(cocos2d::Layer* const parent, const float aimX, const float aimY, const float startX, const float startY)
    :mTimeToNextShoot(SHOOT_INTERVAL)
    ,mAimX(aimX)
    ,mAimY(aimY)
    ,mStartX(startX)
    ,mStartY(startY)
    ,mParent(parent)
    ,mSpeedPower(1000.0f)
    ,mLevel(0)
{
    addNewBullets(1);
}

void Gun::update(const float delta) {
    mTimeToNextShoot -= delta;
    if (mTimeToNextShoot <= 0.0f) {
        addNewBullets(1);
        mTimeToNextShoot = SHOOT_INTERVAL;
    }
    updateBullets(delta);
}

void Gun::updateBullets(const float delta) {
    for (auto bullet = mBullets.begin(); bullet != mBullets.end(); ++bullet) {
        cocos2d::Point bulletDirection((*bullet)->getMoveToX(), (*bullet)->getMoveToY());
        cocos2d::Point diffVector = cocos2d::Point(mStartX,mStartY)-bulletDirection;
        float angle = M_PI_2 - atan2f(diffVector.x,diffVector.y);
        cocos2d::Point directionVector = cocos2d::Point(cosf(-1*angle), sinf(-1*angle));
        directionVector.normalize();
        float newX = (*bullet)->getPositionX() - (delta*(BASE_SPEED+mSpeedPower)*directionVector.x);
        float newY = (*bullet)->getPositionY() + (delta*(BASE_SPEED+mSpeedPower)*directionVector.y);
        (*bullet)->setPosition(newX,newY);
    }
}

void Gun::addNewBullets(const int count) {
    for (int i = 0; i < count; ++i) {
        auto bullet = mBullets.insert(mBullets.end(), EntityPtr(new Entity("bullet")));
        (*bullet)->setPosition(mStartX, mStartY);
        (*bullet)->setMoveTo(mAimX, mAimY);
        mParent->addChild((*bullet)->getSprite());
    }
}

void Gun::setNewAim(const float x, const float y) {
    mAimX = x;
    mAimY = y;
}

void Gun::removeBullete(const std::list<EntityPtr>::iterator &it) {
    mParent->removeChild((*it)->getSprite());
    mBullets.erase(it);
}

void Gun::setNewGunLevel(const int level) {
    mLevel = level;
    mSpeedPower *= level;
}