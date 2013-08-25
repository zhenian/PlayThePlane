//
//  CCFoePlane.h
//  PlayThePlane
//
//  Created by jun on 13-8-23.
//
//

#ifndef __PlayThePlane__CCFoePlane__
#define __PlayThePlane__CCFoePlane__

#include "cocos2d.h"

class CCFoePlane : public cocos2d::CCSprite
{
public:
    CCFoePlane();
	~CCFoePlane();
    static CCFoePlane* createWithSpriteFrameName(const char *pszSpriteFrameName);

public:
    int planeType, hp, speed,__id;
 
};

#endif /* defined(__PlayThePlane__CCFoePlane__) */
