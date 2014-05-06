#include "MainScene.h"
#include "Entity.h"
#include <iostream>

USING_NS_CC;

const std::string MainScene::UP_GUN_DAMAGE      = "GunDamage";
const std::string MainScene::UP_GUN_FIRE_RATE   = "GunFireRate";
const std::string MainScene::UP_SCORE           = "Score";

Scene* MainScene::createScene() {
    auto scene = Scene::create();
    auto layer = MainScene::create();
    scene->addChild(layer);
    return scene;
}

bool MainScene::init() {
    createWorld();
    return true;
}

void MainScene::updateGameObjects(const float delta) {
    std::cout << "Start update game objects" << std::endl;
    for (auto enemy = mEnemies.begin(); enemy != mEnemies.end(); ++enemy) {
        std::cout << "\tupdate enemy " << (*enemy)->getID() << std::endl;
        (*enemy)->update(delta);
        if ((*enemy)->getPositionY() > mOrigin.y + mVisibleSize.height - 10.0f) {
            std::cout << "\tenemy " << (*enemy)->getID() << " kill head, gameOver()" << std::endl;
            (*enemy)->markForRemove();
            gameOver();
            return;
        }
    }
    
    std::cout << "\tGun update" << std::endl;
    mGun->update(delta);
    std::cout << "End update game objects";
}

void MainScene::checkCollisionEnemiesWithBullets() {
    std::cout << "Start check collision enemies with bullets" << std::endl;
    for (auto bullet = mGun->firstBullet(); bullet != mGun->endBullet(); ++bullet) {
        std::cout << "\tcheck bullet " << (*bullet)->getID() << std::endl;
        for (auto enemy = mEnemies.begin(); enemy != mEnemies.end(); ++enemy) {
            std::cout << "\tcheck enemy " << (*enemy)->getID() << std::endl;
            if ((*enemy)->isMarkForRemove() || (*enemy)->isWasKilled()) {
                std::cout << "\tenemy" << (*enemy)->getID() << " already mark for remove, continue" << std::endl;
                continue;
            }
            
            const bool isBulletIntersectEnemy = (*bullet)->getPositionX() > (*enemy)->getPositionX()-(*enemy)->getWidth()/2 &&
                                                (*bullet)->getPositionX() < (*enemy)->getPositionX()+(*enemy)->getWidth()/2 &&
                                                (*bullet)->getPositionY() > (*enemy)->getPositionY()-(*enemy)->getHeight()/2 &&
                                                (*bullet)->getPositionY() < (*enemy)->getPositionY()+(*enemy)->getHeight()/2;
            if (isBulletIntersectEnemy) {
                std::cout << "\tBullet " << (*bullet)->getID() << " intersect with enemy " << (*enemy)->getID() << std::endl;
                (*bullet)->markForRemove();
                std::cout << "\tBullet " << (*bullet)->getID() << " mark for remove" << std::endl;
                std::cout << "\tEnemy " << (*enemy)->getID() << " current life: " << (*enemy)->getLife() << std::endl;
                (*enemy)->setLife((*enemy)->getLife() - mGun->getDamage());
                std::cout << "\tEnemy " << (*enemy)->getID() << " new life: " << (*enemy)->getLife() << std::endl;
                break;
            }
        }
    }
    std::cout << "End check collision enemies with bullets" << std::endl;
}

void MainScene::removeObjectsFromScene() {
    std::cout << "Start remove objects from scene" << std::endl;
    unsigned int addEnemiesCount = 0;
    for (auto enemy = mEnemies.begin(); enemy != mEnemies.end(); ++enemy) {
        if ((*enemy)->isMarkForRemove()) {
            std::cout << "\tenemy " << (*enemy)->getID() << " is mark for remove" << std::endl;
            removeChild((*enemy)->getSprite());
            mEnemies.erase(enemy);
            addEnemiesCount++;
            mScore++;
        } else {
            std::cout << "\tenemy " << (*enemy)->getID() << " not mark for remove" << std::endl;
        }
    }
    std::cout << "End remove objects from scene" << std::endl;
    updateScore();
    addEnemy(addEnemiesCount);
    
    for (auto bullet = mGun->firstBullet(); bullet != mGun->endBullet(); ++bullet) {
        if (((*bullet)->isMarkForRemove()) ||
            (*bullet)->getPositionX() > mVisibleSize.width ||
            (*bullet)->getPositionX() <= 10.0f ||
            (*bullet)->getPositionY() > mVisibleSize.height ||
            (*bullet)->getPositionY() <= 10.0f)
            mGun->removeBullets(bullet);
    }
}

