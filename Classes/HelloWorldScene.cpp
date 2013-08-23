#include "HelloWorldScene.h"

USING_NS_CC;
#define WINDOWHEIGHT CCDirector::sharedDirector()->getWinSize().height


CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld::~HelloWorld()
{
    if (foePlanes) {
        foePlanes->release();
    }
}

HelloWorld::HelloWorld():
BG1(NULL),
BG2(NULL),
scoreLabel(NULL),
score(0),
adjustmentBG(0),
player(NULL),
bullet(NULL),
bulletSum(0),
isBigBullet(false),
isChangeBullet(false),
bulletSpeed(0),
bulletTiming(0),
foePlanes(NULL),
bigPlan(0),
smallPlan(0),
mediumPlan(0),
prop(NULL),
props(0),
isVisible(false),
gameOverLabel(NULL),
restart(NULL),
isGameOver(false)
{
}


// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("gameArts.plist", "gameArts.png");
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->textureForKey("gameArts.png");
    CCSpriteBatchNode *spriteBatch = CCSpriteBatchNode::createWithTexture(texture);
    this->addChild(spriteBatch);
    
    this->initData();
    this->loadBackground();
    this->loadPlayer();
    this->madeBullet();
    this->resetBullet();
    this->scheduleUpdate();
    
    return true;
}

void HelloWorld::update(float delta){
    if (!isGameOver) {
        this->backgrouneScroll();
        this->firingBullets();
        this->addFoePlane();
        this->moveFoePlane();
        this->collisionDetection();
        this->makeProps();
        this->bulletTimingFn();
    }
}


void HelloWorld::initData() {
    adjustmentBG = 568;
    bulletSum = 0;
    isBigBullet = false;
    isChangeBullet = false;
    bulletTiming = 900;
    bulletSpeed = 25;
    bigPlan = 0;
    smallPlan = 0;
    mediumPlan = 0;
    props = 0;
    score = 0;
    isVisible = false;
    isGameOver = false;
    foePlanes = new CCArray;
    foePlanes->retain();
    
}

/* ----资源加载-------*/

// 载入背景
void HelloWorld::loadBackground()
{
    this->BG1 = CCSprite::createWithSpriteFrameName("background_2.png");
    this->BG1->setAnchorPoint(ccp(0.5, 0));
    this->BG1->setPosition(ccp(160, adjustmentBG));
    this->addChild(BG1,0);
    
    this->BG2 = CCSprite::createWithSpriteFrameName("background_2.png");
    this->BG2->setAnchorPoint(ccp(0.5, 0));
    this->BG2->setPosition(ccp(160, 568+adjustmentBG));
    this->addChild(BG2,0);
    
    this->scoreLabel = CCLabelTTF::create("0000", "MarkerFelt-Thin", 18.0);
    this->scoreLabel->setColor(ccc3(0, 0, 0));
    this->scoreLabel->setAnchorPoint(ccp(0,1));
    this->scoreLabel->setPosition(ccp(10, WINDOWHEIGHT-10));
    this->addChild(scoreLabel,4);
    
}

// 背景滚动
void HelloWorld::backgrouneScroll()
{
    adjustmentBG--;
    
    if (adjustmentBG<=0) {
        adjustmentBG = 568;
    }
    BG1->setPosition(ccp(160, adjustmentBG));
    BG2->setPosition(ccp(160, adjustmentBG-568)); 
    
    
}
// 玩家飞机加载
void HelloWorld::loadPlayer()
{

    CCArray *playerActionArray = CCArray::create();
    for (int i = 1 ; i<=2; i++) {
        CCString* key = CCString::createWithFormat("hero_fly_%d.png", i);
        //从内存池中取出Frame
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(key->getCString());
        //添加到序列中
        playerActionArray->addObject(frame);
    }
    CCLog("----------  %d",playerActionArray->count());
    //将数组转化为动画序列,换帧间隔0.1秒
    CCAnimation* animPlayer = CCAnimation::createWithSpriteFrames(playerActionArray, 0.1f);
    //生成动画播放的行为对象
    CCAnimate* actPlayer = CCAnimate::create(animPlayer);
    //清空缓存数组
    playerActionArray->removeAllObjects();
    
    player = CCSprite::createWithSpriteFrameName("hero_fly_1.png");
    player->setPosition(ccp(160,50));
    this->addChild(player,3);
    player->runAction(CCRepeatForever::create(actPlayer));
    
    
}



