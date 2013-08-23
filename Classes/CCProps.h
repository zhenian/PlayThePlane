//
//  CCProps.h
//  PlayThePlane
//
//  Created by jun on 13-8-23.
//
//

#ifndef __PlayThePlane__CCProps__
#define __PlayThePlane__CCProps__

#import "cocos2d.h"

typedef enum {
    propsTypeBomb = 4,
    propsTypeBullet = 5
}propsType;

class CCProps : public cocos2d::CCNode
{
public:
    CCProps();
	~CCProps();
    void initWithType(propsType type);
    void propAnimation();
public:
    cocos2d::CCSprite *prop;
    propsType type;
};



#endif /* defined(__PlayThePlane__CCProps__) */
