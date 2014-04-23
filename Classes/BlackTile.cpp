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
    }
}