// 发射子弹
void HelloWorld::firingBullets()
{
    this->bullet->setPosition(ccp(this->bullet->getPositionX(),this->bullet->getPositionY()+bulletSpeed));
    if (this->bullet->getPositionY()>WINDOWHEIGHT-20) {
        //[self resetBullet];
        this->resetBullet();
    }
    
}

// 子弹还原
void HelloWorld::resetBullet()
{
    
    if ((isBigBullet&&isChangeBullet)||(!isBigBullet&&isChangeBullet)) {
        this->bullet->removeFromParent();
        this->madeBullet();
        isChangeBullet = false;
    }
    
    bulletSpeed = (460-(this->player->getPositionY() + 50))/15;
    if (bulletSpeed<5)
    {
        bulletSpeed=5;
    }
    this->bullet->setPosition(ccp(this->player->getPositionX(),this->player->getPositionY()+50));
}

// 制造子弹
void HelloWorld::madeBullet()
{
    bullet= CCSprite::createWithSpriteFrameName((!isBigBullet)?"bullet1.png":"bullet2.png");
    bullet->setAnchorPoint(ccp(0.5,0.5));
    this->addChild(bullet);
}

// --------------飞机移动-----------------------


bool HelloWorld::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void HelloWorld::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint touchLocation = this->convertTouchToNodeSpace(pTouch);
    CCPoint oldTouchLocation = pTouch->getPreviousLocationInView();
    
    oldTouchLocation = CCDirector::sharedDirector()->convertToGL(oldTouchLocation);
    oldTouchLocation = this->convertToNodeSpace(oldTouchLocation);
    
    CCPoint translation = ccpSub(touchLocation, oldTouchLocation);
    this->panForTranslation(translation);

    
}

void HelloWorld::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{

}

void HelloWorld::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{

}

void HelloWorld::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, true);
}



CCPoint HelloWorld::boundLayerPos(CCPoint newPos)
{
    CCPoint retval = newPos;
    retval.x = player->getPositionX()+newPos.x;
    retval.y = player->getPositionY()+newPos.y;
    
    if (retval.x>=286) {
        retval.x = 286;
    }else if (retval.x<=33) {
        retval.x = 33;
    }
    
    if (retval.y >=WINDOWHEIGHT-50) {
        retval.y = WINDOWHEIGHT-50;
    }else if (retval.y <= 43) {
        retval.y = 43;
    }
    
    return retval;
}

void HelloWorld::panForTranslation(CCPoint translation)
{
    if (!isGameOver) {
        player->setPosition(this->boundLayerPos(translation));
    }
}


// -------------------------------------------

// 移动敌机
void HelloWorld::moveFoePlane()
{
    CCFoePlane *foe;
    CCARRAY_FOREACH(foePlanes, foe){
        foe->setPosition(ccp(foe->getPositionX(), foe->getPositionY()-foe->speed));
        if (foe->getPositionY()<(-75)) {
            foePlanes->removeObject(foe);
            foe->removeFromParent();
        }
    }
}

