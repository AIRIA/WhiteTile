//
//  PianoPlayer.h
//  WhiteTile
//
//  Created by AIRIA on 14-4-29.
//
//

#ifndef __WhiteTile__PianoPlayer__
#define __WhiteTile__PianoPlayer__

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

class PianoPlayer
{
private:
    static std::vector<std::string> tonghua;
    static std::vector<std::string> yujian;
public:
    static void play();
};

#endif /* defined(__WhiteTile__PianoPlayer__) */
