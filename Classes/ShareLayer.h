//
//  ShareLayer.h
//  WhiteTile
//
//  Created by AIRIA on 14-4-28.
//
//

#ifndef __WhiteTile__ShareLayer__
#define __WhiteTile__ShareLayer__

#include <iostream>
#include "BaseLayer.h"

class ShareLayer:public BaseLayer
{
private:
    void __cancelHandler(CCObject *pSender);
    void __weixinHandler(CCObject *pSender);
public:
    virtual bool init();
    CREATE_FUNC(ShareLayer);
};

#endif /* defined(__WhiteTile__ShareLayer__) */