// 添加飞机
void HelloWorld::addFoePlane()
{
    bigPlan++;
    smallPlan++;
    mediumPlan++;
    
    if (bigPlan>500) {
        CCFoePlane *foePlane = this->makeBigFoePlane();
        this->addChild(foePlane,3);
        this->foePlanes->addObject(foePlane);
        bigPlan = 0;
    }
    
    if (mediumPlan>400) {
        CCFoePlane *foePlane = this->makeMediumFoePlane();
        this->addChild(foePlane,3);
        this->foePlanes->addObject(foePlane);
        mediumPlan = 0;
    }
    
    if (smallPlan>45) {
        CCFoePlane *foePlane = this->makeSmallFoePlane();
        this->addChild(foePlane,3);
        this->foePlanes->addObject(foePlane);
        smallPlan = 0;
    }
    
}

// 造大飞机
CCFoePlane* HelloWorld::makeBigFoePlane()
{
     
    CCArray *bigFoePlaneActionArray = CCArray::create();
    for (int i = 1 ; i<=2; i++) {
        CCString* key = CCString::createWithFormat("enemy2_fly_%i.png", i);
        //从内存池中取出Frame
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(key->getCString());
        //添加到序列中
        bigFoePlaneActionArray->addObject(frame);
    }
    
    //将数组转化为动画序列,换帧间隔0.1秒
    CCAnimation* animPlayer = CCAnimation::createWithSpriteFrames(bigFoePlaneActionArray, 0.1f);
    //生成动画播放的行为对象
    CCAnimate* actPlayer = CCAnimate::create(animPlayer);
    //清空缓存数组
    bigFoePlaneActionArray->removeAllObjects();
    
    CCFoePlane *bigFoePlane = CCFoePlane::createWithSpriteFrameName("enemy2_fly_1.png");
    [bigFoePlane setPosition:ccp((arc4random()%210)+55, 732)];
    [bigFoePlane setPlaneType:2];
    [bigFoePlane setHp:30];
    [bigFoePlane runAction:[CCRepeatForever actionWithAction:actPlayer]];
    [bigFoePlane setSpeed:(arc4random()%2)+2];
    
    return bigFoePlane;
}

// 造中飞机
CCFoePlane* HelloWorld::makeMediumFoePlane()
{
    CCFoePlane *mediumFoePlane = [CCFoePlane spriteWithSpriteFrameName:@"enemy3_fly_1.png"];
    [mediumFoePlane setPosition:ccp((arc4random()%268)+23, 732)];
    [mediumFoePlane setPlaneType:3];
    [mediumFoePlane setHp:15];
    [mediumFoePlane setSpeed:(arc4random()%3)+2];
    return mediumFoePlane;
}

// 造小飞机
CCFoePlane* HelloWorld::makeSmallFoePlane()
{
    CCFoePlane *smallFoePlane = [CCFoePlane spriteWithSpriteFrameName:@"enemy1_fly_1.png"];
    [smallFoePlane setPosition:ccp((arc4random()%240)+17, 732)];
    [smallFoePlane setPlaneType:1];
    [smallFoePlane setHp:1];
    [smallFoePlane setSpeed:(arc4random()%4)+2];
    return smallFoePlane;
}

// 制作道具
void HelloWorld::makeProps()
{
    props++;
    if (props>1520) {
        prop = [CCProps node];
        [prop initWithType:(arc4random()%2)+4];
        [self addChild:prop.prop];
        [prop propAnimation];
        [prop retain];
        props = 0;
        isVisible = YES;
    }
    
}

// 子弹持续时间
void HelloWorld::bulletTimingFn()
{
    if (isBigBullet) {
        if (bulletTiming>0) {
            bulletTiming--;
        }else {
            isBigBullet = NO;
            isChangeBullet = YES;
            bulletTiming = 900;
        }
    }
}

