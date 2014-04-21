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
    showGuide();
    return true;
}

void GameScene::showGuide()
{
    if(!GameConfig::guide)
    {
        return;
    }
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    BaseLayer *guideLayer = BaseLayer::create(GUIDE_LAYER_COLOR, winSize);
    
    CCLabelTTF *guideTitle = CCLabelTTF::create(GameConfig::guideTitle.c_str(), "Arial", 38);
    guideTitle->setColor(GUIDE_TITLE);
    guideTitle->setPosition(ccp(VisibleRect::center().x, VisibleRect::top().y-200));
    
    CCLabelTTF *guideContent = CCLabelTTF::create(GameConfig::guideContent.c_str(), "Arial", 30);
    guideContent->setColor(GUIDE_CONTENT);
    guideContent->setDimensions(CCSize(winSize.width,60));
    guideContent->setAnchorPoint(ccp(0.5f, 1.0f));
    guideContent->setPosition(guideTitle->getPosition()-ccp(0,40));
    
    guideLayer->addChild(guideTitle);
    guideLayer->addChild(guideContent);
    addChild(guideLayer);
}

