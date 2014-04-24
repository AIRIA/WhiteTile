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
#include "BlackTile.h"

using namespace cocos2d;

class GameScene:public CCLayer
{
private:
    int screens;
    int tileCount;
    int score;
    CCSize m_winSize;
    CCLayer *scrollLayer;
    
    CCLabelTTF *scoreLabel,*scoreLabelShadow;
    /* 现实引导文字介绍 */
    void showGuide();
    /* 创建层里面的白块儿 */
    void createTile(CCLayer *layer,int horizontalTiles=4,int verticalTiles=4,int blackTiles=1,bool isstart=false);
    
    void initLayers();
    void __showResult();
    void __backHandler(CCObject *pSender);
    void __retryHandler(CCObject *pSender);
    void __endGame(CCObject *obj);
    void __updateScore(CCObject *obj);
    void __updateScrollLayerPosition(CCObject *obj);
public:
    
    
    void startGame(CCObject *pSender);
    static CCScene *scene();
    virtual void update(float f);
    virtual bool init();
    CREATE_FUNC(GameScene);
    virtual void onEnter();
    virtual void onExit();
};

#endif /* defined(__WhiteTile__GameScene__) */
