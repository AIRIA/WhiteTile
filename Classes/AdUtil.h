//
//  AdUtil.h
//  WhiteTile
//
//  Created by AIRIA on 14-4-28.
//
//

#ifndef __WhiteTile__AdUtil__
#define __WhiteTile__AdUtil__


#include <iostream>
#include "cocos2d.h"
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif

USING_NS_CC;

#define AD_CLASS "com/giant/whitetile/AdsAdmob"

class AdUtil{
private:
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	static JniMethodInfo methodInfo;
	static jobject adObj;
#endif
public:
	static void init();
    static void showAds();
    static void hideAds();
};


#endif /* defined(__WhiteTile__AdUtil__) */
