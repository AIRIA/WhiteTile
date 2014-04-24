//
//  Tile.cpp
//  WhiteTile
//
//  Created by AIRIA on 14-4-24.
//
//

#include "Tile.h"

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

void WT::Tile::onEnter()
{
    BaseSprite::onEnter();
}

void WT::Tile::onExit()
{
    BaseSprite::onExit();
    isRendering = false;
}