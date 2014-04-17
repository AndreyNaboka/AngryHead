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
    
private:
    void createWorld();
    void addEnemy(const int count);
    CREATE_FUNC(MainScene);
    void rotateRay(const cocos2d::Point& toPoint);
    std::string getScore() const;
    void updateScore();
    void startNewGame();
    void gameOver();
    void updateGameObjects(const float delta);
    void showDebugInfo();
    void checkCollisionEnemiesWithBullets();
    void removeObjectsFromScene();
    void showLevelUp();
    
private:
    EntityPtr mBackground;
    EntityPtr mEarth;
    EntityPtr mHead;
    EntityPtr mEye;
    EntityPtr mPupil;
    EntityPtr mRay;
    
    GunPtr mGun;
    
    size_t mMaxObjectsOnScene;
    cocos2d::Size mVisibleSize;
    cocos2d::Point mOrigin;
    cocos2d::Label* mScoreLabel;
    cocos2d::Label* mLastLabel;
    unsigned int mScore;
    unsigned int mMoney;
    unsigned int mEnemiesForNextLevel;
    unsigned int mLevel;
    
    std::list<EnemyPtr> mEnemies;
    
    static const int ENEMIES_COUNT = 10;
    static const int LEVELS[];
    
    
    enum GAME_STATE {
        MAIN_GAME_STATE,
        GAME_OVER,
        LEVEL_UP
    };
    
    GAME_STATE mGameState;
};

#endif // __HELLOWORLD_SCENE_H__
