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
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "ShareLayer.h"
#include "AdUtil.h"

#define WT_BLACK_TILE_SCALE 0.8f
#define WT_TILES_COUNT 30
#define WT_TILES_BUFFER 10
#define WT_MAX_SPEED 20
#define WT_MIN_SPEED 14

using namespace WT;
using namespace cocos2d::extension;
using namespace CocosDenshion;

CCScene *GameScene::scene(GameType gameType)
{
    AdUtil::init();
    CCScene *scene = CCScene::create();
    GameConfig::gameType = gameType;
    GameScene *game = GameScene::create();
    scene->addChild(game);
    return scene;
}

void GameScene::__initSelectMode()
{
    gameMode = GameConfig::gameType;
    m_fClassicTime = 0.0f;
    m_fChanTime = 30.000f;
    switch (gameMode) {
        case kAracade:
            resultTitle = "街机模式";
            topScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("aracade",0);
            break;
        case kClassic:
            resultTitle = "经典模式";
            scoreLabel->setString("0.000\"");
            topScore = CCUserDefault::sharedUserDefault()->getFloatForKey("classic",0.000);
            break;
        case kChan:
            resultTitle = "禅模式";
            scoreLabel->setString("30.000\"");
            topScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("chan",0);
            break;
        default:
            break;
    }
    CCLog("select mode:%s",resultTitle.c_str());
}

void GameScene::__saveScore()
{
    switch (gameMode) {
        case kAracade:
            CCUserDefault::sharedUserDefault()->setIntegerForKey("aracade",topScore);
            break;
        case kClassic:
            CCUserDefault::sharedUserDefault()->setFloatForKey("classic",topScore);
            break;
        case kChan:
            CCUserDefault::sharedUserDefault()->setIntegerForKey("chan",topScore);
            break;
        default:
            break;
    }
    CCUserDefault::sharedUserDefault()->flush();
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
    
}

/* 初始化游戏需要的属性 包括黑块儿的大小 */
bool GameScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    AdUtil::showAds();
    isGameover = false;
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
    __initSelectMode();
    if(gameMode==kClassic)
    {
        createTile(50);
        __createGreenResultLayer(); 
    }else{
        createTile(WT_TILES_COUNT);
    }
    
    return true;
}

void GameScene::__createGreenResultLayer()
{
    CCLayerColor *resultLayer = CCLayerColor::create(ccc4(73,178,87, 255),m_winSize.width,m_winSize.height);
    resultLayer->ignoreAnchorPointForPosition(false);
    resultLayer->setAnchorPoint(CCPointZero);
//    resultLayer->setPosition(CCPointZero);
    resultLayer->setPosition(ccp(0.0,(m_winSize.height+2)/4*GameConfig::tileCount));
    GameConfig::scroller->addChild(resultLayer);
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
                tile->setColor(ccWHITE);
                tile->isRendering = true;
                tile->cocos2d::CCNode::setScale(m_fTileScaleX, m_fTileScaleY);
                break;
            }
            if(tile==NULL){
                tile = WT::Tile::create();
                tile->isRendering = true;
                tile->setTouchEnabled(true);
                tile->cocos2d::CCNode::setScale(m_fTileScaleX, m_fTileScaleY);
                tileBatchNode->addChild(tile);
                tilePools->addObject(tile);
                tile->setTargetEnded(this, menu_selector(GameScene::__tileTouchUpHandler));
                tile->setTargetBegan(this, menu_selector(GameScene::__whiteTileTouchHandler));
            }
            tile->row = row;
            tile->col = j;
            tile->setPosition(ccp((j+0.5)*(m_fTileWidth+1),(row+0.5)*(m_fTileHeight+1)));