void MainScene::update(const float delta) {
    
    showDebugInfo();
    
    switch (mGameState) {
        case MAIN_GAME_STATE:
            updateGameObjects(delta);
            checkCollisionEnemiesWithBullets();
            removeObjectsFromScene();
            break;
        case LEVEL_UP:
        case GAME_OVER:
        default:
            break;
    }
    

}

void MainScene::showLevelUp() {
    mGameState = LEVEL_UP;

    updateScore();
    

    mUpgradeBackground.reset(new Entity("upgrade_background"));
    mUpgradeBackground->setScale(mVisibleSize.width/mUpgradeBackground->getWidth(), mVisibleSize.height/mUpgradeBackground->getHeight());
    mUpgradeBackground->setPosition(mOrigin.x+(mUpgradeBackground->getWidth()/2), mOrigin.y+(mUpgradeBackground->getHeight()/2));
    mUpgradeBackground->setOpacity(100);

    mUpgradeWindow.reset(new Entity("upgrade_window"));
    const float upgradeWindowWidthScaleFactor = (mVisibleSize.width/mUpgradeWindow->getWidth())*0.9;
    const float upgradeWindowHeightScaleFactor = (mVisibleSize.height/mUpgradeWindow->getHeight())*0.8;
    mUpgradeWindow->setScale(upgradeWindowWidthScaleFactor, upgradeWindowHeightScaleFactor);
    mUpgradeWindow->setPosition(mVisibleSize.width/2, mVisibleSize.height/2);
    
    mStartAfterUpgradeButton.reset(new Entity("start_after_upgrade_button"));
    mStartAfterUpgradeButton->setPosition(mOrigin.x + (mVisibleSize.width/2), mOrigin.y + mStartAfterUpgradeButton->getHeight()/2);

    mUpgradeButton.reset(new Entity("upgrade_text"));
    mUpgradeButton->setPosition(mVisibleSize.width*0.3, mVisibleSize.height*0.38);
    mUpgradeButton->setScale(upgradeWindowWidthScaleFactor, upgradeWindowHeightScaleFactor);
    
    mUpgradeDamageButton.reset(new Entity("upgrade_damage"));
    mUpgradeDamageButton->setPosition(mVisibleSize.width*0.3, mVisibleSize.height*0.65);
    mUpgradeDamageButton->setScale(upgradeWindowWidthScaleFactor, upgradeWindowHeightScaleFactor);
    
    
    char fireRateCost[512];
    snprintf(fireRateCost, 512, "%i$", FIRE_RATE_COST);
    mFireRateCostLabel = Label::create(std::string(fireRateCost), "", 50);
    mFireRateCostLabel->setColor(cocos2d::Color3B(0.0,0.0,0.0));
    mFireRateCostLabel->setPosition(mVisibleSize.width*0.28, mVisibleSize.height*0.45);
    
    char damageCost[512];
    snprintf(damageCost, 512, "%i$", DAMAGE_COST);
    mDamageCostLabel = Label::create(std::string(damageCost), "", 50);
    mDamageCostLabel->setColor(cocos2d::Color3B(0.0,0.0,0.0));
    mDamageCostLabel->setPosition(mVisibleSize.width*0.28, mVisibleSize.height*0.72);
    
    char fireRateLevel[64];
    snprintf(fireRateLevel, 64, "%i", mGun->getFireRateLevel());
    mFireRateLevelLabel = Label::create(std::string(fireRateLevel), "", 50);
    mFireRateLevelLabel->setColor(cocos2d::Color3B(0.0,0.0,0.0));
    mFireRateLevelLabel->setPosition(mVisibleSize.width - (mVisibleSize.width*0.28), mVisibleSize.height*0.35);
    
    char damageLevel[64];
    snprintf(damageLevel, 64, "%i", mGun->getDamageLevel());
    mDamageLevelLabel = Label::create(std::string(damageLevel), "", 50);
    mDamageLevelLabel->setColor(cocos2d::Color3B(0.0,0.0,0.0));
    mDamageLevelLabel->setPosition(mVisibleSize.width - (mVisibleSize.width*0.28), mVisibleSize.height*0.62);
    
    addChild(mUpgradeBackground->getSprite());
    addChild(mUpgradeWindow->getSprite());
    addChild(mUpgradeButton->getSprite());
    addChild(mStartAfterUpgradeButton->getSprite());
    addChild(mUpgradeDamageButton->getSprite());
    addChild(mFireRateCostLabel);
    addChild(mDamageCostLabel);
    addChild(mFireRateLevelLabel);
    addChild(mDamageLevelLabel);
}

