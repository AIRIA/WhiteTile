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
#define WT_BLACK_TILE_SCALE 0.8f
#define WT_TILES_COUNT 200
#define WT_TILES_BUFFER 10
#define WT_MAX_SPEED 16
#define WT_MIN_SPEED 10

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
}

void GameScene::onExit()
{
    CCLayer::onExit();
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, WT_GAME_OVER);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, WT_UPDATE_SCORE);
}

/* 触摸黑块儿的时候 更新得分 */
void GameScene::__updateScore(cocos2d::CCObject *obj)
{
    char scoreStr[10];
    sprintf(scoreStr, "%02d",GameConfig::score);
    scoreLabel->setString(scoreStr);
    scoreLabelShadow->setString(scoreStr);
}
/* 游戏结束 移除所有的元素 重置全局变量 */
void GameScene::__endGame(CCObject *obj)
{
    unscheduleUpdate();
    WT::Tile *tile = (WT::Tile*)obj;
    int row = tile->row;
    tile->setScaleX(m_fTileScaleX);
    tile->setScaleY(m_fTileScaleY);
    CCActionInterval *rollBack = CCMoveTo::create(0.2f, ccp(0,(-row+1)*(m_fTileHeight+1)));
    CCCallFuncND *rollBackFunc = CCCallFuncND::create(this, callfuncND_selector(GameScene::__rollBackHandler),tile);
    GameConfig::scroller->runAction(CCSequence::create(rollBack,rollBackFunc,NULL));
    return;
}

void GameScene::__rollBackHandler(cocos2d::CCObject *pSender,void *param)
{
     WT::Tile *tile = (WT::Tile*)param;
    tile->setColor(ccGRAY);
    tile->setOpacity(255);
    CCActionInterval *fadeOut = CCFadeOut::create(0.15f);
    CCActionInterval *fadeIn = CCFadeOut::create(0.15f);
    CCActionInterval *fadeSeq = CCSequence::create(fadeOut,fadeIn,NULL);
    CCActionInterval *blink = CCRepeat::create(fadeSeq, 3);
    CCActionInstant *blinkHandler = CCCallFunc::create(this, callfunc_selector(GameScene::__blinkHandler));
    tile->runAction(CCSequence::create(blink,blinkHandler,NULL));
}

void GameScene::__blinkHandler()
{
    removeAllChildren();
    __showResult();
    GameConfig::score = 0;
    GameConfig::speed = WT_INIT_SPEED;
    GameConfig::tileCount = 0;
    GameConfig::passScreens = 0;
}

/* 初始化游戏需要的属性 包括黑块儿的大小 */
bool GameScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    tileCount = WT_TILES_COUNT;
    activeTiles = 1;
    tileTag = 1;
    GameConfig::speed = WT_MIN_SPEED;
    m_vBlackTags.clear();
    tilePools = CCArray::createWithCapacity(WT_TILES_COUNT);
    tilePools->retain();
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
    createTile(WT_TILES_COUNT);
    return true;
}

void GameScene::initLayers()
{
    tileBatchNode = CCSpriteBatchNode::create("whiteBlock.png");
    GameConfig::scroller = CCNode::create();
    tileBatchNode->setAnchorPoint(CCPointZero);
    GameConfig::scroller->addChild(tileBatchNode);
    addChild(GameConfig::scroller);
}

/* 根据行数创建指定数量和位置的瓦块儿 */
void GameScene::createTile(int rows)
{
    WT::Tile *tile = NULL;
    CCObject *obj = NULL;
    /* 从对象池中获取指针 */
    for (int i=0; i<rows; i++) {
        int randomPos = rand()%m_nHorizontalTiles;
        int row = GameConfig::tileCount;
        for (int j=0; j<m_nVerticalTiles; j++) {
            CCARRAY_FOREACH(tilePools, obj)
            {
                tile = (WT::Tile*)obj;
                if(tile->isRendering)
                {
                    tile = NULL;
                    continue;
                }
//                CCLog("缓存池中获取数据");
                tile->setColor(ccWHITE);
                tile->isRendering = true;
                tile->cocos2d::CCNode::setScale(m_fTileScaleX, m_fTileScaleY);
                break;
            }
            if(tile==NULL){
                tile = WT::Tile::create();
                tile->isRendering = true;
                tile->cocos2d::CCNode::setScale(m_fTileScaleX, m_fTileScaleY);
                tileBatchNode->addChild(tile);
                tilePools->addObject(tile);
                tile->setTargetEnded(this, menu_selector(GameScene::__tileTouchUpHandler));
                tile->setTargetBegan(this, menu_selector(GameScene::__tileTouchDownHandler));
            }
            tile->row = row;
            tile->col = j;
            tile->setPosition(ccp((j+0.5)*(m_fTileWidth+1),(row+0.5)*(m_fTileHeight+1)));
            tile->setTouchEnabled(false);
            tile->setTag(tileTag++);
            if (randomPos==j) {
                tile->setColor(ccBLACK);
                GameConfig::tileCount++;
                activeTiles++;
                m_vBlackTags.push_back(tile->getTag());
                tile->setTouchEnabled(true);
                tile->cocos2d::CCNode::setScale(m_fTileScaleX*WT_BLACK_TILE_SCALE, m_fTileScaleY*WT_BLACK_TILE_SCALE);
            }
        }
    }
}

void GameScene::startGame(cocos2d::CCObject *pSender)
{
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

void GameScene::__tileTouchDownHandler(cocos2d::CCObject *pSender)
{
    WT::Tile *tile = (WT::Tile*)pSender;
    tile->setColor(ccGRAY);
    tile->unscheduleUpdate();
    m_vBlackTags.erase(m_vBlackTags.begin());
    WT::Tile *nextBlackTile = (WT::Tile*)tile->getParent()->getChildByTag(*m_vBlackTags.begin());
    nextBlackTile->scheduleUpdate();
}

void GameScene::__tileTouchUpHandler(cocos2d::CCObject *pSender)
{
    WT::Tile *tile = (WT::Tile*)pSender;
    tile->setTouchEnabled(false);
    activeTiles--;
    tile->runAction(CCScaleTo::create(0.15f, m_fTileScaleX,m_fTileScaleY));
    char scoreStr[10];
    sprintf(scoreStr, "%02d",++GameConfig::score);
    scoreLabel->setString(scoreStr);
    scoreLabelShadow->setString(scoreStr);
    /* 判断是否需要加速 */
    if(GameConfig::score%30==0)
    {
        GameConfig::speed += 2;
        if(GameConfig::speed>WT_MAX_SPEED){
            GameConfig::speed = WT_MAX_SPEED;
        }
    }
    /* 判断是不是应该回收资源 */
    if (activeTiles==WT_TILES_BUFFER) {
        int index = tile->row;
        WT::Tile *inactiveTile = NULL;
        for (int i=(index-(WT_TILES_COUNT-WT_TILES_BUFFER))*m_nHorizontalTiles; i<(index-WT_TILES_BUFFER)*m_nHorizontalTiles; i++) {
//            CCLog("remove tag:%d,row:%d",i,index);
            inactiveTile = (WT::Tile*)tileBatchNode->getChildByTag(i+1);
            inactiveTile->isRendering = false;
        }
        createTile(WT_TILES_BUFFER);
    }
    if (tile->row==0) {
        scheduleUpdate();
    }
}
/* GameOver */
void GameScene::__whiteTileTouchHandler(cocos2d::CCObject *pSender)
{
    
}
