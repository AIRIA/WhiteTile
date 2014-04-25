//
//  HomeScene.h
//  WhiteTile
//
//  Created by AIRIA on 14-4-21.
//
//

#ifndef __WhiteTile__HomeScene__
#define __WhiteTile__HomeScene__

#include <iostream>
#include "BaseLayer.h"


using namespace cocos2d;

class HomeScene:public CCLayer
{
private:
    BaseLayer *__getMenuLayerItem(const ccColor4B &bgColor,const ccColor3B &textColor,const CCSize &layerSize,const CCPoint &layerAnchor,const CCPoint &layerPos,const char *labelText,float fontSize);
    
    void __classicHandler(CCObject *pSender);
    void __chanHandler(CCObject *pSender);
    void __arcadeHandler(CCObject *pSender);
    
public:
    static CCScene *scene();
    virtual bool init();
    CREATE_FUNC(HomeScene);
};

#endif /* defined(__WhiteTile__HomeScene__) */
