#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class FirstLevelMenuScene : public CCScene
{
public:
	FirstLevelMenuScene(bool bPortrait = false);
	virtual void onEnter();

	virtual void runThisScene() = 0;


	// callback for back to the homescreen menu page
	virtual void ManiMenuCallback(CCObject* pSender);
};

typedef CCLayer* (*NEWTESTFUNC)();
#define TESTLAYER_CREATE_FUNC(className) \
static CCLayer* create##className() \
{ return new className(); }

class GameMenuScene : public FirstLevelMenuScene
{
public:
	virtual void runThisScene();
};

class GameMenuLayer : public CCLayer
{
public:

	static CCScene* scene();

	virtual void onEnter();
	virtual void onExit();

	void menuCallback(CCObject* pSender);

	CREATE_FUNC(GameMenuLayer);
};

class HelloWorld : public cocos2d::CCLayer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// a selector callback from different scenes to main menu
	void menuCallback(cocos2d::Ref* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
