//
//  BaseSprite.cpp
//  WhiteTile
//
//  Created by AIRIA on 14-4-24.
//
//

#include "BaseSprite.h"

BaseSprite::BaseSprite():m_iTouchPriority(0),
m_bSwallow(false),
m_pTargetBegan(NULL),
m_pTargetEnded(NULL),
m_pTargetMove(NULL),
m_bTouchEnable(true)
{
    //setTouchEnabled(false);
}

void BaseSprite::onEnter()
{
    if(m_bTouchEnable)
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,m_iTouchPriority,true);
    }
    CCSprite::onEnter();
}

void BaseSprite::onExit()
{
	CCLog("BaseSprite onExit invoke : remove touch delegate");
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}

BaseSprite * BaseSprite::createSpriteWithFrameName( const char *frameName )
{
    BaseSprite *bs = new BaseSprite();
    if(bs&&bs->initWithSpriteFrameName(frameName))
    {
        bs->autorelease();
        return bs;
    }
    return NULL;
}

BaseSprite * BaseSprite::create( const char *fileName )
{
    BaseSprite *bs = new BaseSprite();
    if(bs&&bs->initWithFile(fileName))
    {
        bs->autorelease();
        return bs;
    }
    return NULL;
}

bool BaseSprite::_isContainPoint( CCTouch *touch )
{
    CCPoint arPoint = getAnchorPointInPoints();
    CCPoint point = convertTouchToNodeSpaceAR(touch);
    CCRect rect = boundingBox();
    CCSize spriteSize = rect.size;
    CCRect spriteRect = CCRectMake(-arPoint.x,-arPoint.y,spriteSize.width,spriteSize.height);
    return spriteRect.containsPoint(point);
}

bool BaseSprite::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    if(_isContainPoint(pTouch))
    {
        CCLog("Touch Began");
        if(m_pTargetBegan&&beganHandler)
        {
            (m_pTargetBegan->*beganHandler)(this);
        }
        return true;//swallow other event
    }
    return false;
}

void BaseSprite::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
    if(m_pTargetMove&&moveHandler)
    {
        (m_pTargetMove->*moveHandler)(this);
    }
}

void BaseSprite::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
    if(m_pTargetEnded&&endedHandler)
    {
        CCLog("BaseSprite Touched Ended");
        (m_pTargetEnded->*endedHandler)(this);
    }
}

void BaseSprite::setTargetBegan( CCObject *target,SEL_MenuHandler sel )
{
    this->m_pTargetBegan = target;
    this->beganHandler = sel;
}

void BaseSprite::setTargetMove( CCObject *target,SEL_MenuHandler sel )
{
    this->m_pTargetMove = target;
    this->moveHandler = sel;
}

void BaseSprite::setTargetEnded( CCObject *target,SEL_MenuHandler sel )
{
    this->m_pTargetEnded = target;
    this->endedHandler = sel;
}

void BaseSprite::setTouchEnabled( bool enabled )
{
    if (m_bTouchEnable!= enabled)
    {
        m_bTouchEnable = enabled;
        if (m_bRunning)
        {
            if (enabled)
            {
                CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,m_iTouchPriority,m_bSwallow);
            }
            else
            {
                CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
            }
        }
    }
}

void BaseSprite::setTouchPriority( int priority )
{
    //在设置的权重的时候 先移除掉事件代码 然后重新添加 来设置权重
    if(m_iTouchPriority!=priority)
    {
        m_iTouchPriority = priority;
        if(m_bTouchEnable)
        {
            setTouchEnabled(false);
            setTouchEnabled(true);
        }
    }
}