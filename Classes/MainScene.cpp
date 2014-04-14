#include "MainScene.h"
#include "Entity.h"
#include <iostream>

USING_NS_CC;

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

void MainScene::update(const float delta) {
    
    if (getChildrenCount() > mMaxObjectsOnScene) {
        mMaxObjectsOnScene = getChildrenCount();
        std::cout << "Max objects count: " << mMaxObjectsOnScene << std::endl;
    }
    
    for (auto enemy = mEnemies.begin(); enemy != mEnemies.end(); ++enemy)
        (*enemy)->update(delta);
    
    mGun->update(delta);
    
    //
    // Check collision bullets with enemies
    //
    for (auto bullet = mGun->firstBullet(); bullet != mGun->endBullet(); ++bullet) {
        for (auto enemy = mEnemies.begin(); enemy != mEnemies.end(); ++enemy) {
            
            if ((*enemy)->isMarkForRemove()) continue;
            
            const bool bulletRightThanLeftLineEnemy = (*bullet)->getPositionX() > (*enemy)->getPositionX()-(*enemy)->getWidth()/2;
            const bool bulletLeftThanRightLineEnemy = (*bullet)->getPositionX() < (*enemy)->getPositionX()+(*enemy)->getWidth()/2;
            const bool bulletTopThanBottomLineEnemy = (*bullet)->getPositionY() > (*enemy)->getPositionY()-(*enemy)->getHeight()/2;
            const bool bulletBottomThanTopLineEnemy = (*bullet)->getPositionY() < (*enemy)->getPositionY()+(*enemy)->getHeight()/2;
            
            if (bulletRightThanLeftLineEnemy && bulletLeftThanRightLineEnemy && bulletTopThanBottomLineEnemy && bulletBottomThanTopLineEnemy) {
                (*bullet)->markForRemove();
                (*enemy)->markForRemove();
                (*enemy)->wasKilled();
                mScore++;
                updateScore();
                break;
            }
        }
    }
    
    
    //
    // Remove enemies whose touch to head
    //
    unsigned int addEnemiesCount = 0;
    for (auto enemy = mEnemies.begin(); enemy != mEnemies.end(); ++enemy) {
        if ((*enemy)->isMarkForRemove() && (*enemy)->canRemove()) {
            removeChild((*enemy)->getSprite());
            mEnemies.erase(enemy);
            addEnemiesCount++;
        }
    }
    addEnemy(addEnemiesCount);
    
    //
    // Remove out of bounds bullets
    //
    for (auto bullet = mGun->firstBullet(); bullet != mGun->endBullet(); ++bullet) {
        if (((*bullet)->isMarkForRemove() && (*bullet)->canRemove()) ||
             (*bullet)->getPositionX() > mVisibleSize.width ||
             (*bullet)->getPositionX() <= 10.0f ||
             (*bullet)->getPositionY() > mVisibleSize.height ||
             (*bullet)->getPositionY() <= 10.0f)
            mGun->removeBullete(bullet);
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
 
    mScore = 0;
    auto fontFile = FileUtils::getInstance()->fullPathForFilename("fonts/Marker Felt");
    mScoreLabel = cocos2d::Label::create(getScore(), fontFile, 40);
    mScoreLabel->setAnchorPoint(Point(0.0f,0.0f));
    mScoreLabel->setAlignment(TextHAlignment::RIGHT);
    updateScore();
    addChild(mScoreLabel);
    
    
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan=std::bind(&MainScene::onTouchesBegan, this, std::placeholders::_1, std::placeholders::_2);
    listener->onTouchesMoved=std::bind(&MainScene::onTouchesMoved, this, std::placeholders::_1, std::placeholders::_2);
    listener->onTouchesEnded=std::bind(&MainScene::onTouchesEnded, this, std::placeholders::_1, std::placeholders::_2);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 100);
    
    mMaxObjectsOnScene = 0;
    
    srand(static_cast<unsigned int>(time(0)));
    schedule(schedule_selector(MainScene::update));
    
    addEnemy(ENEMIES_COUNT);
}

void MainScene::addEnemy(const int count) {
    const float minDistanceToHead = 10.0f;
    const float moveToY = mHead->getPositionY() - mHead->getHeight()/2;
    for (int i = 0; i < count; ++i) {
        const float xPosition = (float)rand()/((float)RAND_MAX/(mVisibleSize.width));
        const float yPosition = 10.0f;
        auto enemy = mEnemies.insert(mEnemies.end(), EnemyPtr(new Enemy("enemy", minDistanceToHead)));
        (*enemy)->setMoveTo(xPosition, moveToY);
        (*enemy)->setPosition(xPosition, yPosition);
        addChild((*enemy)->getSprite());
    }
}

void MainScene::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event) {
    for (auto touch = touches.begin(); touch != touches.end(); ++touch) {
        mGun->setNewAim((*touch)->getLocation().x, (*touch)->getLocation().y);
        rotateRay((*touch)->getLocation());
    }
}

void MainScene::onTouchesMoved(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event) {
    for (auto touch = touches.begin(); touch != touches.end(); ++touch) {
        mGun->setNewAim((*touch)->getLocation().x, (*touch)->getLocation().y);
        rotateRay((*touch)->getLocation());
    }
}

void MainScene::onTouchesEnded(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event) {
    
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
    mScoreLabel->setString(getScore());
    mScoreLabel->setPosition(mOrigin.x+mVisibleSize.width-mScoreLabel->getContentSize().width, mOrigin.y+mVisibleSize.height-mScoreLabel->getContentSize().height);
}