void MainScene::getEnemyPosition(float &x, float &y, const float enemyWidth) {
    y = 10.0f;
    bool positionFound = true;
    int tryCounter = 0;
    float farX = 0.0f;
    do {
        x = (float)rand()/((float)RAND_MAX/(mVisibleSize.width));
        for (auto enemy = mEnemies.begin(); enemy != mEnemies.end(); ++enemy) {
            const float distance = (*enemy)->getSprite()->getPosition().getDistance(cocos2d::Point(x,y));
            if (distance < enemyWidth) {
                if (farX < x)
                    farX = x;
                positionFound = false;
                tryCounter++;
                break;
            }
            
        }
        if (tryCounter >= 50) break;
        
    } while (!positionFound);
    if (tryCounter >= 50)
        x = farX;
}

void MainScene::addEnemy(const int count) {
    const float minDistanceToHead = 10.0f;
    const float moveToY = mOrigin.x + mVisibleSize.height;
    for (int i = 0; i < count; ++i) {
        const float enemyLife = (rand() % 3 + 1) * ENEMY_BASE_LIFE;
        std::string enemyType;
        float enemySpeed = 0.0f;
        if (enemyLife <= ENEMY_BASE_LIFE) {
            enemyType = "enemy";
            enemySpeed = 40.0f;
        } else if (enemyLife > ENEMY_BASE_LIFE && enemyLife <= ENEMY_BASE_LIFE*2) {
            enemyType = "enemy_mid";
            enemySpeed = 30.0f;
        } else if (enemyLife > ENEMY_BASE_LIFE*2) {
            enemyType = "enemy_hard";
            enemySpeed = 20.0f;
        }
        
        auto enemy = EnemyPtr(new Enemy(enemyType, minDistanceToHead));
        float xPosition, yPosition;
        getEnemyPosition(xPosition, yPosition, std::max(enemy->getWidth(), enemy->getHeight()));
        enemy->setPosition(xPosition, yPosition);
        enemy->setMoveTo(xPosition, moveToY);
        enemy->setLife(enemyLife);
        enemy->setSpeed(enemySpeed);
        addChild(enemy->getSprite());
        mEnemies.insert(mEnemies.end(), enemy);
        std::cout << "Add new enemy " << enemy->getID() << ", type: " << enemyType << std::endl;
    }
}

void MainScene::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) {
    proceedTouches(touches, event);
}

void MainScene::onTouchesMoved(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event) {
    proceedTouches(touches, event);
}

void MainScene::onTouchesEnded(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event) {
    proceedTouches(touches, event);
    mButtonPressed = false;
}

void MainScene::hideGameOver() {
    removeChild(mLastLabel);
}

