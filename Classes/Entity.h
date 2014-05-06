//
//  Entity.h
//  AngryHead
//
//  Created by Andrey Naboka on 10/04/14.
//
//

#ifndef __AngryHead__Entity__
#define __AngryHead__Entity__

#include <string>
#include "cocos2d.h"

class Entity {
public:
    Entity(const std::string& name);
    std::string        getID() const;
    cocos2d::Sprite*   getSprite();
    virtual void       update(const float delta);
    void               setScale(const float x, const float y);
    void               setScale(const float scale);
    void               setPosition(const float x, const float y);
    float              getWidth() const;
    float              getHeight() const;
    float              getPositionY() const;
    float              getPositionX() const;
    void               rotateObject(const float angle);
    void               setOpacity(const int opacity);
    bool               isMarkForRemove() const { return mNeedRemove; }
    void               markForRemove() { mNeedRemove = true; }
    void               setMoveTo(const float x, const float y);
    float              getMoveToX() const { return mMoveToX; }
    float              getMoveToY() const { return mMoveToY; }
    void               rotateSprite(const float angle);
    void               setAnchorPoint(const float x, const float y);
    void               setNewSprite(const std::string& name);
    GLuint             getOpacity() const;
protected:
    std::string        mName;
    std::string        mFullID;
    int                mID;
    cocos2d::Sprite*   mSprite;
    bool               mNeedRemove;
    float              mX, mY;
    float              mMoveToX, mMoveToY;
    
    static int ID;
};

typedef std::shared_ptr<Entity> EntityPtr;

#endif /* defined(__AngryHead__Entity__) */
