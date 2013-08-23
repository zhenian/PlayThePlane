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
    /*
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    
    
    void playGame(CCObject* pSender);
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void registerWithTouchDispatcher();
private:
    bool isEnter;
     */
};


#endif /* defined(__PlayThePlane__IntroLayer__) */
