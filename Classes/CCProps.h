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
    static CCProps * create(void);
    void initWithType(propsType type);
    void propAnimation();
    //cocos2d::CCSprite *prop;
    CC_SYNTHESIZE_RETAIN(cocos2d::CCSprite*, __prop, Prop);

public:
    propsType type;
};



#endif /* defined(__PlayThePlane__CCProps__) */
