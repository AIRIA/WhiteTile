//
//  GameConfig.cpp
//  WhiteTile
//
//  Created by AIRIA on 14-4-21.
//
//

#include "GameConfig.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"


using namespace cocos2d;
using namespace CocosDenshion;
SoundType GameConfig::soundType = kPiano;
GameType GameConfig::gameType = kClassic;
bool GameConfig::guide = true;

std::string GameConfig::guideTitle = "";
std::string GameConfig::guideContent = "";
int GameConfig::tileCount = 0;
int GameConfig::score = 0;
int GameConfig::speed = WT_INIT_SPEED;
int GameConfig::passScreens = 0;
CCNode *GameConfig::scroller = NULL;

void GameConfig::init()
{
    
    char keyName[20];
    for (int i=1; i<27; i++) {
        sprintf(keyName,"piano/sound%d.mp3",i);
        SimpleAudioEngine::sharedEngine()->preloadEffect(keyName);
    }
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
