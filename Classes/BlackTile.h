//
//  BlackTile.h
//  WhiteTile
//
//  Created by AIRIA on 14-4-22.
//
//

#ifndef __WhiteTile__BlackTile__
#define __WhiteTile__BlackTile__

#include <iostream>

#include "BaseLayer.h"

class BlackTile:public BaseLayer
{
protected:
    virtual bool _isContainPoint(CCTouch *touch);
private:
    CCSize m_winSize;
    CCLayerColor *gray;
public:
    virtual void onEnter();
    int row,col;
    int index;
    virtual bool init();
    bool m_bIsTouched;
    virtual void update(float delta);
    static BlackTile *create(float width,float height);
    virtual bool ccTouchBegan(CCTouch *pTouch,CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch,CCEvent *pEvent);
};

#endif /* defined(__WhiteTile__BlackTile__) */
