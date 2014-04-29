//
//  ShareUtil.cpp
//  WhiteTile
//
//  Created by 完美计划 on 14-4-28.
//
//

#include "ShareUtil.h"

void ShareUtil::share2wx(const char *path)
{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	bool isHave = JniHelper::getStaticMethodInfo(methodInfo, "com/giant/share/WeiXinManager", "shareImage", "(Ljava/lang/String;)V");
	if (isHave)
	{
		CCLog("invoke share method success");
		jstring pathStr = methodInfo.env->NewStringUTF(path);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, pathStr);
	}
	else
	{
		CCLog("haven't find shareImage method");
	}
#endif
}

void ShareUtil::mark()
{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	bool isHave = JniHelper::getStaticMethodInfo(methodInfo, "com/giant/share/WeiXinManager", "mark", "()V");
	if (isHave)
	{
		CCLog("invoke mark method success");
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	}
	else
	{
		CCLog("haven't find shareImage method");
	}
#endif
}
