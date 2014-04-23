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
#include "HomeScene.h"

CCScene *GameScene::scene()
{
    CCScene *scene = CCScene::create();
    scene->addChild(GameScene::create());
    return scene;
}

void GameScene::onEnter()
{
    CCLayer::onEnter();
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameScene::__endGame), "END_GAME", NULL);
}

void GameScene::onExit()
{
    CCLayer::onExit();
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "END_GAME");
}

void GameScene::__endGame(CCObject *obj)
{
    unscheduleUpdate();
    removeAllChildren();
    __showResult();
    screens = 0;
    GameConfig::score = 0;
    GameConfig::blackTiles->removeAllObjects();
    GameConfig::blackTiles->release();
    GameConfig::tileCount = 0;
}

bool GameScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    score = 0;
    scoreLabel = CCLabelTTF::create("00", "fonts/SourceSansPro-Bold.ttf", 50);
    scoreLabel->setColor(ccc3(255, 0, 0));
    scoreLabel->setPosition(VisibleRect::top()-ccp(0,50));
    scoreLabelShadow = CCLabelTTF::create("00", "fonts/SourceSansPro-Bold.ttf", 50);
    scoreLabelShadow->setColor(ccc3(105,53,52));
    scoreLabelShadow->setOpacity(128);
    scoreLabelShadow->setPosition(VisibleRect::top()-ccp(-2, 52));
    
    GameConfig::blackTiles = CCArray::create();
    GameConfig::blackTiles->retain();
    m_winSize = CCDirector::sharedDirector()->getWinSize();
    initLayers();
    createTile(layer1,4,4,1,true);
    createTile(layer2);
    showGuide();
    
    addChild(scoreLabelShadow);
    addChild(scoreLabel);
    return true;
}

void GameScene::initLayers()
{
    scrollLayer = CCLayer::create();
    layer1 = CCLayer::create();
    layer2 = CCLayer::create();
    scrollLayer->setAnchorPoint(CCPointZero);
    layer1->setAnchorPoint(CCPointZero);
    layer2->setAnchorPoint(CCPointZero);
    layer1->setPosition(CCPointZero);
    layer2->setPosition(VisibleRect::leftTop());
    layer1->setTag(1);
    layer2->setTag(2);
    scrollLayer->addChild(layer1);
    scrollLayer->addChild(layer2);
    addChild(scrollLayer);
}

void GameScene::createTile(cocos2d::CCLayer *layer,int horizontalTiles,int verticalTiles,int blackTiles,bool isstart)
{
    CCSpriteBatchNode *pTileBatch = CCSpriteBatchNode::create("whiteBlock.png");
    layer->addChild(pTileBatch);
    /* 每个块儿之间保留1像素的间隔 */
    float tileWidth = (m_winSize.width-(horizontalTiles-1)/2)/horizontalTiles;
    float tileHeight = (m_winSize.height-verticalTiles/2)/verticalTiles;
    for(int i=0;i<verticalTiles;i++)
    {
        int randomPos = rand()%horizontalTiles;
        for (int j=0; j<horizontalTiles; j++) {
            CCSprite *tile = CCSprite::create("whiteBlock.png");
            tile->setAnchorPoint(CCPointZero);
            CCSize tileSize = tile->getContentSize();
            tile->cocos2d::CCNode::setScale(tileWidth/tileSize.width, tileHeight/tileSize.height);
            tile->setPosition(ccp(j*(tileWidth+1),i*(tileHeight+1)));
            if (isstart&&i==0) {
                /* 生成一排黄色的tile */
                pTileBatch->addChild(tile);
            }else if (isstart&&i==1&&j==randomPos){
                /* 添加开始文字 和触发开始游戏的逻辑 */
                BlackTile *bt = BlackTile::create(tileWidth,tileHeight);
                bt->setPosition(ccp(j*(tileWidth+1),i*(tileHeight+1)));
                bt->setTouchEnabled(true);
                CCLabelTTF *startLabel = CCLabelTTF::create("开始", "Arial", 40);
                startLabel->setPosition(ccp(tileWidth/2,tileHeight/2));
                bt->addChild(startLabel);
                bt->row = i;
                bt->col = j;
                bt->setTarget(this, menu_selector(GameScene::startGame));
                layer->addChild(bt);
            }else if (j==randomPos){
                BlackTile *bt = BlackTile::create(tileWidth,tileHeight);
                bt->setPosition(ccp(j*(tileWidth+1),i*(tileHeight+1)));
                bt->setTouchEnabled(true);
                bt->row = i;
                bt->col = j;
                layer->addChild(bt);
            }else{
                pTileBatch->addChild(tile);
            }
            
        }
    }
    
}

void GameScene::startGame(cocos2d::CCObject *pSender)
{
    screens = 0;
    scheduleUpdate();
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
    char scoreStr[10];
    sprintf(scoreStr, "%02d",GameConfig::score);
    scoreLabel->setString(scoreStr);
    scoreLabelShadow->setString(scoreStr);
    
    int offset = GameConfig::score/60;
    
    if(offset>5){
        offset = 5;
    }
    int speed = 13 + offset*1;
    float scrollY = scrollLayer->getPositionY() - speed;
    float passScreens = abs(scrollY)/m_winSize.height;
    int intScreen = passScreens;
    if(intScreen>screens&&passScreens>(screens+0.5))
    {
        CCLayer *layer = CCLayer::create();
        screens = intScreen;
        createTile(layer);
        layer->setAnchorPoint(CCPointZero);
        layer->setPosition(VisibleRect::leftTop()*(screens+1));
        layer->setTag(screens+2);
        scrollLayer->addChild(layer);
        scrollLayer->removeChildByTag(screens, true);
        if(screens%5==0)
        {
            scrollY += screens*m_winSize.height;
            CCArray *layers = scrollLayer->getChildren();
            CCObject *obj = NULL;
            int tag = 1;
            screens = 0;
            CCARRAY_FOREACH(layers, obj)
            {
                layer = (CCLayer*)obj;
                layer->setTag(tag);
                layer->setPosition(VisibleRect::leftTop()*(tag-1));
                tag++;
            }
        }
    }
    
    scrollLayer->setPositionY(scrollY);
}

void GameScene::__showResult()
{
    removeChild(scrollLayer);
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
