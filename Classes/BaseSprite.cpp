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
    CCSize size = getContentSize();
    CCPoint anchorPoint = getAnchorPoint();
    CCPoint archorPos = getAnchorPointInPoints();
    CCPoint arPoint = ccp(size.width*getScaleX()*anchorPoint.x/0.8f,size.height*getScaleY()*anchorPoint.y/0.8f);
    
    CCPoint point = convertToNodeSpaceAR(touch->getLocation());
    point = ccp(point.x*getScaleX(),point.y*getScaleY());
    
    CCRect rect = boundingBox();
    CCSize spriteSize = rect.size;
    spriteSize = CCSizeMake(spriteSize.width/0.8, spriteSize.height/0.8f);
    CCRect spriteRect = CCRectMake(-arPoint.x,-arPoint.y-spriteSize.height/3,spriteSize.width,spriteSize.height+spriteSize.height/1.5);
    return spriteRect.containsPoint(point);
}

bool BaseSprite::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    if(_isContainPoint(pTouch))
    {
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