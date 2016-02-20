#include "AppDelegate.h"
#include "MainMenu.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

void AppDelegate::initGLContextAttrs()
{
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

	GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
		glview = GLViewImpl::createWithRect("SNAKE", Rect(0, 0, 800, 600));
		director->setOpenGLView(glview);
	}

	director->getOpenGLView()->setDesignResolutionSize(800, 600, ResolutionPolicy::SHOW_ALL);

	director->setDisplayStats(false);

	director->setAnimationInterval(1.0 / 60);

	FileUtils::getInstance()->addSearchPath("res");

	auto scene = MainMenu::createScene();

	// run
	director->runWithScene(scene);

	return true;
}

void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
