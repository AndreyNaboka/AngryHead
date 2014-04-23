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

class Enemy;

class EnemyFactory {
private:
    EnemyFactory() {}
    EnemyFactory(const EnemyFactory& rhs) {}
    EnemyFactory& operator=(const EnemyFactory& rhs);
public:
    virtual ~EnemyFactory();
    static EnemyFactory* getInstance();
    Enemy* getNewEnemy(const std::string& name, const float minDistance);
    void removeEnemy(const std::string& enemyName);
    
    struct EnemyCoords {
        float x;
        float y;
    };
private:
    static EnemyFactory* mFactory;
    typedef std::map<std::string, EnemyCoords> EnemiesPositionList;
    typedef EnemiesPositionList::iterator EnemiesPositionListIterator;
    EnemiesPositionList mEnemiesPosition;
};

class Enemy : public Entity {
public:
    virtual void update(const float delta);
    virtual bool isCanRemove() const { return mCanRemove; }
    void wasKilled() { mWasKilled = true; }
    bool enemyKillHead() const { return mMovingVector.getLength() < mMinDistance; }
    static void setNewSpeed(const float minSpeed, const float maxSpeed) { mMinSpeed = minSpeed, mMaxSpeed = maxSpeed; }
    static float getMinSpeed() { return mMinSpeed; }
    static float getMaxSpeed() { return mMaxSpeed; }
private:
    Enemy(const std::string& name, const float minDistance);
    Enemy();
    Enemy(const Enemy&);
    Enemy& operator=(const Enemy&);
    
protected:
    float mMinDistance;
    float mSpeed;
    bool  mCanRemove;
    bool  mWasKilled;
    
    static float mMaxSpeed, mMinSpeed;
    
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
    
    friend EnemyFactory;
};

typedef std::shared_ptr<Enemy> EnemyPtr;

#endif /* defined(__AngryHead__DynamicObject__) */
