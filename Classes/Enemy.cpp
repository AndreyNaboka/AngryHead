//
//  DynamicObject.cpp
//  AngryHead
//
//  Created by Andrey Naboka on 11/04/14.
//
//

#include "Enemy.h"
#include "cocos2d.h"

Enemy::Enemy(const std::string& name, const float minDistance)
    :Entity(name)
    ,mMinDistance(minDistance)
    ,mTimeToShowCoin(TIME_TO_SHOW_COIN)
    ,mTimeToShowExplosion(TIME_TO_SHOW_EXLOSION)
    ,mState(NORMAL)
    ,mCanRemove(false)
    ,mWasKilled(false)
{
    mSpeed = MIN_SPEED + (float)rand()/((float)RAND_MAX/(MAX_SPEED-MIN_SPEED));
}

void Enemy::update(const float delta) {
    if (isMarkForRemove() && mWasKilled) {
        if (mTimeToShowExplosion > 0.0f) {
            mTimeToShowExplosion -= delta;
            
            if (mState == NORMAL) {
                setNewSprite("enemy_death");
                mState = EXPLOSION;
            }
            
            GLuint newOpacity = 255 * mTimeToShowExplosion;
            setOpacity(newOpacity);
            
        } else if (mTimeToShowExplosion <= 0.0f && mTimeToShowCoin > 0.0f) {
            mTimeToShowCoin -= delta;
            
            if (mState == EXPLOSION) {
                mState = COIN;
                setNewSprite("coin");
            }
            
            GLuint newOpacity = 255 * mTimeToShowCoin;
            setOpacity(newOpacity);
            
        } else if (mTimeToShowCoin <= 0.0f && mTimeToShowCoin <= 0.0f) {
            mCanRemove = true;
        }
        return;
    }

    mMovingVector = cocos2d::Point(mMoveToX-mX, mMoveToY-mY);
    if (mMovingVector.getLength()==0) {
        markForRemove();
        mCanRemove = true;
        return;
    }
    
    cocos2d::Point movingVector = mMovingVector/mMovingVector.getLength();
    const float newX = mX + (delta*mSpeed*movingVector.x);
    const float newY = mY + (delta*mSpeed*movingVector.y);
    setPosition(newX, newY);
}