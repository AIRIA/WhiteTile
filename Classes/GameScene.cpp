//
//  GameScene.cpp
//  WhiteTile
//
//  Created by AIRIA on 14-4-21.
//
//

#include "GameScene.h"
#include "BaseLayer.h"
#include "GameConfig.h"
#include "VisibleRect.h"
#include "BlackTile.h"

CCScene *GameScene::scene()
{
    CCScene *scene = CCScene::create();
    scene->addChild(GameScene::create());
    return scene;
}

bool GameScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    score = 0;
    m_winSize = CCDirector::sharedDirector()->getWinSize();
    initLayers();
    createTile(layer1,4,4,1,true);
    createTile(layer2);
    createTile(layer3);
    showGuide();
    scheduleUpdate();
    return true;
}

void GameScene::initLayers()
{
    scrollLayer = CCLayer::create();
    layer1 = CCLayer::create();
    layer2 = CCLayer::create();
    layer3 = CCLayer::create();
    scrollLayer->setAnchorPoint(CCPointZero);
    layer1->setAnchorPoint(CCPointZero);
    layer2->setAnchorPoint(CCPointZero);
    layer3->setAnchorPoint(CCPointZero);
    layer1->setPosition(CCPointZero);
    layer2->setPosition(VisibleRect::leftTop());
    layer3->setPosition(VisibleRect::leftTop()*2);
    scrollLayer->addChild(layer1);
    scrollLayer->addChild(layer2);
    scrollLayer->addChild(layer3);
    addChild(scrollLayer);
}

void GameScene::createTile(cocos2d::CCLayer *layer,int horizontalTiles,int verticalTiles,int blackTiles,bool isstart)
{
    /* 每个块儿之间保留1像素的间隔 */
    float tileWidth = (m_winSize.width-horizontalTiles-1)/horizontalTiles;
    float tileHeight = (m_winSize.height-verticalTiles)/verticalTiles;
    for(int i=0;i<verticalTiles;i++)
    {
        int randomPos = rand()%horizontalTiles;
        for (int j=0; j<horizontalTiles; j++) {
            CCLayerColor *tile = CCLayerColor::create(cc4WHITE,tileWidth,tileHeight);
            tile->ignoreAnchorPointForPosition(false);
            tile->setAnchorPoint(CCPointZero);
            tile->setPosition(ccp(j*(tileWidth+1),i*(tileHeight+1)));
            
            
            if (isstart&&i==0) {
                /* 生成一排黄色的tile */
                tile->setColor(ccc3(246,192,85));
                layer->addChild(tile);
            }else if (isstart&&i==1&&j==randomPos){
                /* 添加开始文字 和触发开始游戏的逻辑 */
                BlackTile *bt = BlackTile::create(tileWidth,tileHeight);
                bt->setPosition(ccp(j*(tileWidth+1),i*(tileHeight+1)));
                bt->setTouchEnabled(true);
                CCLabelTTF *startLabel = CCLabelTTF::create("开始", "Arial", 100);
                startLabel->setPosition(ccp(tileWidth/2,tileHeight/2));
                bt->addChild(startLabel);
                bt->row = i;
                bt->col = j;
                layer->addChild(bt);
            }else if (j==randomPos){
                BlackTile *bt = BlackTile::create(tileWidth,tileHeight);
                bt->setPosition(ccp(j*(tileWidth+1),i*(tileHeight+1)));
                bt->setTouchEnabled(true);
                bt->row = i;
                bt->col = j;
                layer->addChild(bt);
            }else{
                layer->addChild(tile);
            }
            
        }
    }
    
}

void GameScene::showGuide()
{
    if(!GameConfig::guide)
    {
        return;
    }
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    BaseLayer *guideLayer = BaseLayer::create(GUIDE_LAYER_COLOR, winSize);
    
    CCLabelTTF *guideTitle = CCLabelTTF::create(GameConfig::guideTitle.c_str(), "Arial", 80);
    guideTitle->setColor(GUIDE_TITLE);
    guideTitle->setPosition(ccp(VisibleRect::center().x, VisibleRect::center().y+200));
    
    CCLabelTTF *guideContent = CCLabelTTF::create(GameConfig::guideContent.c_str(), "Arial", 60);
    guideContent->setColor(GUIDE_CONTENT);
    guideContent->setDimensions(CCSize(winSize.width,500));
    guideContent->setAnchorPoint(ccp(0.5f, 1.0f));
    guideContent->setPosition(guideTitle->getPosition()-ccp(0,50));
    
    guideLayer->addChild(guideTitle);
    guideLayer->addChild(guideContent);
    addChild(guideLayer);
}

void GameScene::update(float delta)
{
    float scrollY = scrollLayer->getPositionY() - 15;
    if(abs(scrollY)>=m_winSize.height)
    {
        CCArray *layers = scrollLayer->getChildren();
        CCLayer *newLayer = CCLayer::create();
        CCLayer *firstChild = (CCLayer*)layers->objectAtIndex(0);
        /* 检查是否有未点击的黑快儿 */
        CCArray *tiles = firstChild->getChildren();
        CCObject *tile = NULL;
        CCARRAY_FOREACH(tiles, tile)
        {
            BlackTile *bt = dynamic_cast<BlackTile*>(tile);
            if(bt&&!bt->m_bIsTouched)
            {
                CCLog("untouch row:%d,col:%d",bt->row,bt->col);
                unscheduleUpdate();
                CCMessageBox("GameOver", "Warn");
                return;
            }
        }
        
        
        scrollLayer->removeChild(firstChild);
        scrollLayer->addChild(newLayer);
        layers = scrollLayer->getChildren();
        CCObject *obj = NULL;
        int idx = 0;
        CCARRAY_FOREACH(layers, obj){
            CCLayer *layer = (CCLayer*)obj;
            layer->setPosition(VisibleRect::leftTop()*idx);
            idx++;
        }
        createTile(newLayer);
        scrollY += m_winSize.height;
    }
    scrollLayer->setPositionY(scrollY);
}

