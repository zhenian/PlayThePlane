#include "HelloWorldScene.h"

USING_NS_CC;
#define WINDOWHEIGHT CCDirector::sharedDirector()->getWinSize().height

static int __cnt = 1;

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
    CC_SAFE_RELEASE_NULL(__foePlanes);
    if (prop) {
        prop->release();
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
__foePlanes(NULL),
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
    this->setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    // touch事件代理
    //[[[CCDirector sharedDirector] touchDispatcher] addTargetedDelegate:self priority:0 swallowsTouches:YES];
    
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
    this->setFoePlanes(CCArray::create());
    //foePlanes->retain();
    
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
    CCObject* foeObj;
    CCARRAY_FOREACH(this->getFoePlanes(), foeObj){
        CCFoePlane *foe = (CCFoePlane *)foeObj;
        foe->setPosition(ccp(foe->getPositionX(), foe->getPositionY()-foe->speed));
         // 敌机出了底屏
        if (foe->getPositionY()< -75) {
            CCLog("-------  out of scream  1   __id=%d",foe->__id);
            this->getFoePlanes()->removeObject(foe);
            if(foe == NULL){ 
            }else{
                CCLog("-------  out of scream  2   __id=%d",foe->__id);
                foe->removeFromParent();//// ??????
                CCLog("--------------  foe is NOT NULL removve from parent (%d) %d",foe->__id,foe==NULL);
            }
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
        this->getFoePlanes()->addObject(foePlane);
        bigPlan = 0;
    }
    
    if (mediumPlan>400) {
        CCFoePlane *foePlane = this->makeMediumFoePlane();
        this->addChild(foePlane,3);
        this->getFoePlanes()->addObject(foePlane);
        mediumPlan = 0;
    }
    
    if (smallPlan>45) {
        CCFoePlane *foePlane = this->makeSmallFoePlane();
        this->addChild(foePlane,3);
        this->getFoePlanes()->addObject(foePlane);
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
    bigFoePlane->setPosition(ccp((arc4random()%210)+55, 732));
    bigFoePlane->planeType = 2;
    bigFoePlane->hp = 30;
    bigFoePlane->runAction(CCRepeatForever::create(actPlayer));
    bigFoePlane->speed = (arc4random()%2)+2 ;
    bigFoePlane->__id = __cnt++;
    return bigFoePlane;
}

// 造中飞机
CCFoePlane* HelloWorld::makeMediumFoePlane()
{
    CCFoePlane *mediumFoePlane = CCFoePlane::createWithSpriteFrameName("enemy3_fly_1.png");
    mediumFoePlane->setPosition(ccp((arc4random()%268)+23, 732));
    mediumFoePlane->planeType=3;
    mediumFoePlane->hp=15;
    mediumFoePlane->speed = (arc4random()%3)+2;
    mediumFoePlane->__id = __cnt++;
    return mediumFoePlane;
}

// 造小飞机
CCFoePlane* HelloWorld::makeSmallFoePlane()
{
    CCFoePlane *smallFoePlane = CCFoePlane::createWithSpriteFrameName("enemy1_fly_1.png");
    smallFoePlane->setPosition(ccp((arc4random()%240)+17, 732));
    smallFoePlane->planeType=1;
    smallFoePlane->hp=1;
    smallFoePlane->speed=(arc4random()%4)+2;
    smallFoePlane->__id = __cnt++;
    return smallFoePlane;
}

// 制作道具
void HelloWorld::makeProps()
{
    props++;
    if (props>1520) {
        prop = CCProps::create();
        prop->initWithType((propsType)((arc4random()%2)+4));// ?????
        this->addChild(prop->getProp());
        prop->propAnimation();
        prop->retain();
        props = 0;
        isVisible = false;
    }
    
}

// 子弹持续时间
void HelloWorld::bulletTimingFn()
{
    if (isBigBullet) {
        if (bulletTiming>0) {
            bulletTiming--;
        }else {
            isBigBullet = false;
            isChangeBullet = true;
            bulletTiming = 900;
        }
    }
}

// 碰撞检测
void HelloWorld::collisionDetection()
{
    
    // 子弹跟敌机
    CCRect bulletRec = bullet->boundingBox();
    
    CCObject* foeObj;
    CCARRAY_FOREACH(this->getFoePlanes(), foeObj){
        CCFoePlane *foePlane = (CCFoePlane *)foeObj;
        if (bulletRec.intersectsRect(foePlane->boundingBox())  ) {
            this->resetBullet();
            this->fowPlaneBlowupAnimation(foePlane);
            foePlane->hp = foePlane->hp - (isBigBullet?2:1);
            if (foePlane->hp<=0) {
                CCLog("##### move out animation:   %d   for hp(%d)",foePlane->__id,foePlane->hp);
                this->fowPlaneBlowupAnimation(foePlane);
                CCLog("##### move out begin:   %d   for hp(%d)",foePlane->__id,foePlane->hp);
                this->getFoePlanes()->removeObject(foePlane);
                CCLog("##### move out end:   %d   for hp(%d)",foePlane->__id,foePlane->hp);
            }
        }
    }
    
    // 飞机跟打飞机
    CCRect playerRec = player->boundingBox();
    playerRec.origin.x += 25;
    playerRec.size.width -= 50;
    playerRec.origin.y -= 10;
    playerRec.size.height -= 10;
    
    CCObject *foeObj3;
    CCARRAY_FOREACH(this->getFoePlanes(), foeObj3){
        CCFoePlane *foePlane = (CCFoePlane *)foeObj3;
        if (playerRec.intersectsRect(foePlane->boundingBox()) ) {
            CCLog("@@@@@ shit, i was killed  by:   %d",foePlane->__id);
            
            this->gameOver();
            this->playerBlowupAnimation();
            this->fowPlaneBlowupAnimation(foePlane);
            this->getFoePlanes()->removeObject(foePlane);
        }
    }
    
    // 飞机跟道具 
    
    if (isVisible) {
        CCRect playerRec1 = player->boundingBox();
        CCRect propRec = prop->getProp()->boundingBox();
        if (playerRec1.intersectsRect(propRec)) {
            
            prop->getProp()->stopAllActions();
            prop->getProp()->removeFromParent();
            isVisible = false;
            
            if (prop->type == propsTypeBullet) {
                isBigBullet = true;
                isChangeBullet = true;
            }else if (prop->type == propsTypeBomb) {
                CCObject *foeObj4;
                CCARRAY_FOREACH(this->getFoePlanes(), foeObj4){
                    CCFoePlane *foePlane = (CCFoePlane *)foeObj4;
                    this->fowPlaneBlowupAnimation(foePlane);
                }
                this->getFoePlanes()->removeAllObjects();
            }
        }
    }
    
    
}

// 添加打击效果
void HelloWorld::fowPlaneHitAnimation(CCFoePlane* foePlane)
{
    if (foePlane->planeType == 3) {
        if (foePlane->hp==13) {
            CCArray *playerActionArray = new CCArray;
            for (int i = 1 ; i<=2; i++) {
                CCString* key = CCString::createWithFormat("enemy3_hit_%d.png",i);
                //从内存池中取出Frame
                CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(key->getCString());
                //添加到序列中
                playerActionArray->addObject(frame);
            }
            
            //将数组转化为动画序列,换帧间隔0.1秒
            CCAnimation* animPlayer = CCAnimation::createWithSpriteFrames(playerActionArray,0.1f);
            //生成动画播放的行为对象
            CCAnimate* actPlayer = CCAnimate::create(animPlayer);
            //清空缓存数组
            playerActionArray->removeAllObjects();
            foePlane->stopAllActions();
            foePlane->runAction(CCRepeatForever::create(actPlayer));
        }
    }else if (foePlane->planeType == 2) {
        if (foePlane->hp==20) {
            CCArray *playerActionArray = new CCArray;
            for (int i = 1 ; i<=1; i++) {
                CCString* key = CCString::createWithFormat("enemy2_hit_%d.png",i);
                //从内存池中取出Frame
                CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(key->getCString());
                //添加到序列中
                playerActionArray->addObject(frame);
            }
            
            //将数组转化为动画序列,换帧间隔0.1秒
            CCAnimation* animPlayer = CCAnimation::createWithSpriteFrames(playerActionArray,0.1f);
            //生成动画播放的行为对象
            CCAnimate* actPlayer = CCAnimate::create(animPlayer);
            //清空缓存数组
            playerActionArray->removeAllObjects();
            foePlane->stopAllActions();
            foePlane->runAction(CCRepeatForever::create(actPlayer));
        }
    }
}

// 爆炸动画 
void HelloWorld::fowPlaneBlowupAnimation(CCFoePlane*foePlane)
{
    int forSum=0;
    if (foePlane->planeType == 3) {
        forSum = 4;
        score+=6000;
    }else if (foePlane->planeType  == 2) {
        score+=30000;
        forSum = 7;
    }else if (foePlane->planeType  == 1) {
        forSum = 4;
        score+=1000;
    }
    
    scoreLabel->setString(CCString::createWithFormat("%d",score)->getCString());
    
    foePlane->stopAllActions(); 
    CCArray *foePlaneActionArray = new CCArray;
    
    for (int i = 1; i<=forSum ; i++ ) { 
        CCString* key = CCString::createWithFormat("enemy%d_blowup_%i.png",foePlane->planeType , i);
        //从内存池中取出Frame
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(key->getCString());
        //添加到序列中
        foePlaneActionArray->addObject(frame);
    }
    
    //将数组转化为动画序列,换帧间隔0.1秒
    CCAnimation* animPlayer = CCAnimation::createWithSpriteFrames(foePlaneActionArray,0.1f);
    //生成动画播放的行为对象
    CCAnimate* actFowPlane = CCAnimate::create(animPlayer);
    //CCCallFuncN* end = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::blowupEnd));//?????
    //[CCCallFuncN actionWithTarget:self selector:@selector(blowupEnd:)];
    //清空缓存数组
    foePlaneActionArray->removeAllObjects();
    
    CCLog("----- 爆炸动画 begin: %d",foePlane->__id);
    foePlane->runAction(CCSequence::create(actFowPlane, CCCallFuncN::create(this, callfuncN_selector(HelloWorld::blowupEnd)), NULL));
    //foePlane->runAction(CCSequence::create(actFowPlane, CCCallFuncN::create(this, callfuncN_selector(HelloWorld::blowupEnd)),NULL));
    CCLog("----- 爆炸动画 end: %d",foePlane->__id);
}

// 飞机爆炸 
void HelloWorld::playerBlowupAnimation()
{
    
    CCArray *foePlaneActionArray = new CCArray;
    for (int i = 1; i<=4 ; i++ ) { 
        CCString* key = CCString::createWithFormat("hero_blowup_%i.png", i);
        //从内存池中取出Frame
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(key->getCString());
        //添加到序列中
        foePlaneActionArray->addObject(frame);
    }
    
    //将数组转化为动画序列,换帧间隔0.1秒
    CCAnimation* animPlayer = CCAnimation::createWithSpriteFrames(foePlaneActionArray,0.1f);
    //生成动画播放的行为对象 
    CCAnimate* actFowPlane = CCAnimate::create(animPlayer);
    //清空缓存数组
    foePlaneActionArray->removeAllObjects();
    
    player->runAction(CCSequence::create(actFowPlane, NULL));
}
//
void HelloWorld::playerBlowupEnd(CCObject* sender)
{
    
}

void HelloWorld::blowupEnd(CCObject* sender)
{
    CCFoePlane *foePlane = (CCFoePlane *) sender;
    CCLog("----- blow up end: %d",foePlane->__id);
    foePlane->removeFromParent();
}

void HelloWorld::gameOver()
{
    
    isGameOver = true;
    CCObject *nodeObj;
    CCARRAY_FOREACH(this->getChildren(), nodeObj){
        CCNode *node = (CCNode *)nodeObj;
        node->stopAllActions();
    }
    
    gameOverLabel = CCLabelTTF::create("GameOver" ,"MarkerFelt-Thin",35);
    gameOverLabel->setPosition(ccp(160, 300));
    this->addChild(gameOverLabel,4);
    
    CCMenuItemFont *gameOverItem = CCMenuItemFont::create("restart", this, menu_selector(HelloWorld::restartFn));
    gameOverItem->setFontName("MarkerFelt-Thin");
    gameOverItem->setFontSize(30);
    restart = CCMenu::create(gameOverItem,NULL);
    restart->setPosition(ccp(160, 200));
    this->addChild(restart,4);
}

void HelloWorld::restartFn()
{
    //__cnt = 1;
    this->removeAllChildren();
    this->getFoePlanes()->removeAllObjects();
    this->initData();
    this->loadBackground();
    this->loadPlayer();
    this->madeBullet();
    this->resetBullet();
    
}













