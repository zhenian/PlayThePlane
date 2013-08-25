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

void IntroLayer::update(float delta){
        this->backgrouneScroll();
}

// 载入背景
void IntroLayer::loadBackground()
{
    this->BG1 = CCSprite::createWithSpriteFrameName("background_2.png");
    this->BG1->setAnchorPoint(ccp(0.5, 0));
    this->BG1->setPosition(ccp(160, adjustmentBG));
    this->addChild(BG1,0);
    
    this->BG2 = CCSprite::createWithSpriteFrameName("background_2.png");
    this->BG2->setAnchorPoint(ccp(0.5, 0));
    this->BG2->setPosition(ccp(160, 568+adjustmentBG));
    this->addChild(BG2,0);

    
}

// 背景滚动
void IntroLayer::backgrouneScroll()
{
    adjustmentBG--;
    
    if (adjustmentBG<=0) {
        adjustmentBG = 568;
    }
    BG1->setPosition(ccp(160, adjustmentBG));
    BG2->setPosition(ccp(160, adjustmentBG-568));
    
    
}

bool IntroLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->textureForKey("gameArts.png");
    CCSpriteBatchNode *spriteBatch = CCSpriteBatchNode::createWithTexture(texture);
    this->addChild(spriteBatch);
    
    this->loadBackground();
    this->scheduleUpdate();
    
    return true;
}

void IntroLayer::onEnter()
{
    CCScene *pScene = HelloWorld::scene();
    CCTransitionFade* transitionScene = CCTransitionFade::create(1.0, pScene,ccWHITE);
    CCDirector::sharedDirector()->replaceScene(transitionScene);
}