//            tile->setTouchEnabled(false);
            tile->setTag(tileTag++);
            tile->setTouchPriority(10);
            if (randomPos==j&&row>0) {
                tile->setColor(ccBLACK);
                GameConfig::tileCount++;
                activeTiles++;
                m_vBlackTags.push_back(tile->getTag());
                tile->setTouchPriority(0);
                tile->setTargetBegan(this, menu_selector(GameScene::__tileTouchDownHandler));
                tile->cocos2d::CCNode::setScale(m_fTileScaleX*WT_BLACK_TILE_SCALE, m_fTileScaleY*WT_BLACK_TILE_SCALE);
            }else if(randomPos==j&&row==0){
                GameConfig::tileCount++;
            }
            if(row==0){
                tile->setColor(ccYELLOW);
                tile->setTouchEnabled(false);
                tile->cocos2d::CCNode::setScale(m_fTileScaleX, m_fTileScaleY);
            }
        }
    }
    CCLog("tilecount:%d",GameConfig::tileCount);
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
    CCLabelTTF *title = CCLabelTTF::create(resultTitle.c_str(), "Arial", 50);
    title->setColor(ccWHITE);
    title->setPosition(VisibleRect::top()-ccp(0,150));
    resultLayer->addChild(title);
    
    char scoreStr[20],recordStr[20];
    int scoreSize = 150;
    if (gameMode==kClassic) {
        if(GameConfig::score<GameConfig::tileCount-1){
            sprintf(scoreStr, "败了!");
            sprintf(recordStr, " ");
            scoreSize = 200;
        }else{
            scoreSize = 80;
            resultLayer->setColor(ccc3(73,178,87));
            sprintf(scoreStr, "%1.3f\"",m_fClassicTime);
            
            if (topScore > m_fClassicTime) {
                sprintf(recordStr, "新纪录");
                topScore = m_fClassicTime;
            }else if(topScore>0){
                sprintf(recordStr, "最佳 %1.3f\"",topScore);
            }else{
                sprintf(recordStr, "暂无历史成绩");
                topScore = m_fClassicTime;
            }
        }
        
    }else{
        sprintf(scoreStr, "%d",GameConfig::score);
        if(GameConfig::score>topScore){
            sprintf(recordStr, "新纪录");
            topScore = GameConfig::score;
        }else if(topScore>0){
            sprintf(recordStr, "最佳 %1.0f",topScore);
        }else{
            sprintf(recordStr, "暂无历史成绩");
            topScore = GameConfig::score;
        }
    }
    CCLabelTTF *score = CCLabelTTF::create(scoreStr, "fonts/SourceSansPro-Bold.ttf",scoreSize);
    score->setPosition(title->getPosition()-ccp(0, 180));
    score->setColor(ccBLACK);
    resultLayer->addChild(score);
    
    CCLabelTTF *best = CCLabelTTF::create(recordStr, "Arial", 30);
    best->setPosition(score->getPosition()-ccp(0,100));
    best->setColor(ccBLACK);
    resultLayer->addChild(best);
    
    CCLabelTTF *des = CCLabelTTF::create("牛逼闪闪哦~", "Arial", 50);
    des->setColor(ccWHITE);
    des->setPosition(best->getPosition()-ccp(0,90));
//    resultLayer->addChild(des);
    
    CCMenuItemFont *share = CCMenuItemFont::create("炫耀",this,menu_selector(GameScene::__shareHandler));

    CCMenuItemFont *quit = CCMenuItemFont::create("返回", this, menu_selector(GameScene::__backHandler));
    CCMenuItemFont *retry = CCMenuItemFont::create("重来",this, menu_selector(GameScene::__retryHandler));
    share->setFontSize(40);
    quit->setFontSize(40);
    retry->setFontSize(40);
    CCMenu *menu = CCMenu::create(share,quit,retry,NULL);
    menu->setPositionY(200);
    menu->alignItemsHorizontallyWithPadding(50);
    resultLayer->addChild(menu);
    __saveScore();
    GameConfig::score = 0;
    GameConfig::speed = WT_INIT_SPEED;
    GameConfig::tileCount = 0;
    GameConfig::passScreens = 0;
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
    if (isGameover) {
        return;
    }
    WT::Tile *tile = (WT::Tile*)pSender;
    tile->setColor(ccGRAY);
    tile->unscheduleUpdate();
    m_vBlackTags.erase(m_vBlackTags.begin());
    WT::Tile *nextBlackTile = (WT::Tile*)tile->getParent()->getChildByTag(*m_vBlackTags.begin());
    nextBlackTile->scheduleUpdate();
    nextBlackTile->setTouchEnabled(true);

    if(gameMode!=kAracade)
    {
        CCActionInterval *move = NULL;
        if (gameMode==kClassic&&GameConfig::score==GameConfig::tileCount-2) {
            move = CCMoveBy::create(0.1f, ccp(0,(-m_winSize.height/4))*2);
            CCCallFunc *moveHandler = CCCallFunc::create(this, callfunc_selector(GameScene::__showResult));
            GameConfig::scroller->runAction(CCSequence::create(move,moveHandler,NULL));
            unschedule(schedule_selector(GameScene::__timerHandler));
        }else{
            move = CCMoveBy::create(0.1f, ccp(0,(-m_winSize.height/4-0.5)));
            GameConfig::scroller->runAction(move);
        }
        
    }
    
}

