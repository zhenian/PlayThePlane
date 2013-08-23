//
//  IntroLayer.h
//  PlayThePlane
//
//  Created by jun on 13-8-23.
//
//

#ifndef __PlayThePlane__IntroLayer__
#define __PlayThePlane__IntroLayer__

#include "cocos2d.h"

class IntroLayer : public cocos2d::CCLayer
{
public:
    IntroLayer();
	~IntroLayer();
    CREATE_FUNC(IntroLayer);
    virtual bool init();
    virtual void onEnter();
    static cocos2d::CCScene* scene();
};


#endif /* defined(__PlayThePlane__IntroLayer__) */
