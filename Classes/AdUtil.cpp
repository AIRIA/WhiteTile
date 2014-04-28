//
//  AdUtil.cpp
//  WhiteTile
//
//  Created by AIRIA on 14-4-28.
//
//

#include "AdUtil.h"
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
JniMethodInfo AdUtil::methodInfo;
jobject AdUtil::adObj;
#endif

void AdUtil::init() {
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	CCLog("invoke init");
	bool isHave = JniHelper::getStaticMethodInfo(methodInfo,
			AD_CLASS, "getInstance",
			"()Ljava/lang/Object;");
	CCLog("is Have:%d",isHave);
	if (isHave) {
		CCLog("is Have:true");
		adObj = methodInfo.env->CallStaticObjectMethod(methodInfo.classID,
				methodInfo.methodID);
	} else {
		CCLog("didn't find getinstance method");
	}
#endif
}

void AdUtil::showAds() {
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	bool isHave = JniHelper::getMethodInfo(methodInfo,AD_CLASS,"showAds","(III)V");
	if(isHave) {
		jint adType = 0;
		jint adSize = 0;
		jint adPos = 4;
		CCLog("adType:%d,adSize:%d,adPos:%d",adType,adSize,adPos);
		methodInfo.env->CallObjectMethod(adObj,methodInfo.methodID,adType,adSize,adPos);
	} else {
		CCLog("didn't find method:showAds");
	}
#endif
}

void AdUtil::hideAds() {
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	bool isHave = JniHelper::getMethodInfo(methodInfo,AD_CLASS,"hideAds","(I)V");
	if(isHave) {
		jint adType = 0;
		methodInfo.env->CallObjectMethod(adObj,methodInfo.methodID,adType);
	}
#endif
}

