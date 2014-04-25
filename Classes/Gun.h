//
//  Gun.h
//  AngryHead
//
//  Created by Andrey Naboka on 12/04/14.
//
//

#ifndef __AngryHead__Gun__
#define __AngryHead__Gun__

#include "Entity.h"
#include "cocos2d.h"

class Gun {
public:
    Gun(cocos2d::Layer* const parent, const float aimX, const float aimY, const float startX, const float startY);
    void update(const float delta);
    void setNewAim(const float x, const float y);
    std::list<EntityPtr>::iterator firstBullet() { return mBullets.begin(); }
    std::list<EntityPtr>::iterator endBullet()   { return mBullets.end(); }
    void removeBullete(const std::list<EntityPtr>::iterator& it);
    void setNewGunLevel(const int level);
    int getGunLevel() const { return mLevel; }
    float getDamage() const { return mDamage; }
    void setDamage(const float damage) { mDamage = damage; }
    
private:
    void addNewBullets(const int count);
    void updateBullets(const float delta);
    
private:
    cocos2d::Layer* mParent;
    float mDamage;
    std::list<EntityPtr> mBullets;
    
    float mTimeToNextShoot;
    float mAimX, mAimY;
    float mStartX, mStartY;
    float mSpeedPower;
    int   mLevel;
    
    static const float SHOOT_INTERVAL;//    = 0.5f;
    static const float BASE_SPEED;//        = 3000.0f;
};

typedef std::shared_ptr<Gun> GunPtr;

#endif /* defined(__AngryHead__Gun__) */
