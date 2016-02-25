#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "constant.h"
#include "MainGame.h"

USING_NS_CC;
using namespace CocosDenshion;

static int HomeScreenMenuCount = sizeof(HomeScreenMenuNames) / sizeof(*HomeScreenMenuNames);
static int GameMenuCount = sizeof(GameMenuNames) / sizeof(*GameMenuNames);

FirstLevelMenuScene::FirstLevelMenuScene(bool bPortrait)
{
	CCScene::init();
}

void FirstLevelMenuScene::onEnter()
{
	CCScene::onEnter();

	CCMenuItemImage* pBackItem = CCMenuItemImage::create(
		"back.png",
		"back_big.png",
		this,
		menu_selector(FirstLevelMenuScene::ManiMenuCallback));
	CCMenu* pMenu = CCMenu::create(pBackItem, NULL);

	CCSize backSize = pBackItem->getContentSize();

	pMenu->setPosition(ccp(backSize.width, backSize.height));

	addChild(pMenu, 1);
}

void FirstLevelMenuScene::ManiMenuCallback(CCObject* pSender)
{
	CCScene* pScene = HelloWorld::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void GameMenuScene::runThisScene()
{
	CCLayer* pLayer = new GameMenuLayer();
	pLayer->init();
	pLayer->autorelease();
	addChild(pLayer);

	CCDirector::sharedDirector()->replaceScene(this);
}

void GameMenuLayer::onEnter()
{
	CCLayer::onEnter();

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCMenu* pMenu = CCMenu::create();

	for (int i = 0; i < GameMenuCount; i++)
	{
		CCLabelBMFont* label = CCLabelBMFont::create(GameMenuNames[i].c_str(), "futura-48.fnt", 48);
		CCMenuItemLabel* menuItem = CCMenuItemLabel::create(label, this, menu_selector(GameMenuLayer::menuCallback));
		pMenu->addChild(menuItem, i + 1000);
		menuItem->setPosition(ccp(size.width / 2, size.height - (i + 1) * 200));
	}
	pMenu->setPosition(Vec2::ZERO);
	addChild(pMenu);

}

void GameMenuLayer::onExit()
{
	CCLayer::onExit();
}

void GameMenuLayer::menuCallback(CCObject* pSender)
{
	CCMenuItem* menuItem = (CCMenuItem*)(pSender);
	int nIdx = menuItem->getZOrder() - 1000;

	if (nIdx == GAME_MENU_START)
	{
		GameScene* pScene = new GameScene();
		pScene->runThisTest();
		pScene->release();
	}
}

CCScene* GameMenuLayer::scene()
{
	CCScene* scene = CCScene::create();

	GameMenuLayer* layer = GameMenuLayer::create();

	scene->addChild(layer);

	return scene;
}
static FirstLevelMenuScene* CreateScene(int nIdx)
{
	CCDirector::sharedDirector()->purgeCachedData();

	FirstLevelMenuScene* pScene = NULL;

	switch (nIdx)
	{
	case SCENE_GAME:
		pScene = new GameMenuScene();
		break;
	case SCENE_HELP:
		break;
	case SCENE_SETTING:
		break;
	case SCENE_ABOUT:
		break;
	}

	return pScene;
}


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

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!CCLayer::init())
	{
		return false;
	}

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	/////////////////////////////
	// 3. add your codes below...

	//title
	CCLabelBMFont* titleFont = CCLabelBMFont::create("Breakout", "futura-48.fnt", 80);
	addChild(titleFont);
	titleFont->setPosition(ccp(size.width / 2, size.height - 200));

	//add menu
	CCMenu* pItemMenu = CCMenu::create();
	for (int i = 0; i < HomeScreenMenuCount; i++)
	{
		CCLabelBMFont* label = CCLabelBMFont::create(HomeScreenMenuNames[i].c_str(), "futura-48.fnt", 48);
		CCMenuItemLabel* pMenu = CCMenuItemLabel::create(label, this, menu_selector(HelloWorld::menuCallback));
		pItemMenu->addChild(pMenu, i + 1000);
		pMenu->setPosition(ccp(size.width / 2, size.height - (i + 2) * 200));
	}
	pItemMenu->setPosition(Vec2::ZERO);
	addChild(pItemMenu, 2);


	setTouchEnabled(true);

	return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void HelloWorld::menuCallback(Ref* pSender)
{
	CCMenuItem* pMenuItem = (CCMenuItem*)(pSender);
	int nIdx = pMenuItem->getZOrder() - 1000;

	//create the scene and run it
	FirstLevelMenuScene* pScene = CreateScene(nIdx);
	if (pScene)
	{
		pScene->runThisScene();
		pScene->release();
	}
}
