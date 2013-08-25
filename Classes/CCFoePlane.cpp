//
//  CCFoePlane.cpp
//  PlayThePlane
//
//  Created by jun on 13-8-23.
//
//

#include "CCFoePlane.h"

CCFoePlane::CCFoePlane():planeType(0), hp(0), speed(0),__id(0){
}

CCFoePlane::~CCFoePlane(){
    //CCLOG("~~CCFoePlane()__id=%d",__id);
}

CCFoePlane* CCFoePlane::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
    CCFoePlane* pSprite = new CCFoePlane;
    if (pSprite && pSprite->initWithSpriteFrameName(pszSpriteFrameName))
    {
        pSprite->autorelease();
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    return NULL;
}