//
//  ShareLayer.cpp
//  WhiteTile
//
//  Created by AIRIA on 14-4-28.
//
//
#include "BaseSprite.h"
#include "ShareLayer.h"
#include "VisibleRect.h"
#include "ShareUtil.h"
#include "SimpleAudioEngine.h"

#define WT_ACT_TIME 0.2f

using namespace CocosDenshion;

bool ShareLayer::init()
{
    if (!BaseLayer::init()) {
        return false;
    }
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    BaseSprite *bg = BaseSprite::create("whiteBlock.png");
    bg->setColor(ccBLACK);
    bg->setTouchEnabled(true);
    bg->setTouchPriority(-129);
    bg->setPosition(VisibleRect::center());
    CCSize sprSize = bg->getContentSize();
    bg->setScaleX(winSize.width/sprSize.width);
    bg->setScaleY(winSize.height/sprSize.height);
    
    bg->setOpacity(0);
    CCActionInterval *fadeToAct = CCFadeTo::create(WT_ACT_TIME, 128);
    bg->runAction(fadeToAct);
    addChild(bg);
    CCNode *menuNode = CCNode::create();
    BaseSprite *cancel = BaseSprite::create("share/share_white.png");
    sprSize = cancel->getContentSize();
    cancel->setAnchorPoint(ccp(0.5, 0));
    cancel->setScaleX(winSize.width/sprSize.width);
    cancel->setScaleY(50/sprSize.height);
    CCLabelTTF *txt = CCLabelTTF::create("取消", "Arial", 30);
    txt->setColor(ccc3(61,127,190));
    cancel->setPosition(ccp(winSize.width/2, 0));
    cancel->setTouchEnabled(true);
    cancel->setTouchPriority(-130);
    cancel->setTargetBegan(this, menu_selector(ShareLayer::__cancelHandler));
    menuNode->addChild(cancel);
    menuNode->addChild(txt);
    txt->setPosition(ccp(winSize.width/2, 25));
    
    CCSprite *menuBg = CCSprite::create("share/share_white.png");
    menuBg->setScaleX(winSize.width/sprSize.width);
    menuBg->setScaleY(200/sprSize.height);
    menuBg->setAnchorPoint(ccp(0.5f, 0.0f));
    menuBg->setPosition(cancel->getPosition()+ccp(0, cancel->boundingBox().size.height+1));
    menuNode->addChild(menuBg);
    addChild(menuNode);
    menuNode->setPosition(ccp(0, -300));
    CCActionInterval *move = CCMoveTo::create(WT_ACT_TIME, CCPointZero);
    menuNode->runAction(move);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("share/share_icons.plist");
    CCMenuItemSprite *facebook = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("share_facebook"), CCSprite::createWithSpriteFrameName("share_facebook"),this,menu_selector(ShareLayer::__cancelHandler));
    CCMenuItemSprite *weixin = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("share_weixin"), CCSprite::createWithSpriteFrameName("share_weixin"),this,menu_selector(ShareLayer::__weixinHandler));
    CCMenuItemSprite *save = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("share_photos"), CCSprite::createWithSpriteFrameName("share_photos"),this,menu_selector(ShareLayer::__cancelHandler));
    facebook->setScale(0.6f);
    weixin->setScale(0.6f);
    save->setScale(0.6f);
    CCMenu *shareMenu = CCMenu::create(facebook,weixin,save,NULL);
    shareMenu->alignItemsHorizontallyWithPadding(60);
    shareMenu->setPosition(ccp(winSize.width/2,170));
    menuNode->addChild(shareMenu);
    shareMenu->setTouchPriority(-131);
    return true;
}

void ShareLayer::__cancelHandler(cocos2d::CCObject *pSender)
{
    removeFromParent();
}

void ShareLayer::__weixinHandler(cocos2d::CCObject *pSender)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite *sprite = CCSprite::create("whiteBlock.png");
    CCSize sprSize = sprite->getContentSize();
    sprite->setScaleX(winSize.width/sprSize.width);
    sprite->setScaleY(winSize.height/sprSize.height);
    sprite->setPosition(VisibleRect::center());
    sprite->setOpacity(0);
    CCActionInterval *fadeIn = CCFadeIn::create(0.2f);
    CCActionInterval *fadeOut = CCFadeOut::create(0.2f);
    CCCallFunc *fadeAct = CCCallFunc::create(this, callfunc_selector(ShareLayer::__shareHandler));
    CCSequence *seqAct = CCSequence::create(fadeIn,fadeOut,fadeAct,NULL);
    sprite->runAction(seqAct);
    removeFromParent();
    CCScene *runningScene = CCDirector::sharedDirector()->getRunningScene();
    runningScene->addChild(sprite);
    SimpleAudioEngine::sharedEngine()->playEffect("tick.m4a");
}

void ShareLayer::__shareHandler()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCRenderTexture *render = CCRenderTexture::create(winSize.width, winSize.height);
    CCScene *runningScene = CCDirector::sharedDirector()->getRunningScene();
    render->begin();
    runningScene->visit();
    render->end();
    render->saveToFile("record.png",kCCImageFormatPNG);
    ShareUtil::share2wx((CCFileUtils::sharedFileUtils()->getWritablePath()+"record.png").c_str());
}
