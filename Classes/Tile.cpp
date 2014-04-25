//
//  Tile.cpp
//  WhiteTile
//
//  Created by AIRIA on 14-4-24.
//
//

#include "Tile.h"
#include "GameConfig.h"

using namespace WT;

WT::Tile *WT::Tile::create()
{
    WT::Tile *tile = new WT::Tile();
    if(tile&&tile->initWithFile("whiteBlock.png"))
    {
        tile->autorelease();
        return tile;
    }
    CC_SAFE_DELETE(tile);
    return NULL;
}

bool WT::Tile::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(_isContainPoint(pTouch)&&GameConfig::score==row)
    {
        if(m_pTargetBegan&&beganHandler)
        {
            (m_pTargetBegan->*beganHandler)(this);
        }
        return true;//swallow other event
    }
    return false;
}

void WT::Tile::update(float delta)
{
    CCPoint worldPos = getParent()->convertToWorldSpaceAR(getPosition());
    float tileHeight = getContentSize().height*getScaleY()/0.8f;
    if((worldPos.y+tileHeight/2)<0){
        CCLog("tileHeight:%f",tileHeight);
        unscheduleUpdate();
        CCNotificationCenter::sharedNotificationCenter()->postNotification(WT_GAME_OVER, this);
    }
}


