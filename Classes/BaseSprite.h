//
//  BaseSprite.h
//  WhiteTile
//
//  Created by AIRIA on 14-4-24.
//
//

#ifndef __WhiteTile__BaseSprite__
#define __WhiteTile__BaseSprite__

#include <iostream>

#include "cocos2d.h"

using namespace cocos2d;

class BaseSprite:public CCSprite,public CCTargetedTouchDelegate
{
protected:
	bool _isContainPoint(CCTouch *touch);
private:
    bool m_bTouchEnable;
    CCObject *m_pTargetBegan;
    SEL_MenuHandler beganHandler;
    CCObject *m_pTargetMove;
    SEL_MenuHandler moveHandler;
    CCObject *m_pTargetEnded;
    SEL_MenuHandler endedHandler;
public:
	bool m_bSwallow;
    int m_iTouchPriority;
    static BaseSprite *createSpriteWithFrameName(const char *frameName);
    static BaseSprite *create(const char *fileName);
    virtual void onEnter();
    virtual void onExit();
    BaseSprite();
    void setTargetBegan(CCObject *target,SEL_MenuHandler sel);
    void setTargetMove(CCObject *target,SEL_MenuHandler sel);
    void setTargetEnded(CCObject *target,SEL_MenuHandler sel);
    void setTouchEnabled(bool enable);
    void setTouchPriority(int proirity);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    /* */
};

#endif /* defined(__WhiteTile__BaseSprite__) */
