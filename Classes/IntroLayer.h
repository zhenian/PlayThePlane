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
    // 背景
    cocos2d::CCSprite *BG1;
    cocos2d::CCSprite *BG2;
    int adjustmentBG;
    
    IntroLayer();
	~IntroLayer();
    CREATE_FUNC(IntroLayer);
    virtual bool init();
    virtual void update(float delta);
    virtual void onEnter();
    static cocos2d::CCScene* scene();
    void loadBackground();
    void backgrouneScroll();
};


#endif /* defined(__PlayThePlane__IntroLayer__) */
