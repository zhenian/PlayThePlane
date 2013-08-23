//
//  CCProps.cpp
//  PlayThePlane
//
//  Created by jun on 13-8-23.
//
//

#include "CCProps.h"


CCProps::CCProps()
{
}

CCProps::~CCProps()
{
}


CCProps * CCProps::create(void)
{
	CCProps * pRet = new CCProps();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
	return pRet;
}

void CCProps::initWithType(propsType type)
{
    this->type = type;
    cocos2d::CCString *propKey = cocos2d::CCString::createWithFormat("enemy%d_fly_1.png",type);
    this->prop = cocos2d::CCSprite::createWithSpriteFrameName(propKey->getCString());
    this->prop->setPosition(ccp((arc4random()%268)+23, 732));

}

void CCProps::propAnimation()
{
    
    cocos2d::CCMoveTo* act1 = cocos2d::CCMoveTo::create(1, ccp(this->prop->getPosition().x, 400));
    cocos2d::CCMoveTo* act2 = cocos2d::CCMoveTo::create(0.2, ccp(this->prop->getPosition().x, 402));
    cocos2d::CCMoveTo* act3 = cocos2d::CCMoveTo::create(1, ccp(this->prop->getPosition().x, 732));
    cocos2d::CCMoveTo* act4 = cocos2d::CCMoveTo::create(1, ccp(this->prop->getPosition().x, -55));
    this->prop->runAction(cocos2d::CCSequence::create(act1, act2, act3, act4,NULL));
}
