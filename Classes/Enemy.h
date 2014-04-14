//
//  DynamicObject.h
//  AngryHead
//
//  Created by Andrey Naboka on 11/04/14.
//
//

#ifndef __AngryHead__DynamicObject__
#define __AngryHead__DynamicObject__

#include "Entity.h"

class Enemy : public Entity {
public:
    Enemy(const std::string& name, const float minDistance);
    virtual void update(const float delta);
    virtual bool canRemove() const { return mCanRemove; }
    void wasKilled() { mWasKilled = true; }
    bool enemyKillHead() const { return mMovingVector.getLength() < mMinDistance; }
protected:
    float mMinDistance;
    float mSpeed;
    bool  mCanRemove;
    bool  mWasKilled;
    
    const float MIN_SPEED = 20.0f;
    const float MAX_SPEED = 40.0f;
    
    const float TIME_TO_SHOW_EXLOSION = 1.0f;
    const float TIME_TO_SHOW_COIN     = 1.0f;
    float mTimeToShowExplosion;
    float mTimeToShowCoin;
    
    enum ENEMY_STATE {
        NORMAL,
        EXPLOSION,
        COIN
    };
    
    cocos2d::Point mMovingVector;
        
        
    ENEMY_STATE mState;
};

typedef std::shared_ptr<Enemy> EnemyPtr;

#endif /* defined(__AngryHead__DynamicObject__) */
