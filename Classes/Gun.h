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
    
    //
    // Common
    void    update(const float delta);
    void    setNewAim(const float x, const float y);
    void    removeBullets(const std::list<EntityPtr>::iterator& it);
    
    //
    // Fire power controls
    void    setFireRateLevel(const int level);
    int     getFireRateLevel() const             { return mFireRateLevel; }
    float   getDamage() const                    { return DAMAGE_BASE * getDamageLevel(); }
    int     getDamageLevel() const               { return mDamageLevel; }
    void    setDamageLevel(const int level)      { mDamageLevel = level; }
    
    //
    // Bullets controls
    std::list<EntityPtr>::iterator firstBullet() { return mBullets.begin(); }
    std::list<EntityPtr>::iterator endBullet()   { return mBullets.end(); }
    
private:
    void addNewBullets(const int count);
    void updateBullets(const float delta);
    
private:
    cocos2d::Layer* mParent;
    std::list<EntityPtr> mBullets;

    float mTimeToNextShoot;
    float mAimX, mAimY;
    float mStartX, mStartY;
    float mSpeedPower;
    int   mFireRateLevel;
    int   mDamageLevel;
    
    static const float DAMAGE_BASE;
    static const float SHOOT_INTERVAL;//    = 0.5f;
    static const float BASE_SPEED;//        = 3000.0f;
};

typedef std::shared_ptr<Gun> GunPtr;

#endif /* defined(__AngryHead__Gun__) */
