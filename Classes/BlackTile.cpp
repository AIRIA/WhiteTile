//
//  BlackTile.cpp
//  WhiteTile
//
//  Created by AIRIA on 14-4-22.
//
//

#include "BlackTile.h"
#include "GameConfig.h"

bool BlackTile::init()
{
    if (!BaseLayer::init()) {
        return false;
    }
    setTouchEnabled(true);
    
    return true;
}

void BlackTile::onEnter()
{
    BaseLayer::onEnter();
    scheduleUpdate();
}

BlackTile *BlackTile::create(float width, float height)
{
    BlackTile *bt = new BlackTile();
    if(bt&&bt->initWithColor(cc4BLACK, width, height))
    {
        GameConfig::tileCount++;
        bt->index = GameConfig::tileCount;
        bt->m_bIsTouched = false;
        bt->autorelease();
        GameConfig::blackTiles->addObject(bt);
        bt->m_winSize = CCDirector::sharedDirector()->getWinSize();
        return bt;
    }
    CC_SAFE_DELETE(bt);
    return NULL;
}

bool BlackTile::_isContainPoint(cocos2d::CCTouch *touch)
{
    CCPoint arPoint = getAnchorPointInPoints();
    CCPoint point = convertTouchToNodeSpaceAR(touch);
    CCRect rect = boundingBox();
    CCSize spriteSize = rect.size;
    CCRect spriteRect = CCRectMake(-arPoint.x,-arPoint.y-spriteSize.height/3,spriteSize.width,spriteSize.height+spriteSize.height/3);
    return spriteRect.containsPoint(point);
}

bool BlackTile::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(_isContainPoint(pTouch)&&GameConfig::score == (index-1))
    {
        
        CCSize tileSize = getContentSize();
        gray = CCLayerColor::create(ccc4(203, 203, 203, 255),tileSize.width,tileSize.height);
        gray->setScale(0.7f,0.8f);
        gray->ignoreAnchorPointForPosition(false);
        gray->setPosition(ccp(tileSize.width/2, tileSize.height/2));
        addChild(gray);
        return true;
    }
    return false;
}

void BlackTile::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(gray!=NULL){
        BaseLayer::ccTouchEnded(pTouch, pEvent);
        m_bIsTouched = true;
        CCLog("touch row:%d,col:%d,index:%d",row,col,index);
        GameConfig::score++;
        CCActionInterval *scaleAction = CCScaleTo::create(0.15f, 1.0f);
        gray->runAction(scaleAction);
        setTouchEnabled(false);
        unscheduleUpdate();
        /* 更新滚动速度 */
        if(GameConfig::score%60==0)
        {
            GameConfig::speed = WT_INIT_SPEED+1;
            GameConfig::speed = GameConfig::speed>23?23:GameConfig::speed;
        }
        CCNotificationCenter::sharedNotificationCenter()->postNotification(WT_UPDATE_SCORE);
        /* 判断是不是需要更新tile */
        float realPassScreen = abs(GameConfig::scroller->getPositionY())/m_winSize.height;
        int passScreen = realPassScreen;
        if(passScreen>GameConfig::passScreens&&realPassScreen>=(GameConfig::passScreens+0.5))
        {
            GameConfig::passScreens = passScreen;
            CCNotificationCenter::sharedNotificationCenter()->postNotification(WT_UPDATE_SCROLLER_POS);
        }

    }
}

void BlackTile::update(float delta)
{
    CCPoint worldPos = getParent()->convertToWorldSpace(getPosition());
    if (worldPos.y<=-m_winSize.height/4&&m_bIsTouched==false) {
        CCNotificationCenter::sharedNotificationCenter()->postNotification(WT_GAME_OVER);
        unscheduleUpdate();
    }
}