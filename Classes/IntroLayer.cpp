//
//  IntroLayer.cpp
//  PlayThePlane
//
//  Created by jun on 13-8-23.
//
//

#include "IntroLayer.h"
#include "HelloWorldScene.h"

using namespace cocos2d;

CCScene* IntroLayer::scene()
{
    CCScene *scene = CCScene::create();
    IntroLayer *layer = IntroLayer::create();
    scene->addChild(layer);
    return scene;
}

IntroLayer::~IntroLayer()
{
}

IntroLayer::IntroLayer()
{
}

bool IntroLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite *background = CCSprite::create("Default.png");
    background->setRotation(90);
    background->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(background);
	this->setTouchEnabled(true);
    
    return true;
}

void IntroLayer::onEnter()
{
    CCScene *pScene = HelloWorld::scene();
    CCTransitionFade* transitionScene = CCTransitionFade::create(1.0, pScene,ccWHITE);
    CCDirector::sharedDirector()->replaceScene(transitionScene);
}

