//
//  GameConfig.h
//  WhiteTile
//
//  Created by AIRIA on 14-4-21.
//
//

#ifndef __WhiteTile__GameConfig__
#define __WhiteTile__GameConfig__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

static const ccColor3B GUIDE_TITLE = {255,0,0};
static const ccColor3B GUIDE_CONTENT = {0,255,0};
static const ccColor4B GUIDE_LAYER_COLOR = {0,0,0,128};
static const ccColor4B cc4BLACK={0,0,0.255};
static const ccColor4B cc4WHITE={255,255,255,255};

#define WT_UPDATE_SCORE "update_score"
#define WT_GAME_OVER "gameover"
#define WT_STOP_GAME "stop_game"
#define WT_UPDATE_SCROLLER_POS "update_scroller_position"
#define WT_STOPED_GAME "stoped_game"

#define WT_INIT_SPEED 14

enum SoundType{
    kClose,
    kDefault,
    kPiano
};

enum GameType{
    kClassic,
    kAracade,
    kChan
};

class GameConfig
{
public:
    /* 是否打开引导 */
    static bool guide;
    /* 音效类型 */
    static SoundType soundType;
    
    static void init();
    static void save();
    static int tileCount;
    static int score;
    
    /* 选择的类型 */
    static GameType gameType;
    
    static std::string guideTitle;
    static std::string guideContent;
    static int speed;
    static int passScreens;
    static CCNode *scroller;
    
    
};

#endif /* defined(__WhiteTile__GameConfig__) */
