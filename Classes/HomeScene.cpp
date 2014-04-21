//
//  HomeScene.cpp
//  WhiteTile
//
//  Created by AIRIA on 14-4-21.
//
//

#include "HomeScene.h"
#include "VisibleRect.h"
#include "GameConfig.h"
#include "GameScene.h"

#define MENU_FONT_SIZE 45
#define SUB_MENU_FONT_SIZE 28

static const ccColor4B cc4BLACK={0,0,0.255};
static const ccColor4B cc4WHITE={255,255,255,255};

CCScene *HomeScene::scene()
{
    CCScene *homeScene = CCScene::create();
    homeScene->addChild(HomeScene::create());
    return homeScene;
}

bool HomeScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    BaseLayer *leftTop = __getMenuLayerItem(cc4WHITE, ccBLACK, CCSize(winSize.width/2,winSize.height/2), ccp(0.0f, 1.0f), VisibleRect::leftTop(), "经典", MENU_FONT_SIZE);
    leftTop->setTarget(this, menu_selector(HomeScene::__classicHandler));
    
    BaseLayer *rightTop = __getMenuLayerItem(cc4BLACK, ccWHITE, CCSize(winSize.width/2,winSize.height/2), ccp(1.0f, 1.0f), VisibleRect::rightTop(), "街机", MENU_FONT_SIZE);
    rightTop->setTarget(this, NULL);
    
    BaseLayer *leftBottom = __getMenuLayerItem(cc4BLACK, ccWHITE, CCSize(winSize.width/2,winSize.height/2), CCPointZero, CCPointZero, "禅", MENU_FONT_SIZE);
    leftBottom->setTarget(this, NULL);
    
    BaseLayer *soundSetting = __getMenuLayerItem(cc4WHITE, ccBLACK, CCSize(winSize.width/2,winSize.height/10), ccp(0.0f, 1.0f), VisibleRect::center(), "音效:默认", SUB_MENU_FONT_SIZE);
    soundSetting->setTarget(this, NULL);
    BaseLayer *learn = __getMenuLayerItem(cc4BLACK, ccWHITE, CCSize(winSize.width/2,winSize.height/10), ccp(0.0f, 1.0f), VisibleRect::center()-ccp(0,winSize.height/10), "引导:开", SUB_MENU_FONT_SIZE);
    learn->setTarget(this, NULL);
    BaseLayer *rank = __getMenuLayerItem(cc4WHITE, ccBLACK, CCSize(winSize.width/2,winSize.height/10), ccp(0.0f, 1.0f), VisibleRect::center()-ccp(0,winSize.height/10)*2, "排行榜", SUB_MENU_FONT_SIZE);
    rank->setTarget(this, NULL);
    BaseLayer *mark = __getMenuLayerItem(cc4BLACK, ccWHITE, CCSize(winSize.width/2,winSize.height/10), ccp(0.0f, 1.0f), VisibleRect::center()-ccp(0,winSize.height/10)*3, "评分", SUB_MENU_FONT_SIZE);
    mark->setTarget(this, NULL);
    BaseLayer *more = __getMenuLayerItem(cc4WHITE, ccBLACK, CCSize(winSize.width/2,winSize.height/10), ccp(0.0f, 1.0f), VisibleRect::center()-ccp(0,winSize.height/10)*4, "更多游戏", SUB_MENU_FONT_SIZE);
    more->setTarget(this, NULL);
    return true;
}

BaseLayer *HomeScene::__getMenuLayerItem(const ccColor4B &bgColor,const ccColor3B &textColor,const CCSize &layerSize,const CCPoint &layerAnchor,const CCPoint &layerPos, const char *labelText,float fontSize)
{
    BaseLayer *baseLayer = BaseLayer::create(bgColor,layerSize);
    baseLayer->setTouchEnabled(true);
    baseLayer->ignoreAnchorPointForPosition(false);
    baseLayer->setAnchorPoint(layerAnchor);
    baseLayer->setPosition(layerPos);
    CCLabelTTF *label = CCLabelTTF::create(labelText, "Arial", fontSize);
    label->setColor(textColor);
    label->setPosition(ccp(layerSize.width/2, layerSize.height/2));
    baseLayer->addChild(label);
    addChild(baseLayer);
    return baseLayer;
}

#pragma mark----------------------------handlers---------------------------------------------

void HomeScene::__classicHandler(cocos2d::CCObject *pSender)
{
    CCLog("Click");
    GameConfig::guideTitle = "不要踩到白块儿哦~";
    GameConfig::guideContent = "点击最下面的黑块儿,不要错过任何的黑块儿哦,没有时间限制,加油吧";
    CCScene *pGameScene = GameScene::scene();
    CCDirector::sharedDirector()->replaceScene(pGameScene);
}