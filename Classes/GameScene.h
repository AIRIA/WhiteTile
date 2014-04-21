//
//  GameScene.h
//  WhiteTile
//
//  Created by AIRIA on 14-4-21.
//
//

#ifndef __WhiteTile__GameScene__
#define __WhiteTile__GameScene__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class GameScene:public CCLayer
{
private:
    /* 现实引导文字介绍 */
    void showGuide();
    
public:
    static CCScene *scene();
    virtual bool init();
    CREATE_FUNC(GameScene);
};

#endif /* defined(__WhiteTile__GameScene__) */
