//
//  BaseLayer.h
//  WhiteTile
//
//  Created by AIRIA on 14-4-21.
//
//

#ifndef __WhiteTile__BaseLayer__
#define __WhiteTile__BaseLayer__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class BaseLayer:public CCLayerColor
{

protected:
	virtual bool _isContainPoint(CCTouch *touch);
private:
    CCObject *m_pTarget;
    SEL_MenuHandler m_pHandler;
public:
    virtual bool init();
    virtual void registerWithTouchDispatcher();
    void setTarget(CCObject *pTarget,SEL_MenuHandler sel);
    static BaseLayer *create(const ccColor4B &color,const CCSize &size);
    virtual bool ccTouchBegan(CCTouch *pTouch,CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch,CCEvent *pEvent){
    
    };
    virtual void ccTouchEnded(CCTouch *pTouch,CCEvent *pEvent);
};

#endif /* defined(__WhiteTile__BaseLayer__) */
