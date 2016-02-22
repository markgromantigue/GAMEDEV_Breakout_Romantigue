#include "MainMenu.h"
#include "MainGame.h"
#include "Pause.h"
#include "GameOver.h"
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include <stdlib.h>
#include <time.h>

GameScene::GameScene()
{
	GameLayer* pLayer = new GameLayer();
	addChild(pLayer);
	pLayer->release();
}

void GameScene::menuCallBack(Ref* obj)
{
	auto target = (Node *)obj;
	Scene * scene;

	switch (target->getTag())
	{
	case 1:
		
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);

		scene = MainMenu::createScene();
		Director::getInstance()->replaceScene(scene);
		break;
	case 2:
		
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

		scene = Pause::createScene();
		addChild(scene);
		Director::getInstance()->pause();
		break;
	default:
		break;
	}
	
}

GameLayer::GameLayer()
{
	CCSprite* background = CCSprite::create("gameBG.jpg");
	addChild(background);
	background->setPosition(ccp(xBck, yBck));
	background->setScaleY(1.75);

}

GameLayer::~GameLayer()
{

}

void GameLayer::update(float delta)
{


}

void GameScene::runThisTest()
{
}

void GameLayer::addBrick(CCPoint point)
{
	CCTexture2D* brickTexture = CCTextureCache::sharedTextureCache()->addImage("bricks.png");
	Brick* brick = Brick::createBrick(brickTexture);
	brick->setBrickStatu(BrickType(rand() % 6 + BRICK_BROWN));
	brick->setPosition(point);
	addChild(brick);
}