void GameScene::__tileTouchUpHandler(cocos2d::CCObject *pSender)
{
    WT::Tile *tile = (WT::Tile*)pSender;
    tile->setTouchEnabled(false);
    tile->runAction(CCScaleTo::create(0.15f, m_fTileScaleX,m_fTileScaleY));
    ++GameConfig::score;
    activeTiles--;
    if(gameMode==kAracade)
    {
        char scoreStr[10];
        sprintf(scoreStr, "%02d",GameConfig::score);
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
    }
    
    /* 判断是不是应该回收资源 */
    if (activeTiles==WT_TILES_BUFFER&&(gameMode==kAracade||gameMode==kChan)) {
        int index = tile->row;
        WT::Tile *inactiveTile = NULL;
        for (int i=(index-(WT_TILES_COUNT-WT_TILES_BUFFER))*m_nHorizontalTiles; i<(index-WT_TILES_BUFFER)*m_nHorizontalTiles; i++) {
            //            CCLog("remove tag:%d,row:%d",i,index);
            inactiveTile = (WT::Tile*)tileBatchNode->getChildByTag(i+1);
            inactiveTile->isRendering = false;
        }
        createTile(WT_TILES_BUFFER);
    }
   
    if (tile->row==1) {
        switch (gameMode) {
            case kClassic:
                schedule(schedule_selector(GameScene::__timerHandler));
                if (GameConfig::score==50) {
                    unschedule(schedule_selector(GameScene::__timerHandler));
                }
                break;
            case kAracade:
                scheduleUpdate();
                break;
            case kChan:
                schedule(schedule_selector(GameScene::__timerHandler));
                break;
            default:
                break;
        }
        
    }
}
/* GameOver */
void GameScene::__whiteTileTouchHandler(cocos2d::CCObject *pSender)
{
    if(isGameover)
        return;
    SimpleAudioEngine::sharedEngine()->playEffect("error.m4a");
    isGameover = true;
    unscheduleUpdate();
    unschedule(schedule_selector(GameScene::__timerHandler));
    WT::Tile *tile = (WT::Tile*)pSender;
    tile->setTouchEnabled(false);
    tile->setColor(ccRED);
    CCActionInterval *fadeOut = CCFadeOut::create(0.15f);
    CCActionInterval *fadeIn = CCFadeOut::create(0.15f);
    CCActionInterval *fadeSeq = CCSequence::create(fadeOut,fadeIn,NULL);
    CCActionInterval *blink = CCRepeat::create(fadeSeq, 3);
    CCActionInstant *blinkHandler = CCCallFunc::create(this, callfunc_selector(GameScene::__blinkHandler));
    tile->runAction(CCSequence::create(blink,blinkHandler,NULL));
}

/*-------------------------------------------------------------------------------------*/

void GameScene::__timerHandler(float del)
{
    char timeStr[20];
    switch (gameMode) {
        case kClassic:
            m_fClassicTime += del;
            sprintf(timeStr, "%1.3f\"",m_fClassicTime);
            break;
        case kChan:
            m_fChanTime -= del;
            if (m_fChanTime<=0) {
                m_fChanTime = 0.000f;
                unschedule(schedule_selector(GameScene::__timerHandler));
                sprintf(timeStr, "时间到了");
                isGameover = true;
                CCActionInterval *fadeOut = CCFadeOut::create(0.15f);
                CCActionInterval *fadeIn = CCFadeOut::create(0.15f);
                CCActionInterval *fadeSeq = CCSequence::create(fadeOut,fadeIn,NULL);
                CCActionInterval *blink = CCRepeat::create(fadeSeq, 3);
                CCActionInstant *blinkHandler = CCCallFunc::create(this, callfunc_selector(GameScene::__blinkHandler));
                scoreLabel->runAction(CCSequence::create(blink,blinkHandler,NULL));
            }else{
                sprintf(timeStr, "%1.3f\"",m_fChanTime);
            }
            
            
            break;
        default:
            break;
    }
    scoreLabel->setString(timeStr);
    scoreLabelShadow->setString(timeStr);
}
/*        share        */

void GameScene::__shareHandler(cocos2d::CCObject *pSender)
{
    AdUtil::hideAds();
    ShareLayer *sl = ShareLayer::create();
    addChild(sl);
}
