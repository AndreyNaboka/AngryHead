#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Entity.h"
#include "Enemy.h"
#include "Gun.h"
#include <list>


class MainScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void update(const float delta);
    
    virtual void onTouchesEnded(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event *event);
    virtual void onTouchesMoved(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event *event);
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>&, cocos2d::Event*);
    
protected:
    void createWorld();
    void addEnemy(const int count);
    CREATE_FUNC(MainScene);
    void rotateRay(const cocos2d::Point& toPoint);
    
protected:
    EntityPtr mBackground;
    EntityPtr mEarth;
    EntityPtr mHead;
    EntityPtr mEye;
    EntityPtr mPupil;
    EntityPtr mRay;
    
    GunPtr mGun;
    
    size_t mMaxObjectsOnScene;
    
    cocos2d::Size mVisibleSize;
    
    std::list<EnemyPtr> mEnemies;
    
    static const int ENEMIES_COUNT = 10;
};

#endif // __HELLOWORLD_SCENE_H__
