//
//  GameScene.cpp
//  WhiteTile
//
//  Created by AIRIA on 14-4-21.
//
//

#include "GameScene.h"
#include "GameConfig.h"
#include "VisibleRect.h"
#include "HomeScene.h"
#include "Tile.h"

using namespace WT;

CCScene *GameScene::scene()
{
    CCScene *scene = CCScene::create();
    scene->addChild(GameScene::create());
    return scene;
}


void GameScene::onEnter()
{
    CCLayer::onEnter();
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameScene::__endGame), WT_GAME_OVER, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameScene::__updateScore), WT_UPDATE_SCORE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameScene::__updateScrollLayerPosition), WT_UPDATE_SCROLLER_POS, NULL);
}

void GameScene::onExit()
{
    CCLayer::onExit();
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, WT_GAME_OVER);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, WT_UPDATE_SCORE);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, WT_UPDATE_SCROLLER_POS);
}

void GameScene::__updateScrollLayerPosition(cocos2d::CCObject *obj)
{
    int createdScreen = (GameConfig::tileCount+1)/4;
    CCLog("createScreen:%d,tileCount:%d",createdScreen,GameConfig::tileCount+1);
    CCLayer *appendLayer = CCLayer::create();
//    createTile(appendLayer);
    appendLayer->setTag(createdScreen+1);
    appendLayer->setPosition(VisibleRect::leftTop()*createdScreen);
    GameConfig::scroller->addChild(appendLayer);
    GameConfig::scroller->removeChildByTag(GameConfig::passScreens);
    
}

void GameScene::__updateScore(cocos2d::CCObject *obj)
{
    char scoreStr[10];
    sprintf(scoreStr, "%02d",GameConfig::score);
    scoreLabel->setString(scoreStr);
    scoreLabelShadow->setString(scoreStr);
}

void GameScene::__endGame(CCObject *obj)
{
    unscheduleUpdate();
    removeAllChildren();
    __showResult();
    screens = 0;
    GameConfig::score = 0;
    GameConfig::speed = WT_INIT_SPEED;
    GameConfig::blackTiles->removeAllObjects();
    GameConfig::blackTiles->release();
    GameConfig::tileCount = 0;
    GameConfig::passScreens = 0;
}

bool GameScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    score = 0;
    GameConfig::blackTiles = CCArray::create();
    GameConfig::blackTiles->retain();
    tilePools = CCArray::create();
    m_winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite *tmpSprite = CCSprite::create("whiteBlock.png");
    CCSize tileSize = tmpSprite->getContentSize();
    /* 初始化tile的宽高 */
    m_nHorizontalTiles = m_nVerticalTiles = 4;
    m_fTileWidth = (m_winSize.width-(m_nHorizontalTiles-1)/2)/m_nHorizontalTiles;
    m_fTileHeight = (m_winSize.height-m_nVerticalTiles/2)/m_nVerticalTiles;
    m_fTileScaleX = m_fTileWidth/tileSize.width;
    m_fTileScaleY = m_fTileHeight/tileSize.height;
    initLayers();
    showGuide();
    /* score label */
    scoreLabel = CCLabelTTF::create("00", "fonts/SourceSansPro-Bold.ttf", 50);
    scoreLabel->setColor(ccc3(255, 0, 0));
    scoreLabel->setPosition(VisibleRect::top()-ccp(0,50));
    scoreLabelShadow = CCLabelTTF::create("00", "fonts/SourceSansPro-Bold.ttf", 50);
    scoreLabelShadow->setColor(ccc3(105,53,52));
    scoreLabelShadow->setOpacity(128);
    scoreLabelShadow->setPosition(VisibleRect::top()-ccp(-2, 52));
    addChild(scoreLabelShadow);
    addChild(scoreLabel);
    createTile(3);
    return true;
}

void GameScene::initLayers()
{
    tileBatchNode = CCSpriteBatchNode::create("whiteBlock.png");
    GameConfig::scroller = CCNode::create();
    GameConfig::scroller->addChild(tileBatchNode);
    addChild(GameConfig::scroller);
}

