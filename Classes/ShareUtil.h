//
//  ShareUtil.h
//  WhiteTile
//
//  Created by 完美计划 on 14-4-28.
//
//

#ifndef __WhiteTile__ShareUtil__
#define __WhiteTile__ShareUtil__

#include <iostream>
#include "cocos2d.h"
//#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
//#endif

using namespace cocos2d;

class ShareUtil{
public:
	static void mark();
    static void share2wx(const char *imagePath);
};


#endif /* defined(__WhiteTile__ShareUtil__) */