void MainScene::proceedTouches(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event) {
    switch (mGameState) {
        case GAME_OVER:
            hideGameOver();
            showLevelUp();
            break;
        case MAIN_GAME_STATE:
            for (auto touch = touches.begin(); touch != touches.end(); ++touch) {
                mGun->setNewAim((*touch)->getLocation().x, (*touch)->getLocation().y);
                rotateRay((*touch)->getLocation());
            }
            break;
        case LEVEL_UP:
            if (mButtonPressed) return;
            mButtonPressed = true;
            for (auto touch = touches.begin(); touch != touches.end(); ++touch) {
                if (!(*touch) || !mUpgradeButton->getSprite() || !mStartAfterUpgradeButton->getSprite()) continue;
                if (mStartAfterUpgradeButton->getSprite()->getBoundingBox().containsPoint((*touch)->getLocation())) {
                    hideLevelUp();
                    break;
                }
                
                if (mUpgradeButton->getSprite()->getBoundingBox().containsPoint((*touch)->getLocation())) {
                    if (mScore < FIRE_RATE_COST) return;
                    mGun->setFireRateLevel(mGun->getFireRateLevel()+1);
                    mScore -= FIRE_RATE_COST;
                    
                    UserDefault::getInstance()->setIntegerForKey(UP_GUN_FIRE_RATE.c_str(), mGun->getFireRateLevel());
                    char fireRateLevel[64];
                    snprintf(fireRateLevel, 64, "%i", mGun->getFireRateLevel());
                    mFireRateLevelLabel->setString(fireRateLevel);
                    updateScore();
                    break;
                }
                
                if (mUpgradeDamageButton->getSprite()->getBoundingBox().containsPoint((*touch)->getLocation())) {
                    if (mScore < DAMAGE_COST) return;
                    mGun->setDamageLevel(mGun->getDamageLevel()+1);
                    UserDefault::getInstance()->setIntegerForKey(UP_GUN_DAMAGE.c_str(), mGun->getDamage());
                    mScore -= DAMAGE_COST;
                    updateScore();
                    char damageLevel[64];
                    snprintf(damageLevel, 64, "%i", mGun->getDamageLevel());
                    mDamageLevelLabel->setString(damageLevel);
                    break;
                }
            }
            break;
        default:
            break;
    }
}

void MainScene::hideLevelUp() {
    removeChild(mUpgradeButton->getSprite());
    removeChild(mStartAfterUpgradeButton->getSprite());
    removeChild(mUpgradeDamageButton->getSprite());
    removeChild(mUpgradeBackground->getSprite());
    removeChild(mUpgradeWindow->getSprite());
    removeChild(mFireRateCostLabel);
    removeChild(mDamageCostLabel);
    removeChild(mFireRateLevelLabel);
    removeChild(mDamageLevelLabel);
    
    for (auto enemy = mEnemies.begin(); enemy != mEnemies.end(); ++enemy) {
        removeChild((*enemy)->getSprite());
        mEnemies.erase(enemy);
    }
    addEnemy(ENEMIES_COUNT);
    
    mGameState = MAIN_GAME_STATE;
}

void MainScene::startNewGame() {
    for (auto enemy = mEnemies.begin(); enemy != mEnemies.end(); ++enemy) {
        removeChild((*enemy)->getSprite());
        mEnemies.erase(enemy);
    }
    addEnemy(ENEMIES_COUNT);

    mGameState = MAIN_GAME_STATE;

    mScore = 0;
    mGun->setFireRateLevel(1);
    mGun->setDamageLevel(1);
    
    updateScore();
}

void MainScene::rotateRay(const cocos2d::Point& toPoint) {
    const Point diffVector = Point(mRay->getPositionX(),mRay->getPositionY()) - toPoint;
    const float rayAngle   = atan2f(diffVector.x, diffVector.y);
    mRay->rotateSprite(rayAngle);
}

std::string MainScene::getScore() const {
    std::ostringstream oss;
    oss << "Score: " << mScore;
    return oss.str();
}

void MainScene::updateScore() {
    UserDefault::getInstance()->setIntegerForKey(UP_SCORE.c_str(), mScore);
    mScoreLabel->setString(getScore());
    mScoreLabel->setPosition(mOrigin.x+mVisibleSize.width-mScoreLabel->getContentSize().width, mOrigin.y+mVisibleSize.height-mScoreLabel->getContentSize().height);
}

void MainScene::gameOver() {
    mGameState = GAME_OVER;
    auto fontFile = FileUtils::getInstance()->fullPathForFilename("Marker Felt");
    char byebyeText[1024];
    snprintf(byebyeText, 1024, "You are dead, you kill %i enemy. Touch screen to upgrade and restart", mScore);

    mLastLabel = Label::create(std::string(byebyeText), fontFile, 40);
    addChild(mLastLabel);


    mLastLabel->setPosition(mOrigin.x + mVisibleSize.width/2 - (mLastLabel->getWidth()/2),
                            mOrigin.y + mVisibleSize.height/2 - (mLastLabel->getHeight()/2));
}

void MainScene::showDebugInfo() {
    if (getChildrenCount() > mMaxObjectsOnScene) {
        mMaxObjectsOnScene = getChildrenCount();
        std::cout << "Max objects count: " << mMaxObjectsOnScene << std::endl;
    }
}

