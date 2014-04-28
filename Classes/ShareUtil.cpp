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
}
