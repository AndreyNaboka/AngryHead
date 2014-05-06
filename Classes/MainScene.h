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
    void proceedTouches(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event *event);
    void hideLevelUp();
    void hideGameOver();
    void getEnemyPosition(float& x, float& y, const float enemyWidth);
    
private:
    //
    // Level objects
    EntityPtr mBackground;
    EntityPtr mEarth;
    EntityPtr mHead;
    EntityPtr mEye;
    EntityPtr mPupil;
    EntityPtr mRay;
    
    
    //
    // Upgrade window objects
    EntityPtr mUpgradeButton;
    EntityPtr mStartAfterUpgradeButton;
    EntityPtr mUpgradeBackground;
    EntityPtr mUpgradeWindow;
    EntityPtr mUpgradeDamageButton;
    cocos2d::Label* mDamageCostLabel;
    cocos2d::Label* mFireRateCostLabel;
    cocos2d::Label* mFireRateLevelLabel;
    cocos2d::Label* mDamageLevelLabel;
    
    
    GunPtr mGun;
    
    bool mButtonPressed;
    size_t mMaxObjectsOnScene;
    cocos2d::Size mVisibleSize;
    cocos2d::Point mOrigin;
    cocos2d::Label* mScoreLabel;
    cocos2d::Label* mLastLabel;
    unsigned int mScore;
    
    std::list<EnemyPtr> mEnemies;
    
    
    //
    // Main consts
    static const int ENEMIES_COUNT   = 7;
    static const int FIRE_RATE_COST  = 100;
    static const int DAMAGE_COST     = 100;
    static const int ENEMY_BASE_LIFE = 25;
    static const std::string UP_GUN_DAMAGE;
    static const std::string UP_GUN_FIRE_RATE;
    static const std::string UP_SCORE;
    
    enum GAME_STATE {
        MAIN_GAME_STATE,
        GAME_OVER,
        LEVEL_UP
    };
    
    GAME_STATE mGameState;
};

#endif // __HELLOWORLD_SCENE_H__