// 碰撞检测
void HelloWorld::collisionDetection()
{
    
    // 子弹跟敌机
    CGRect bulletRec = bullet.boundingBox;
    for (CCFoePlane *foePlane in foePlanes) {
        if (CGRectIntersectsRect(bulletRec, foePlane.boundingBox)) {
            
            [self resetBullet];
            [self fowPlaneHitAnimation:foePlane];
            foePlane.hp = foePlane.hp-(isBigBullet?2:1);
            if (foePlane.hp<=0) {
                [self fowPlaneBlowupAnimation:foePlane];
                [foePlanes removeObject:foePlane];
            }
            
        }
    }
    
    // 飞机跟打飞机
    CGRect playerRec = player.boundingBox;
    playerRec.origin.x += 25;
    playerRec.size.width -= 50;
    playerRec.origin.y -= 10;
    playerRec.size.height -= 10;
    for (CCFoePlane *foePlane in foePlanes) {
        if (CGRectIntersectsRect(playerRec, foePlane.boundingBox)) {
            [self gameOver];
            [self playerBlowupAnimation];
            [self fowPlaneBlowupAnimation:foePlane];
            [foePlanes removeObject:foePlane];
        }
    }
    
    // 飞机跟道具
    
    if (isVisible) {
        CGRect playerRec1 = player.boundingBox;
        CGRect propRec = prop.prop.boundingBox;
        if (CGRectIntersectsRect(playerRec1, propRec)) {
            
            [prop.prop stopAllActions];
            [prop.prop removeFromParent];
            isVisible = NO;
            
            if (prop.type == propsTypeBullet) {
                isBigBullet = YES;
                isChangeBullet = YES;
            }else if (prop.type == propsTypeBomb) {
                for (CCFoePlane *foePlane in foePlanes) {
                    [self fowPlaneBlowupAnimation:foePlane];
                }
                [foePlanes removeAllObjects];
            }
        }
    }
    
    
}

// 添加打击效果
void HelloWorld::fowPlaneHitAnimation(CCFoePlane* foePlane)
{
    if (foePlane.planeType == 3) {
        if (foePlane.hp==13) {
            NSMutableArray *playerActionArray = [NSMutableArray array];
            for (int i = 1 ; i<=2; i++) {
                NSString* key = [NSString stringWithFormat:@"enemy3_hit_%d.png",i];
                //从内存池中取出Frame
                CCSpriteFrame* frame = [[CCSpriteFrameCache sharedSpriteFrameCache] spriteFrameByName:key];
                //添加到序列中
                [playerActionArray addObject:frame];
            }
            
            //将数组转化为动画序列,换帧间隔0.1秒
            CCAnimation* animPlayer = [CCAnimation animationWithSpriteFrames:playerActionArray delay:0.1f];
            //生成动画播放的行为对象
            id actPlayer = [CCAnimate actionWithAnimation:animPlayer];
            //清空缓存数组
            [playerActionArray removeAllObjects];
            [foePlane stopAllActions];
            [foePlane runAction:[CCRepeatForever actionWithAction:actPlayer]];
        }
    }else if (foePlane.planeType == 2) {
        if (foePlane.hp==20) {
            NSMutableArray *playerActionArray = [NSMutableArray array];
            for (int i = 1 ; i<=1; i++) {
                NSString* key = [NSString stringWithFormat:@"enemy2_hit_%d.png",i];
                //从内存池中取出Frame
                CCSpriteFrame* frame = [[CCSpriteFrameCache sharedSpriteFrameCache] spriteFrameByName:key];
                //添加到序列中
                [playerActionArray addObject:frame];
            }
            
            //将数组转化为动画序列,换帧间隔0.1秒
            CCAnimation* animPlayer = [CCAnimation animationWithSpriteFrames:playerActionArray delay:0.1f];
            //生成动画播放的行为对象
            id actPlayer = [CCAnimate actionWithAnimation:animPlayer];
            //清空缓存数组
            [playerActionArray removeAllObjects];
            [foePlane stopAllActions];
            [foePlane runAction:[CCRepeatForever actionWithAction:actPlayer]];
        }
    }
}

