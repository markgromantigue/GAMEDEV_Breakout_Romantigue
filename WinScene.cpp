#include "WinScene.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* Win::createScene()
{
	auto scene = Scene::create();
	auto layer = Win::create();
	scene->addChild(layer);
	return scene;
}

bool Win::init()
{
	if(!Layer::init())
		return false;

	auto size = Director::getInstance()->getWinSize();

	TTFConfig config("fonts/arial.ttf", 80);

	auto label = Label::createWithTTF(config, "You Win! :))");
	label->setPosition(Point(size.width/2, size.height/2));
	this->addChild(label);

	auto menuItemBack = MenuItemFont::create("Back", CC_CALLBACK_1(Win::menuCallBack, this));
	auto menu = Menu::create(menuItemBack, NULL);

	menu->setPosition(Point::ZERO);
	menuItemBack->setPosition(Point(size.width*5/6, size.height/8));

	this->addChild(menu);

	return true;
}

void Win::menuCallBack(Ref* obj)
{
	auto scene = HelloWorld::scene();
	Director::getInstance()->replaceScene(scene);
}