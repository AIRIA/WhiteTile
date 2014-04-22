//
//  BaseLayer.cpp
//  WhiteTile
//
//  Created by AIRIA on 14-4-21.
//
//

#include "BaseLayer.h"
bool BaseLayer::init()
{
    if (!CCLayerColor::init()) {
        return false;
    }
    return true;
}

BaseLayer *BaseLayer::create(const ccColor4B &color,const CCSize &size)
{
    BaseLayer *layer = new BaseLayer();
    if (layer && layer->initWithColor(color, size.width, size.height)) {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

void BaseLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

bool BaseLayer::_isContainPoint(cocos2d::CCTouch *touch)
{
    CCPoint arPoint = getAnchorPointInPoints();
    CCPoint point = convertTouchToNodeSpaceAR(touch);
    CCRect rect = boundingBox();
    CCSize spriteSize = rect.size;
    CCRect spriteRect = CCRectMake(-arPoint.x,-arPoint.y,spriteSize.width,spriteSize.height);
    return spriteRect.containsPoint(point);
}

void BaseLayer::setTarget(cocos2d::CCObject *pTarget, SEL_MenuHandler sel)
{
    this->m_pTarget = pTarget;
    this->m_pHandler = sel;
}

bool BaseLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(_isContainPoint(pTouch))
    {
        return true;//吞噬其他的事件 禁止事件继续传播
    }
    return false;
}

void BaseLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(m_pTarget!=NULL&&m_pHandler!=NULL)
    {
        (m_pTarget->*m_pHandler)(this);
    }
}