void MainScene::createWorld() {
    mVisibleSize = Director::getInstance()->getVisibleSize();
    mOrigin = Director::getInstance()->getVisibleOrigin();
    
    mBackground.reset(new Entity("background"));
    mBackground->setScale(mVisibleSize.width / mBackground->getWidth(), mVisibleSize.height / mBackground->getHeight());
    mBackground->setPosition(mOrigin.x + (mVisibleSize.width/2), mOrigin.y + (mVisibleSize.height/2));
    addChild(mBackground->getSprite());
    
    
    mEarth.reset(new Entity("earth"));
    mEarth->setScale(mVisibleSize.width / mEarth->getWidth(), (mVisibleSize.height * 0.3f) / mEarth->getHeight());
    mEarth->setPosition(mOrigin.x + (mVisibleSize.width/2), mOrigin.y + mEarth->getHeight()/2);
    addChild(mEarth->getSprite());
    
    
    mHead.reset(new Entity("head"));
    mHead->setScale( ((mVisibleSize.height - (mOrigin.y+mEarth->getHeight()/2))*0.8) / mHead->getHeight() );
    mHead->setPosition(mOrigin.x + (mVisibleSize.width/2), mOrigin.y + mEarth->getHeight() + mHead->getHeight()/2);
    addChild(mHead->getSprite());
    
    
    mEye.reset(new Entity("eye"));
    mEye->setScale(mHead->getHeight()*0.2 / mEye->getHeight());
    mEye->setPosition(mOrigin.x + (mVisibleSize.width/2), mOrigin.y + mHead->getPositionY() + (mHead->getHeight()*0.15));
    addChild(mEye->getSprite());
    
    
    mPupil.reset(new Entity("pupil"));
    mPupil->setScale(mEye->getHeight()*0.6 / mPupil->getHeight());
    mPupil->setPosition(mOrigin.x + (mVisibleSize.width/2), mOrigin.y + mEye->getPositionY());
    addChild(mPupil->getSprite());
    
    
    mRay.reset(new Entity("ray"));
    mRay->setScale(1.0f, mVisibleSize.width/mRay->getHeight());
    mRay->setAnchorPoint(0.5f, 1.0f);
    mRay->setPosition(mPupil->getPositionX(), mPupil->getPositionY());
    mRay->setOpacity(40);
    addChild(mRay->getSprite());
    
    
    const float gunShootToX = mVisibleSize.width/2;
    const float gunShootToY = 0.0f;
    const float gunPositionX = mEye->getPositionX();
    const float gunPositionY = mEye->getPositionY();
    mGun.reset(new Gun(this, gunShootToX, gunShootToY, gunPositionX, gunPositionY));
    
    mScore = UserDefault::getInstance()->getIntegerForKey(UP_SCORE.c_str());
    mGun->setFireRateLevel(UserDefault::getInstance()->getIntegerForKey(UP_GUN_FIRE_RATE.c_str(), 1));
    const int savedGunDamage = UserDefault::getInstance()->getIntegerForKey(UP_GUN_DAMAGE.c_str());
    mGun->setDamageLevel( savedGunDamage ? savedGunDamage : 1 );
    
    auto fontFile = FileUtils::getInstance()->fullPathForFilename("fonts/Marker Felt");
    mScoreLabel = cocos2d::Label::create(getScore(), fontFile, 40);
    mScoreLabel->setAnchorPoint(Point(0.0f,0.0f));
    mScoreLabel->setAlignment(TextHAlignment::RIGHT);
    updateScore();
    addChild(mScoreLabel);
 
    mButtonPressed = true;
    
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan=std::bind(&MainScene::onTouchesBegan, this, std::placeholders::_1, std::placeholders::_2);
    listener->onTouchesMoved=std::bind(&MainScene::onTouchesMoved, this, std::placeholders::_1, std::placeholders::_2);
    listener->onTouchesEnded=std::bind(&MainScene::onTouchesEnded, this, std::placeholders::_1, std::placeholders::_2);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 100);
    
    mMaxObjectsOnScene = 0;
    mDamageCostLabel = NULL;
    mFireRateCostLabel = NULL;
    
    srand(static_cast<unsigned int>(time(0)));
    schedule(schedule_selector(MainScene::update));
    
    addEnemy(ENEMIES_COUNT);
    
    showLevelUp();
}