// 爆炸动画 
void HelloWorld::fowPlaneBlowupAnimation(CCFoePlane*foePlane)
{
    int forSum;
    if (foePlane.planeType == 3) {
        forSum = 4;
        score+=6000;
    }else if (foePlane.planeType  == 2) {
        score+=30000;
        forSum = 7;
    }else if (foePlane.planeType  == 1) {
        forSum = 4;
        score+=1000;
    }
    
    [scoreLabel setString:[NSString stringWithFormat:@"%d",score]];
    
    [foePlane stopAllActions];
    NSMutableArray *foePlaneActionArray = [NSMutableArray array];
    
    for (int i = 1; i<=forSum ; i++ ) {
        NSString* key = [NSString stringWithFormat:@"enemy%d_blowup_%i.png",foePlane.planeType , i];
        //从内存池中取出Frame
        CCSpriteFrame* frame = [[CCSpriteFrameCache sharedSpriteFrameCache] spriteFrameByName:key];
        //添加到序列中
        [foePlaneActionArray addObject:frame];
    }
    
    //将数组转化为动画序列,换帧间隔0.1秒
    CCAnimation* animPlayer = [CCAnimation animationWithSpriteFrames:foePlaneActionArray delay:0.1f];
    //生成动画播放的行为对象
    id actFowPlane = [CCAnimate actionWithAnimation:animPlayer];
    id end = [CCCallFuncN actionWithTarget:self selector:@selector(blowupEnd:)];
    //清空缓存数组
    [foePlaneActionArray removeAllObjects];
    
    [foePlane runAction:[CCSequence actions:actFowPlane, end, nil]];
}

// 飞机爆炸 
void HelloWorld::playerBlowupAnimation()
{
    NSMutableArray *foePlaneActionArray = [NSMutableArray array];
    
    for (int i = 1; i<=4 ; i++ ) {
        NSString* key = [NSString stringWithFormat:@"hero_blowup_%i.png", i];
        //从内存池中取出Frame
        CCSpriteFrame* frame = [[CCSpriteFrameCache sharedSpriteFrameCache] spriteFrameByName:key];
        //添加到序列中
        [foePlaneActionArray addObject:frame];
    }
    
    //将数组转化为动画序列,换帧间隔0.1秒
    CCAnimation* animPlayer = [CCAnimation animationWithSpriteFrames:foePlaneActionArray delay:0.1f];
    //生成动画播放的行为对象
    id actFowPlane = [CCAnimate actionWithAnimation:animPlayer];
    //清空缓存数组
    [foePlaneActionArray removeAllObjects];
    
    [player runAction:[CCSequence actions:actFowPlane, nil]];
}
//
void HelloWorld::playerBlowupEnd(CCObject* sender)
{
    
}

void HelloWorld::blowupEnd(CCObject* sender)
{
    
    CCFoePlane *foePlane = (CCFoePlane *) sender;
    [foePlane removeFromParent];
}

void HelloWorld::gameOver()
{
    
    isGameOver = YES;
    CCNode *node;
    CCARRAY_FOREACH([self children], node){
        [node stopAllActions];
    }
    
    gameOverLabel = [CCLabelTTF labelWithString:@"GameOver" fontName:@"MarkerFelt-Thin" fontSize:35];
    [gameOverLabel setPosition:ccp(160, 300)];
    [self addChild:gameOverLabel z:4];
    
    CCMenuItemFont *gameOverItem = [CCMenuItemFont itemWithString:@"restart" target:self selector:@selector(restart)];
    [gameOverItem setFontName:@"MarkerFelt-Thin"];
    [gameOverItem setFontSize:30];
    restart = [CCMenu menuWithItems:gameOverItem, nil];
    [restart setPosition:ccp(160, 200)];
    [self addChild:restart z:4];
}

void HelloWorld::restartFn()
{
    [self removeAllChildren];
    [foePlanes removeAllObjects];
    [self initData];
    [self loadBackground];
    [self loadPlayer];
    [self madeBullet];
    [self resetBullet];
    
}













