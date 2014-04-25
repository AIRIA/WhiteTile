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
    std::vector<int> m_vBlackTags;
private:
    int tileTag;
    int activeTiles;
    int tileCount;
    CCSize m_winSize;
    CCSpriteBatchNode *tileBatchNode;
    CCLabelTTF *scoreLabel,*scoreLabelShadow;
    float m_fTileWidth,m_fTileHeight;
    int m_nVerticalTiles,m_nHorizontalTiles;
    CCArray *tilePools;
    float m_fTileScaleX,m_fTileScaleY;
    
    /* 显示引导文字介绍 */
    void showGuide();
    /* 创建层里面的白块儿 */
    void createTile(int rows);
    
    void initLayers();
    void __showResult();
    void __backHandler(CCObject *pSender);
    void __retryHandler(CCObject *pSender);
    void __endGame(CCObject *obj);
    void __updateScore(CCObject *obj);
    void __tileTouchDownHandler(CCObject *pSender);
    void __tileTouchUpHandler(CCObject *pSender);
    void __whiteTileTouchHandler(CCObject *pSender);
    void __rollBackHandler(CCObject *pSender,void *param);
    void __blinkHandler();
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
