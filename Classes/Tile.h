//
//  Tile.h
//  WhiteTile
//
//  Created by AIRIA on 14-4-24.
//
//

#ifndef __WhiteTile__Tile__
#define __WhiteTile__Tile__

#include <iostream>

#include "BaseSprite.h"
namespace WT{
    class Tile:public BaseSprite
    {
    public:
        /* 是否在渲染 如果在渲染的话 就禁止从缓存池中取出 */
        bool isRendering;
        int row,col;
    public:
        virtual bool ccTouchBegan(CCTouch *pTouch,CCEvent *pEvent);
        static Tile *create();
        virtual void update(float delta);
    };
};


#endif /* defined(__WhiteTile__Tile__) */
