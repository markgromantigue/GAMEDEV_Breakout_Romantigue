#include "MainMenu.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
	auto scene = Scene::create();
	auto layer = MainMenu::create();
	scene->addChild(layer);
	return scene;
}

bool MainMenu::init()
{
	if(!Layer::init())
		return false;

	auto size = Director::getInstance()->getWinSize();

	auto spriteBG = Sprite::create("breakoutBG.png");
	spriteBG->setPosition(Point(size.width/2, size.height/2));
	this->addChild(spriteBG);

	auto menuItemStart = MenuItemFont::create("START", CC_CALLBACK_1(MainMenu::menuCallBack, this));
	menuItemStart->setTag(1);
	auto menuItemHelp = MenuItemFont::create(" ", CC_CALLBACK_1(MainMenu::menuCallBack, this));
	menuItemHelp->setTag(2);
	auto menu = Menu::create(menuItemStart, menuItemHelp, NULL);

	menu->setPosition(Point::ZERO);
	menuItemStart->setPosition(Point(400, 80));
	//menuItemHelp->setPosition(Point(size.width*0.6, size.height*0.2));

	this->addChild(menu);
	return true;
}

void MainMenu::menuCallBack(Ref* obj)
{
	auto target = (Node *)obj;
	Scene * scene;

	switch (target->getTag())
	{
	case 1:
		break;
	default:
		break;
	}
	Director::getInstance()->replaceScene(scene);
}