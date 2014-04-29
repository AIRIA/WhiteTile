//
//  PianoPlayer.cpp
//  WhiteTile
//
//  Created by AIRIA on 14-4-29.
//
//

#include "PianoPlayer.h"
#include "GameConfig.h"

std::vector<std::string> PianoPlayer::tonghua;

void PianoPlayer::play()
{
    int key = rand()%26+1;
    char keyName[20];
    sprintf(keyName,"piano/sound%d.mp3",key);
    SimpleAudioEngine::sharedEngine()->playEffect(keyName);
}



