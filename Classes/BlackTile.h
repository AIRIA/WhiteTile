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
private:
    CCLayerColor *gray;
public:
    int row,col;
    int index;
    virtual bool init();
    bool m_bIsTouched;
    static BlackTile *create(float width,float height);
    virtual bool ccTouchBegan(CCTouch *pTouch,CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch,CCEvent *pEvent);
};

#endif /* defined(__WhiteTile__BlackTile__) */