void GameScene::createTile(int rows)
{
    /* 从对象池中获取指针 */
    for (int i=0; i<rows; i++) {
        int randomPos = rand()%m_nHorizontalTiles;
        for (int j=0; j<m_nVerticalTiles; j++) {
            WT::Tile *tile = NULL;
            CCObject *obj = NULL;
            CCARRAY_FOREACH(tilePools, obj)
            {
                tile = (WT::Tile*)obj;
                if(tile->isRendering)
                {
                    tile = NULL;
                    continue;
                }
                break;
            }
            if(tile==NULL){
                tile = WT::Tile::create();
                tile->isRendering = true;
                tile->setAnchorPoint(CCPointZero);
                tile->cocos2d::CCNode::setScale(m_fTileScaleX, m_fTileScaleY);
                tileBatchNode->addChild(tile);
                tilePools->addObject(tile);
            }
            tile->setPosition(ccp(j*(m_fTileWidth+1),i*(m_fTileHeight+1)));
            tile->setTouchEnabled(false);
            tile->setOpacity(255);
            tile->row = i;
            tile->col = j;
            if (randomPos==j) {
                tile->setOpacity(0);
                if(i==0){
                    tile->setTouchEnabled(true);
                    tile->setTargetEnded(this, menu_selector(GameScene::startGame));
                }
            }
        }
    }
}

void GameScene::startGame(cocos2d::CCObject *pSender)
{
    screens = 0;
    scheduleUpdate();
    CCLog("start game");
}

void GameScene::showGuide()
{
    if(!GameConfig::guide)
    {
        return;
    }
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    BaseLayer *guideLayer = BaseLayer::create(GUIDE_LAYER_COLOR, winSize);
    
    CCLabelTTF *guideTitle = CCLabelTTF::create(GameConfig::guideTitle.c_str(), "Arial", 30);
    guideTitle->setColor(GUIDE_TITLE);
    guideTitle->setPosition(ccp(VisibleRect::center().x, VisibleRect::center().y+200));
    
    CCLabelTTF *guideContent = CCLabelTTF::create(GameConfig::guideContent.c_str(), "Arial", 20);
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
    float scrollY = GameConfig::scroller->getPositionY() - GameConfig::speed;
    GameConfig::scroller->setPositionY(scrollY);
}

void GameScene::__showResult()
{
    removeChild(GameConfig::scroller);
    CCLayerColor *resultLayer = CCLayerColor::create(ccc4(227,48,56, 255),m_winSize.width,m_winSize.height);
    resultLayer->ignoreAnchorPointForPosition(false);
    resultLayer->setAnchorPoint(CCPointZero);
    resultLayer->setPosition(CCPointZero);
    addChild(resultLayer);
    CCLabelTTF *title = CCLabelTTF::create("街机模式", "Arial", 50);
    title->setColor(ccWHITE);
    title->setPosition(VisibleRect::top()-ccp(0,150));
    resultLayer->addChild(title);
    
    char scoreStr[20];
    sprintf(scoreStr, "%d",GameConfig::score);
    CCLabelTTF *score = CCLabelTTF::create(scoreStr, "fonts/SourceSansPro-Bold.ttf",150);
    score->setPosition(title->getPosition()-ccp(0, 180));
    score->setColor(ccBLACK);
    resultLayer->addChild(score);
    
    sprintf(scoreStr, "最佳成绩:%d",GameConfig::score);
    CCLabelTTF *best = CCLabelTTF::create(scoreStr, "Arial", 30);
    best->setPosition(score->getPosition()-ccp(0,100));
    best->setColor(ccBLACK);
    resultLayer->addChild(best);
    CCMenuItemFont *share = CCMenuItemFont::create("炫耀",this,NULL);

    CCMenuItemFont *quit = CCMenuItemFont::create("返回", this, menu_selector(GameScene::__backHandler));
    CCMenuItemFont *retry = CCMenuItemFont::create("重来",this, menu_selector(GameScene::__retryHandler));
    share->setFontSize(40);
    quit->setFontSize(40);
    retry->setFontSize(40);
    CCMenu *menu = CCMenu::create(share,quit,retry,NULL);
    menu->setPositionY(200);
    menu->alignItemsHorizontallyWithPadding(50);
    resultLayer->addChild(menu);
}

void GameScene::__backHandler(cocos2d::CCObject *pSender)
{
    CCDirector::sharedDirector()->replaceScene(HomeScene::scene());
}

void GameScene::__retryHandler(cocos2d::CCObject *pSender)
{
    removeAllChildren();
    init();
}
