#include "GameScene.h"
#include "SimpleAudioEngine.h"

#include "Fish.h"
#include "FishCache.h"
#include "CannonCache.h"
#include "BulletCache.h"

using namespace cocos2d;
using namespace std;

// CC_DEGREES_TO_RADIANS()

static GameScene* instanceOfGameScene;

CCScene* GameScene::scene()
{
    CCScene * scene = NULL;
    do
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        GameScene *layer = GameScene::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

bool GameScene::init()
{
    bool bRet = false;
    do
    {
        /** 设置随机种子 */
        srand(time(NULL));

        /* super init first */
        CC_BREAK_IF(! CCLayer::init());
        instanceOfGameScene = this;

        // 1. Add a menu item with "X" image, which is clicked to quit the program.
        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(GameScene::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        this->windowSize = CCDirector::sharedDirector()->getWinSize();
        CCLOG("window_size: {width: %f, height: %f}", this->windowSize.width, this->windowSize.height);
        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(this->windowSize.width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

        /** 开启触控 */
        this->setTouchEnabled(true);
        /** 批量加载资源 */
        this->loadFrames();
        this->loadSource();
        /** 初始化基本 UI */
        this->initUI();

        /** 初始化配置 */
        Fish::initFishConf();

        this->initGame();

        /** schedule */
        this->schedule(schedule_selector(GameScene::update), 0.5f);

        bRet = true;
    } while (0);

    return bRet;
}

GameScene *GameScene::sharedGameScene(void)
{
    assert(NULL != instanceOfGameScene);

    return instanceOfGameScene;
}

void GameScene::loadFrames(void)
{
    CCLOG("GameScene::loadFrames(void)");
    CCSpriteFrameCache *pFC = CCSpriteFrameCache::sharedSpriteFrameCache();
    assert(NULL != pFC);

    pFC->addSpriteFramesWithFile("fish.plist");
    pFC->addSpriteFramesWithFile("fish2.plist");
    pFC->addSpriteFramesWithFile("fish3.plist");
    pFC->addSpriteFramesWithFile("fish4.plist");
    pFC->addSpriteFramesWithFile("cannon.plist");
}

void GameScene::loadSource(void)
{
    CCLOG("GameScene::loadSource(void)");

    CCTexture2D *texture = NULL;
    cocos2d::CCSpriteBatchNode *node = NULL;
    CCTextureCache *pSTC = CCTextureCache::sharedTextureCache();

    texture = pSTC->addImage("fish.png");
    node    = CCSpriteBatchNode::createWithTexture(texture);
    this->addChild(node, UI_LAYER_TAG, SourceNodeTagFish01);

    texture = pSTC->addImage("fish2.png");
    node    = CCSpriteBatchNode::createWithTexture(texture);
    this->addChild(node, UI_LAYER_TAG, SourceNodeTagFish02);

    texture = pSTC->addImage("fish3.png");
    node    = CCSpriteBatchNode::createWithTexture(texture);
    this->addChild(node, UI_LAYER_TAG, SourceNodeTagFish03);

    texture = pSTC->addImage("fish4.png");
    node    = CCSpriteBatchNode::createWithTexture(texture);
    this->addChild(node, UI_LAYER_TAG, SourceNodeTagFish04);

    texture = pSTC->addImage("cannon.png");
    node    = CCSpriteBatchNode::createWithTexture(texture);
    this->addChild(node, UI_LAYER_TAG, SourceNodeTagCannon);
}

void GameScene::initUI(void)
{
    CCLOG("GameScene::initUI(void)");

    CCTextureCache *pSTC = CCTextureCache::sharedTextureCache();

    /** 背景 */
    CCTexture2D *texture = pSTC->addImage("bj01.jpg");
    CCSprite *pBG = CCSprite::createWithTexture(texture);
    pBG->setAnchorPoint(ccp(0.5f, 0.5f));
    pBG->setPosition(ccp(this->windowSize.width * 0.5f, this->windowSize.height * 0.5f));
    this->addChild(pBG);

    /** top bar */
    texture = pSTC->addImage("ui_box_01.png");
    CCSprite *pTopBar = CCSprite::createWithTexture(texture);
    pTopBar->setAnchorPoint(ccp(0.5f, 1.0f));
    pTopBar->setPosition(ccp((this->windowSize.width * 0.5f), (this->windowSize.height)));
    this->addChild(pTopBar, UI_LAYER_TAG);

    /** 底栏静态 UI 图 */
    texture = pSTC->addImage("ui_box_02.png");
    CCSprite *pBottomBar = CCSprite::createWithTexture(texture);
    pBottomBar->setAnchorPoint(ccp(0.0f, 0.0f));
    pBottomBar->setPosition(ccp(0.0f, 0.0f));
    this->addChild(pBottomBar, UI_LAYER_TAG);
}

void GameScene::initGame(void)
{
    CCLOG("GameScene::intGame(void)");

    FishCache *fish_cache = new FishCache();
    this->addChild(fish_cache, GAME_LAYER_TAG, GameSceneNodeTagFish);

    CannonCache *cannon_cache = new CannonCache();
    this->addChild(cannon_cache, OP_LAYER_TAG, GameSceneNodeTagCannon);

    BulletCache *bullet_cache = new BulletCache();
    this->addChild(bullet_cache, GAME_LAYER_TAG, GameSceneNodeTagBullet);
}

void GameScene::update(float delta)
{
    CCLOG("GameScene::update->delte = %f", delta);

    CCNode *node_cache = this->getChildByTag(GameSceneNodeTagFish);

    FishCache *fish_cache = (FishCache *)node_cache;
    FishCache::spawnFish(fish_cache);
}

void GameScene::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();

    /** ios 设备最好加上以下内容 */
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void GameScene::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCSetIterator it = pTouches->begin();
    for (; it != pTouches->end(); it++)
    {
        CCTouch *pTouch = (CCTouch *)*it;
        CCPoint location = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
        this->touchEvent(location);
    }
}

void GameScene::touchEvent(CCPoint touch_pos)
{
    CCNode *node_cache = this->getChildByTag(GameSceneNodeTagCannon);
    CannonCache *cannon_cache = (CannonCache *)node_cache;

    CannonCache::touchFollow(cannon_cache, touch_pos);

    node_cache = this->getChildByTag(GameSceneNodeTagBullet);
    BulletCache *bullet_cache = (BulletCache *)node_cache;

    BulletCache::shootBullet(bullet_cache, touch_pos, cannon_cache->level);
}