//
//  GameConfig.cpp
//  WhiteTile
//
//  Created by AIRIA on 14-4-21.
//
//

#include "GameConfig.h"
#include "cocos2d.h"

using namespace cocos2d;

SoundType GameConfig::soundType = kPiano;
bool GameConfig::guide = false;

std::string GameConfig::guideTitle = "";
std::string GameConfig::guideContent = "";
int GameConfig::tileCount = 0;
int GameConfig::score = 0;
CCArray *GameConfig::blackTiles = CCArray::create();

void GameConfig::init()
{
    CCUserDefault *userDefualt = CCUserDefault::sharedUserDefault();
    if(userDefualt->isXMLFileExist()==true)
    {
        guide = userDefualt->getBoolForKey("guide");
        soundType = (SoundType)userDefualt->getIntegerForKey("soundType");
    }else{
        save();
    }
    
}

void GameConfig::save()
{
    CCUserDefault *userDefualt = CCUserDefault::sharedUserDefault();
    userDefualt->setBoolForKey("guide", true);
    userDefualt->setIntegerForKey("soundType", kPiano);
}