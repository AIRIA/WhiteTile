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
#include "ShareUtil.h"

#define MENU_FONT_SIZE 40
#define SUB_MENU_FONT_SIZE 20
#include "AdUtil.h"
enum{
	kGuide,
	kRank,
	kMark,
	kMore,
	kLabel
};

CCScene *HomeScene::scene()
{
    AdUtil::hideAds();
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
    rightTop->setTarget(this, menu_selector(HomeScene::__arcadeHandler));
    
    BaseLayer *leftBottom = __getMenuLayerItem(cc4BLACK, ccWHITE, CCSize(winSize.width/2,winSize.height/2), CCPointZero, CCPointZero, "禅", MENU_FONT_SIZE);
    leftBottom->setTarget(this, menu_selector(HomeScene::__chanHandler));
    
    BaseLayer *soundSetting = __getMenuLayerItem(cc4WHITE, ccBLACK, CCSize(winSize.width/2,winSize.height/10), ccp(0.0f, 1.0f), VisibleRect::center(), "音效:默认", SUB_MENU_FONT_SIZE);
    soundSetting->setTarget(this, NULL);
    std::string guideStr = GameConfig::guide?"引导:开":"引导:关";
    BaseLayer *guide = __getMenuLayerItem(cc4BLACK, ccWHITE, CCSize(winSize.width/2,winSize.height/10), ccp(0.0f, 1.0f), VisibleRect::center()-ccp(0,winSize.height/10), guideStr.c_str(), SUB_MENU_FONT_SIZE);
    guide->setTarget(this, menu_selector(HomeScene::__guideHandler));
    guide->setTag(kGuide);
    BaseLayer *rank = __getMenuLayerItem(cc4WHITE, ccBLACK, CCSize(winSize.width/2,winSize.height/10), ccp(0.0f, 1.0f), VisibleRect::center()-ccp(0,winSize.height/10)*2, "排行榜", SUB_MENU_FONT_SIZE);
    rank->setTarget(this, menu_selector(HomeScene::__markGame));
    BaseLayer *mark = __getMenuLayerItem(cc4BLACK, ccWHITE, CCSize(winSize.width/2,winSize.height/10), ccp(0.0f, 1.0f), VisibleRect::center()-ccp(0,winSize.height/10)*3, "评分", SUB_MENU_FONT_SIZE);
    mark->setTarget(this, menu_selector(HomeScene::__markGame));
    BaseLayer *more = __getMenuLayerItem(cc4WHITE, ccBLACK, CCSize(winSize.width/2,winSize.height/10), ccp(0.0f, 1.0f), VisibleRect::center()-ccp(0,winSize.height/10)*4, "更多游戏", SUB_MENU_FONT_SIZE);
    more->setTarget(this, menu_selector(HomeScene::__markGame));
    return true;
}

BaseLayer *HomeScene::__getMenuLayerItem(const ccColor4B &bgColor,const ccColor3B &textColor,const CCSize &layerSize,const CCPoint &layerAnchor,const CCPoint &layerPos, const char *labelText,float fontSize)
{
    BaseLayer *baseLayer = BaseLayer::create(bgColor,layerSize);
    baseLayer->setTouchEnabled(true);
    baseLayer->ignoreAnchorPointForPosition(false);
    baseLayer->setAnchorPoint(layerAnchor);
    baseLayer->setPosition(layerPos);
    CCLabelTTF *label = CCLabelTTF::create(labelText, "DroidSans-Bold", fontSize);
    label->setColor(textColor);
    label->setTag(kLabel);
    label->setPosition(ccp(layerSize.width/2, layerSize.height/2));
    baseLayer->addChild(label);
    addChild(baseLayer);
    return baseLayer;
}

#pragma mark----------------------------handlers---------------------------------------------

void HomeScene::__classicHandler(cocos2d::CCObject *pSender)
{
    GameConfig::guideTitle = "不要踩到白块儿哦~";
    GameConfig::guideContent = "点击最下面的黑块儿,越快到达终点越棒哦.";
    CCScene *pGameScene = GameScene::scene(kClassic);
    CCDirector::sharedDirector()->replaceScene(pGameScene);
}
void HomeScene::__arcadeHandler(cocos2d::CCObject *pSender)
{
    GameConfig::guideTitle = "不要踩到白块儿哦~";
    GameConfig::guideContent = "点击最下面的黑块儿,不要错过任何的黑块儿哦,没有时间限制,开始吧";
    CCScene *pGameScene = GameScene::scene(kAracade);
    CCDirector::sharedDirector()->replaceScene(pGameScene);
}
void HomeScene::__chanHandler(cocos2d::CCObject *pSender)
{
    GameConfig::guideTitle = "不要踩到白块儿哦~";
    GameConfig::guideContent = "点击最下面的黑块儿,30秒内看你能点击多少个黑色块儿.";
    CCScene *pGameScene = GameScene::scene(kChan);
    CCDirector::sharedDirector()->replaceScene(pGameScene);
}

void HomeScene::__markGame(CCObject *pSender)
{
	ShareUtil::mark();
}

void HomeScene::__guideHandler(CCObject *pSender)
{
	CCLabelTTF *label = (CCLabelTTF*)(getChildByTag(kGuide)->getChildByTag(kLabel));
	if(GameConfig::guide){
		GameConfig::guide = false;
		label->setString("引导:关");
	}else{
		GameConfig::guide = true;
		label->setString("引导:开");
	}
	GameConfig::save();
}
