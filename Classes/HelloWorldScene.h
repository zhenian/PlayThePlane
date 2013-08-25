#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "CCFoePlane.h"
#include "CCProps.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    HelloWorld();
	~HelloWorld();
    virtual bool init();  
    CREATE_FUNC(HelloWorld);
    virtual void update(float delta);
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void registerWithTouchDispatcher();
    
    static cocos2d::CCScene* scene();
private:
    // 背景
    cocos2d::CCSprite *BG1;
    cocos2d::CCSprite *BG2;
    
    // 分数
    cocos2d::CCLabelTTF *scoreLabel;
    int score;
    
    int adjustmentBG;
    
    // 玩家飞机
    cocos2d::CCSprite *player;
    // 子弹
    cocos2d::CCSprite *bullet;
    // 子弹数量
    int bulletSum;
    // 子弹样式
    bool isBigBullet;
    bool isChangeBullet;
    // 子弹速度
    int bulletSpeed;
    // 特殊子弹时间
    int bulletTiming;
    
    // 敌方飞机
    //cocos2d::CCArray *foePlanes;
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray*, __foePlanes, FoePlanes);
    
    // 添加飞机时机
    int bigPlan;
    int smallPlan;
    int mediumPlan;
    
    // 道具
    CCProps *prop;
    // 添加道具时机
    int props;
    // 是否存在
    bool isVisible;
    
    cocos2d::CCLabelTTF *gameOverLabel;
    cocos2d::CCMenu *restart;
    bool isGameOver;
    
protected:
    void initData();
    void loadBackground();
    void loadPlayer();
    void backgrouneScroll();
    void madeBullet();
    void resetBullet();
    void firingBullets();
    cocos2d::CCPoint boundLayerPos(cocos2d::CCPoint newPos);
    void panForTranslation(cocos2d::CCPoint translation);
    void moveFoePlane();
    void addFoePlane();
    CCFoePlane* makeBigFoePlane();
    CCFoePlane* makeMediumFoePlane();
    CCFoePlane* makeSmallFoePlane();
    void makeProps();
    void bulletTimingFn();
    void collisionDetection();
    void fowPlaneHitAnimation(CCFoePlane* foePlane);
    void fowPlaneBlowupAnimation(CCFoePlane*foePlane);
    void playerBlowupAnimation();
    void playerBlowupEnd(CCObject* sender);
    void blowupEnd(CCObject* sender);
    void gameOver();
    void restartFn();
};

#endif // __HELLOWORLD_SCENE_H__
