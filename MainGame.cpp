#include "MainGame.h"
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "GameOverScene.h"
#include <stdlib.h>
#include <time.h>

static CCPoint BALL_RECT = ccp(12.0f, 12.0f);
static CCPoint PADDLE_RECT = ccp(54.0f, 18.0f);
static CCPoint BRICK_RECT = ccp(40.0f, 20.0f);
static CCPoint EXTRA_RECT = ccp(40.0f, 20.0f);
static CCPoint FLOOR_RECT = ccp(1000.0f, 10.0f);
static float EXTRA_DOWNSPEED = 200.0f;
static Label* labelScoreTitle;
static Label* labelScore;

CCRect GameAreaDef::touchRect;
CCRect GameAreaDef::BallMoveRect;
CCRect GameAreaDef::ScoreRect;
CCRect GameAreaDef::AdDisplayRect;

GameScene::GameScene()
{
	GameLayer* pLayer = new GameLayer();
	addChild(pLayer);
	pLayer->release();
}

void GameScene::menuCallback(CCObject* pSender)
{

}

void GameAreaDef::lazyInit()
{
	
	auto visSize = Director::getInstance()->getVisibleSize();
	auto pointOrigin = Director::getInstance()->getVisibleOrigin();

	if (touchRect.size.width == 0.0f && touchRect.size.height == 0.0f)
	{
		touchRect.size.width = 720;
		touchRect.size.height = 140;
		touchRect.origin.x = (visSize.width - touchRect.size.width) / 2 + pointOrigin.x;
		touchRect.origin.y = 0;
	}

	if (BallMoveRect.size.width == 0.0f && BallMoveRect.size.height == 0.0f)
	{
		BallMoveRect.size.width = 800;
		BallMoveRect.size.height = 500;
		BallMoveRect.origin.x = (visSize.width - 800) / 2 + pointOrigin.x;
		BallMoveRect.origin.y = touchRect.getMaxY() + 1;
	}

	if (ScoreRect.size.width == 0.0f && ScoreRect.size.height == 0.0f)
	{
		ScoreRect.size.width = 720;
		ScoreRect.size.height = 130;
		ScoreRect.origin.x = (visSize.width - touchRect.size.width) / 2 + pointOrigin.x;
		ScoreRect.origin.y = BallMoveRect.getMaxY() + 1;
	}

	if (AdDisplayRect.size.width == 0.0f && AdDisplayRect.size.height == 0.0f)
	{
		AdDisplayRect.size.width = 720;
		AdDisplayRect.size.height = 50;
		AdDisplayRect.origin.x = (visSize.width - touchRect.size.width) / 2 + pointOrigin.x;
		AdDisplayRect.origin.y = ScoreRect.getMaxY();
	}
}

CCRect GameAreaDef::getFingerTouchRect()
{
	lazyInit();
	return touchRect;
}

CCRect GameAreaDef::getBallMoveRect()
{
	lazyInit();
	return BallMoveRect;
}

CCRect GameAreaDef::getScoreRect()
{
	lazyInit();
	return ScoreRect;
}

CCRect GameAreaDef::getAdDisplayRect()
{
	lazyInit();
	return AdDisplayRect;
}

float GameAreaDef::getGameAreaLeftMost()
{
	lazyInit();
	return BallMoveRect.getMinX();
}

float GameAreaDef::getGameAreaRightMost()
{
	lazyInit();
	return BallMoveRect.getMaxX();
}

Extra::Extra()
{

}

Extra::~Extra()
{

}

enum Tag
{
	TagShadow = 1,
};

Extra* Extra::createExtra(CCTexture2D* aTexture)
{
	Extra* pExtra = new Extra();
	pExtra->initWithTexture(aTexture);
	pExtra->setTextureRect(CCRectMake(EXTRA_RECT.x, 0, EXTRA_RECT.x, EXTRA_RECT.y));
	pExtra->setScale(BRICK_SCALE_FACTOR);
	pExtra->autorelease();

	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("gfx/bricks.png");
	CCSprite* pShadow = new CCSprite();
	pShadow->initWithTexture(texture, CCRectMake(6 * EXTRA_RECT.x, 0, EXTRA_RECT.x, EXTRA_RECT.y));
	//CCSprite* pShadow= CCSprite::create("gfx/bricks.png", CCRectMake(6*EXTRA_RECT.x, 0, EXTRA_RECT.x, EXTRA_RECT.y));
	if (pShadow)
	{
		pShadow->setScale(0.8f);
		pExtra->addChild(pShadow, TagShadow);
		pShadow->autorelease();
		pShadow->setPosition(ccp(20, -10));
	}

	return pExtra;
}

void Extra::setBonusType(BonusType type)
{
	m_bonus = type;
	setTextureRect(CCRectMake(EXTRA_RECT.x*type, 0, EXTRA_RECT.x, EXTRA_RECT.y));
}

/*
void Extra::draw()
{
CCSprite::draw();
}
*/
void Extra::collidewithFloor(Floor* floor)
{
	if (boundingBox().intersectsRect(floor->boundingBox()))
	{
		//GameDirector::sharedGameDirector()->setGameStatus(m_bonus);
		removeFromParent();
		GameDirector::sharedGameDirector()->removeExtra(this);
	}
}

void Extra::collidewithPaddle(Paddle* paddle)
{
	if (boundingBox().intersectsRect(paddle->boundingBox()))
	{
		//GameDirector::sharedGameDirector()->setGameStatus(m_bonus);
		removeFromParent();
		GameDirector::sharedGameDirector()->removeExtra(this);
	}
}

void Extra::move(float delta)
{
	setPositionY(getPositionY() - EXTRA_DOWNSPEED*delta);
	if (getPositionY() < GameAreaDef::getFingerTouchRect().getMinY())
	{
		removeFromParent();
		GameDirector::sharedGameDirector()->removeExtra(this);
	}
}

Brick::Brick()
{

}

Brick::~Brick()
{

}

CCRect Brick::rect()
{
	CCPoint s = ccpMult(BRICK_RECT, BRICK_SCALE_FACTOR);
	return CCRectMake(-s.x / 2, -s.y / 2, s.x, s.y);
}

Brick* Brick::createBrick(CCTexture2D* aTexture)
{
	Brick* pBrick = new Brick();
	pBrick->initWithTexture(aTexture);
	pBrick->setTextureRect(CCRectMake(BRICK_RECT.x, 0, BRICK_RECT.x, BRICK_RECT.y));
	pBrick->setScale(BRICK_SCALE_FACTOR);
	pBrick->autorelease();

	CCSprite* pShadow = new CCSprite();
	pShadow->initWithTexture(aTexture, CCRectMake(6 * BRICK_RECT.x, 0, BRICK_RECT.x, BRICK_RECT.y));
	//CCSprite* pShadow= CCSprite::create("gfx/bricks.png", CCRectMake(6*BRICK_RECT.x, 0, BRICK_RECT.x, BRICK_RECT.y));
	if (pShadow)
	{
		pShadow->setScale(0.8f);
		pBrick->addChild(pShadow);
		pShadow->autorelease();
		pShadow->setPosition(ccp(20, -10));
	}

	return pBrick;
}

void Brick::setBrickStatu(BrickType type)
{
	m_brickType = type;
	if (type == BRICK_INVISIBLE)
	{
		setVisible(false);
		return;
	}
	setTextureRect(CCRectMake(type*BRICK_RECT.x, 0, BRICK_RECT.x, BRICK_RECT.y));
	setVisible(true);
}

bool Brick::collideWithBall(Ball* ball)
{
	if (boundingBox().intersectsRect(ball->boundingBox()))
		return true;
	else
		return false;
}

void Brick::brickCrashedByBall(Ball* ball)
{
	//0.create extra according to current brick's attribute
	CCTexture2D* extraTexture = CCTextureCache::sharedTextureCache()->addImage("gfx/extras.png");
	Extra* extra = Extra::createExtra(extraTexture);
	extra->setBonusType(SCORE_1000); //this should be changed to the bonus type associated with brick in future
	extra->setPosition(getPosition());
	GameDirector::sharedGameDirector()->addExtra(extra);
	this->getParent()->addChild(extra);

	//1.remove shadow
	//TODO...

	//2.fade in and moveby two actions in parallel
	//and then callback removebrick
	CCActionInterval* fadeout = CCFadeOut::create(1.0f);
	CCPoint delta = ccpMult(ccpNormalize(ball->getVelocity()), this->boundingBox().size.width * 3);
	CCActionInterval* moveby = CCMoveBy::create(1.0f, delta);
	CCActionInterval* spawn = CCSpawn::create(fadeout, moveby, NULL);

	CCFiniteTimeAction* removebrick = CCCallFunc::create(this, callfunc_selector(Brick::callbackRemoveBrick));

	this->runAction(CCSequence::create(spawn, removebrick, NULL));

}

void Brick::callbackRemoveBrick()
{
	GameDirector::sharedGameDirector()->removeBrick(this);
	this->removeFromParent();
}

Ball::Ball(void) : m_shadow(NULL)
{
}

Ball::~Ball(void)
{
}

float Ball::radius()
{
	return BALL_RECT.x*SCALE_FACTOR / 2; //scale the radius of ball
	//return getTexture()->getContentSize().height * SCALE_FACTOR / 2;
}

Ball* Ball::ballWithTexture(CCTexture2D* aTexture, CCRect rect)
{
	Ball* pBall = new Ball();
	pBall->initWithTexture(aTexture, rect);
	pBall->setScale(SCALE_FACTOR); //scale the ball
	//CCLog("%f", pBall->boundingBox().size.width);
	//pBall->autorelease();


	CCSprite* shadow = new CCSprite();
	shadow->initWithFile("gfx/fr_llc.png");
	pBall->addChild(shadow);
	shadow->autorelease();
	shadow->setPosition(ccp(10, -10));

	return pBall;
}

void Ball::move(float delta)
{
	this->setPosition(ccpAdd(getPosition(), ccpMult(m_velocity, delta)));

	if (getPosition().x > GameAreaDef::getBallMoveRect().getMaxX() - radius())
	{
		setPosition(ccp(GameAreaDef::getBallMoveRect().getMaxX() - radius(), getPosition().y));
		m_velocity.x *= -1;
	}
	else if (getPosition().x < GameAreaDef::getBallMoveRect().getMinX() + radius())
	{
		setPosition(ccp(GameAreaDef::getBallMoveRect().getMinX() + radius(), getPosition().y));
		m_velocity.x *= -1;
	}

	if (getPosition().y > GameAreaDef::getScoreRect().getMinY() - radius())
	{
		setPosition(ccp(getPosition().x, GameAreaDef::getScoreRect().getMinY() - radius()));
		m_velocity.y *= -1;
	}
	else if (getPosition().y < GameAreaDef::getFingerTouchRect().getMinY() + radius())
	{
		setPosition(ccp(getPosition().x, GameAreaDef::getFingerTouchRect().getMinY() + radius()));
		m_velocity.y *= -1;
	}
}

void Ball::collideWithFloor(Floor* floor)
{
	if (boundingBox().intersectsRect(floor->boundingBox()))
	{
		GameDirector::sharedGameDirector()->init();
		auto scene = GameOver::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}

void Ball::collideWithPaddle(Paddle* paddle)
{
	CCRect paddleRect = paddle->rect();
	paddleRect.origin.x += paddle->getPosition().x;
	paddleRect.origin.y += paddle->getPosition().y;

	float lowY = paddleRect.getMinY();
	float midY = paddleRect.getMidY();
	float highY = paddleRect.getMaxY();

	float leftX = paddleRect.getMinX();
	float rightX = paddleRect.getMaxX();

	if (getPosition().x > leftX && getPosition().x < rightX) {

		bool hit = false;
		float angleOffset = 0.0f;

		if (getPosition().y <= highY + this->boundingBox().size.height)
		{
			setPosition(ccp(getPosition().x, highY + this->boundingBox().size.height));
			hit = true;
			angleOffset = (float)M_PI / 2;
			//CCLog("%f, %f, %f", getPosition().y, highY, radius());
		}


		if (hit)
		{
			float hitAngle = ccpToAngle(ccpSub(paddle->getPosition(), getPosition())) + angleOffset;

			float scalarVelocity = ccpLength(m_velocity) * 1.05f;
			float velocityAngle = -ccpToAngle(m_velocity) + 0.5f * hitAngle;

			m_velocity = ccpMult(ccpForAngle(velocityAngle), scalarVelocity);
		}
	}
}

void Ball::setBallColor(BallColor color)
{
	setTextureRect(CCRectMake(color * 12, 0, 12, 12));
}

void Ball::draw(Renderer* renderer, const Mat4& transform, uint32_t flags)
{
	//CCSprite::draw();
}

void GameLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (std::find(heldKeys.begin(), heldKeys.end(), keyCode) == heldKeys.end()){
		heldKeys.push_back(keyCode);
	}
}

void GameLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	heldKeys.erase(std::remove(heldKeys.begin(), heldKeys.end(), keyCode), heldKeys.end());
}

GameLayer::GameLayer()
{
	//TODO...
	//create game including loading game data from file,
	auto size = Director::getInstance()->getVisibleSize();

	CCSprite* background = CCSprite::create("gameBG.jpg");
	addChild(background);
	float xBck = (GameAreaDef::getBallMoveRect().getMaxX() + GameAreaDef::getBallMoveRect().getMinX()) / 2;
	float yBck = (GameAreaDef::getBallMoveRect().getMaxY() + GameAreaDef::getBallMoveRect().getMinY()) / 2;
	background->setPosition(ccp(size.width / 2, size.height / 2));
	background->setScaleY(1.75);

	TTFConfig config("fonts/Marker Felt.ttf", 30);
	labelScoreTitle = Label::createWithTTF(config, "Score:");
	addChild(labelScoreTitle);
	labelScoreTitle->setPosition(ccp(80, 570));

	labelScore = Label::createWithTTF(config, "0");
	addChild(labelScore);
	labelScore->setTag(110);
	labelScore->setPosition(ccp(150, 570));


	m_ballVelocity = ccp(40.0f, -400.0f);
	m_ball = Ball::ballWithTexture(CCTextureCache::sharedTextureCache()->addImage("gfx/balls.png"), CCRectMake(0, 0, 12, 12));
	m_ball->setVelocity(m_ballVelocity);
	m_ball->setPosition(ccp(120, 320));
	addChild(m_ball);
	m_ball->retain(); //release from GameDirector
	GameDirector::sharedGameDirector()->addBall(m_ball);

	CCTexture2D* paddleTexture = CCTextureCache::sharedTextureCache()->addImage("gfx/paddle.png");
	m_paddle = Paddle::paddleWithTexture(paddleTexture);
	m_paddle->setTextureRect(CCRectMake(0, 0, 54, 18));
	m_paddle->setPosition(ccp(120, 20));
	addChild(m_paddle);
	m_paddle->retain();
	GameDirector::sharedGameDirector()->addPaddle(m_paddle);

	CCTexture2D* floorTexture = CCTextureCache::sharedTextureCache()->addImage("gfx/floor.png");
	m_floor = Floor::floorWithTexture(floorTexture);
	m_floor->setTextureRect(CCRectMake(0, 0, 1000, 18));
	m_floor->setPosition(ccp(54, 1));
	addChild(m_floor);
	m_floor->retain();
	GameDirector::sharedGameDirector()->addFloor(m_floor);

	for (int i = 0; i < 11; i++)
	{
		addBrick(ccp(100 + 60 * i, 500));
	}
	for (int i = 0; i < 11; i++)
	{
		addBrick(ccp(100 + 60 * i, 400));
	}
	schedule(schedule_selector(GameLayer::update));

	auto listener2 = EventListenerKeyboard::create();

	listener2->onKeyPressed = [&](EventKeyboard::KeyCode k, Event * e){

		switch (k)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			if (std::find(heldKeys.begin(), heldKeys.end(), k) == heldKeys.end()){
				heldKeys.push_back(k);
			}
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			if (std::find(heldKeys.begin(), heldKeys.end(), k) == heldKeys.end()){
				heldKeys.push_back(k);
			}
			break;
		default:
			break;
		}
	};

	listener2->onKeyReleased = [&](EventKeyboard::KeyCode k, Event * e){

		heldKeys.erase(std::remove(heldKeys.begin(), heldKeys.end(), k), heldKeys.end());
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);
}

GameLayer::~GameLayer()
{
	//m_ball->release();
	//m_paddle->release();
	//m_brick->release();
}

void GameLayer::update(float delta)
{
	if (std::find(heldKeys.begin(), heldKeys.end(), RIGHT_ARROW) != heldKeys.end()){
		m_paddle->setPosition(ccp(m_paddle->getPosition().x + 10, 20));
	}

	if (std::find(heldKeys.begin(), heldKeys.end(), LEFT_ARROW) != heldKeys.end()){
		m_paddle->setPosition(ccp(m_paddle->getPosition().x - 10, 20));
	}
	//this is mainly function to control the whole game
	//it should update components' movement and check interaction between components
	//it should also control the whole game layer display
	
	GameDirector::sharedGameDirector()->logicUpdate(delta);
	//m_ball->move(delta);
	//m_ball->collideWithPaddle(m_paddle);


	//m_ball->draw();

}

void GameScene::runThisTest()
{
	CCDirector::sharedDirector()->replaceScene(this);
}

void GameLayer::addBrick(CCPoint point)
{
	CCTexture2D* brickTexture = CCTextureCache::sharedTextureCache()->addImage("gfx/bricks.png");
	Brick* brick = Brick::createBrick(brickTexture);
	brick->setBrickStatu(BrickType(rand() % 6 + BRICK_BROWN));
	brick->setPosition(point);
	addChild(brick);
	//m_brick->retain();
	GameDirector::sharedGameDirector()->addBrick(brick);
}
static GameDirector* s_GameDirector = NULL;

GameDirector::GameDirector() : m_arrayBalls(NULL),
m_arrayBricks(NULL), m_arrayExtras(NULL), m_paddle(NULL)
{

}

GameDirector::~GameDirector()
{
	CCObject* pObject;
	if (m_arrayBalls)
	{
		CCARRAY_FOREACH(m_arrayBalls, pObject)
		{
			Ball* ball = (Ball*)pObject;
			ball->removeFromParent();
		}
		m_arrayBalls->release();
		m_arrayBalls = NULL;
	}
	if (m_arrayBricks)
	{
		CCARRAY_FOREACH(m_arrayBricks, pObject)
		{
			Brick* brick = (Brick*)pObject;
			brick->removeFromParent();
		}
		m_arrayBricks->release();
		m_arrayBricks = NULL;
	}
	if (m_arrayExtras)
	{
		CCARRAY_FOREACH(m_arrayExtras, pObject)
		{
			Extra* extra = (Extra*)pObject;
			extra->removeFromParent();
		}
		m_arrayExtras->release();
		m_arrayExtras = NULL;
	}
	if (m_paddle)
	{
		m_paddle = NULL;
	}
	if (s_GameDirector)
	{
		s_GameDirector = NULL;
	}
}

GameDirector* GameDirector::sharedGameDirector()
{
	if (!s_GameDirector)
	{
		srand(time(NULL)); //random seed
		s_GameDirector = new GameDirector();
		s_GameDirector->init();
	}
	return s_GameDirector;
}

void GameDirector::init()
{
	m_arrayBalls = CCArray::create();
	if (m_arrayBalls)
		m_arrayBalls->retain();
	m_arrayExtras = CCArray::create();
	if (m_arrayExtras)
		m_arrayExtras->retain();
	m_arrayBricks = CCArray::create();
	if (m_arrayBricks)
		m_arrayBricks->retain();
	resetAllBallStatus();
	resetGameScore();
}

Paddle::Paddle(void)
{
	leftMost = GameAreaDef::getGameAreaLeftMost();
	rightMost = GameAreaDef::getGameAreaRightMost();
}

Floor::Floor(void)
{
	leftMost = GameAreaDef::getGameAreaLeftMost();
	rightMost = GameAreaDef::getGameAreaRightMost();
}

Paddle::~Paddle(void)
{
}
Floor::~Floor(void)
{
}

CCRect Paddle::rect()
{
	CCPoint s = ccpMult(PADDLE_RECT, SCALE_FACTOR);
	return CCRectMake(-s.x / 2, -s.y / 2, s.x, s.y); //scale rect of paddle
}

CCRect Floor::rect()
{
	CCPoint a = ccpMult(FLOOR_RECT, SCALE_FACTOR);
	return CCRectMake(-a.x / 2, -a.y / 2, a.x, a.y); //scale rect of paddle
}

Paddle* Paddle::paddleWithTexture(CCTexture2D* aTexture)
{
	Paddle* pPaddle = new Paddle();
	pPaddle->initWithTexture(aTexture);
	pPaddle->setScale(SCALE_FACTOR); //scale paddle sprite
	pPaddle->autorelease();

	return pPaddle;
}

Floor* Floor::floorWithTexture(CCTexture2D* aTexture)
{
	Floor* pFloor = new Floor();
	pFloor->initWithTexture(aTexture);
	pFloor->setScale(SCALE_FACTOR);
	pFloor->autorelease();

	return pFloor;
}

bool Paddle::initWithTexture(CCTexture2D* aTexture)
{
	if (CCSprite::initWithTexture(aTexture))
	{
		m_state = kPaddleStateUngrabbed;
	}

	return true;
}
bool Floor::initWithTexture(CCTexture2D* aTexture)
{
	if (CCSprite::initWithTexture(aTexture))
	{
		m_state = kPaddleStateUngrabbed;
	}

	return true;
}


void Paddle::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	// pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	CCSprite::onEnter();
}

void Paddle::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	//pDirector->getTouchDispatcher()->removeDelegate(this);
	CCSprite::onExit();
}

bool Paddle::containsTouchLocation(CCTouch* touch)
{
	return rect().containsPoint(convertTouchToNodeSpaceAR(touch));
}

bool Paddle::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	if (m_state != kPaddleStateUngrabbed) return false;
	if (!containsTouchLocation(touch)) return false;

	m_state = kPaddleStateGrabbed;
	return true;
}

void Paddle::ccTouchMoved(EventKeyboard::KeyCode k, Event * e)
{
	// If it weren't for the TouchDispatcher, you would need to keep a reference
	// to the touch from touchBegan and check that the current touch is the same
	// as that one.
	// Actually, it would be even more complicated since in the Cocos dispatcher
	// you get CCSets instead of 1 UITouch, so you'd need to loop through the set
	// in each touchXXX method.

	CCAssert(m_state == kPaddleStateGrabbed, "Paddle - Unexpected state!");

	//CCPoint touchPoint = touch->getLocation();
	auto listener = EventListenerKeyboard::create();

	listener->onKeyPressed = [&](EventKeyboard::KeyCode k, Event * e){
		switch (k)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			setPosition(ccp(getPosition().x - 1, getPosition().y));
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			setPosition(ccp(getPosition().x + 1, getPosition().y));
			break;
		default:
			break;
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

CCObject* Paddle::copyWithZone(CCZone *pZone)
{
	this->retain();
	return this;
}

CCObject* Floor::copyWithZone(CCZone *pZone)
{
	this->retain();
	return this;
}

void Paddle::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCAssert(m_state == kPaddleStateGrabbed, "Paddle - Unexpected state!");

	m_state = kPaddleStateUngrabbed;
}

void Paddle::touchDelegateRetain()
{
	this->retain();
}

void Paddle::touchDelegateRelease()
{
	this->release();
}


void GameDirector::logicUpdate(float delta)
{
	//check the relationship between different bonus types
	//check if the bonus bit available as time passed

	//float fps = 1.0f/CCDirector::sharedDirector()->getSecondsPerFrame;
	for (unsigned int i = 0; i < sizeof(m_ballStatus) / sizeof(*m_ballStatus); i++)
	{
		//some bonuses have no time limit while others have
		if (i == SCORE_200 || i == SCORE_500 || i == SCORE_1000 || i == SCORE_2000 || i == SCORE_5000 || i == SCORE_10K)
			continue;
		else if (i == SHRINK_PADDLE || i == EXPAND_PADDLE)
			continue;
		else if (i == EXTRA_BALL)
			continue;
		else if (i == JOKER)
			continue;
		else if (i == RESET)
			continue;
		else if (i == TIME_ADD)
			continue;
		else if (i == RANDOM_EXTRA)
			continue;

		if (m_ballStatus[i].isEnabled)
		{
			if (m_ballStatus[i].periodInFrame)
			{
				m_ballStatus[i].periodInFrame -= delta;
				if (!m_ballStatus[i].periodInFrame <= 0.0f) //time out
				{
					m_ballStatus[i].isEnabled = false;
				}
			}
		}
	}

	CCObject* pObject;
	CCObject* pObject2;
	CCARRAY_FOREACH(m_arrayBalls, pObject)
	{
		Ball* ball = (Ball*)pObject;
		ball->move(delta);
		ball->collideWithPaddle(m_paddle);
		ball->collideWithFloor(m_floor);
		//check collision between ball and brick
		CCObject* pObject1;
		CCARRAY_FOREACH(m_arrayBricks, pObject1)
		{
			Brick* brick = (Brick*)pObject1;

			if (brick->boundingBox().intersectsRect(ball->boundingBox()))
			{
				addGameScore(1000);

				char str_score[20];
				sprintf(str_score, "%d", getGameScore());
				labelScore->setString(str_score);
				//first to check ball status
				//then to check brick associated bonus
				//for strong brick, the ball will only crash brick after some hits
				if (m_ballStatus[ENERGY_BALL].isEnabled) //energy ball, crash brick immdiately
				{
					//CCLog("Energy Ball");

				}
				else if (m_ballStatus[EXPLOSIVE_BALL].isEnabled) //explosive ball, need explosive effect
				{
					//CCLog("Explosive Ball");
				}
				else if (m_ballStatus[WEAK_BALL].isEnabled) //weak ball, 40% chance not to crash ball
				{
					//CCLog("Weak Ball");
				}
				else if (m_ballStatus[CHAOS].isEnabled) //chaos, ball reflected randomly
				{
					//CCLog("Chaos");
				}
				//for normal status, brick crashed by ball
				//brick->brickCrashedByBall(ball);
				CCPoint velocity = ball->getVelocity();
				if (brick->boundingBox().getMinX() + ball->radius() < ball->getPositionX() < brick->boundingBox().getMaxX() - ball->radius())
				{
					float yvector = ball->getPositionY() - brick->getPositionY();
					if (yvector*velocity.y < 0) //make sure the ball moves away from brick
					{
						velocity.y *= -1;
						//brick crashed by ball
						brick->brickCrashedByBall(ball);
					}
				}
				else if (brick->boundingBox().getMinX() + ball->radius() == ball->getPositionX() || brick->boundingBox().getMaxX() - ball->radius() == ball->getPositionX())
				{
					if (brick->boundingBox().getMaxY() + ball->radius() == ball->getPositionY() || brick->boundingBox().getMinY() - ball->radius() == ball->getPositionY())
					{
						velocity = ccpMult(velocity, -1.0f);
						brick->brickCrashedByBall(ball);
					}
					else
					{
						float xvector = ball->getPositionX() - brick->getPositionX();
						if (xvector*velocity.x < 0) //make sure the ball moves away from brick
						{
							velocity.x *= -1;
							brick->brickCrashedByBall(ball);
						}
					}
				}
				ball->setVelocity(velocity);
			}
		}
	}

	CCARRAY_FOREACH(m_arrayExtras, pObject2)
	{
		Extra* extra = (Extra*)pObject2;
		extra->move(delta);
		extra->collidewithFloor(m_floor);
		//extra->collidewithPaddle(m_paddle);
	}
}

void GameDirector::setGameStatus(BonusType type)
{
	switch (type)
	{
	case SCORE_200:
		addGameScore(200);
		break;
	case SCORE_500:
		addGameScore(500);
		break;
	case SCORE_1000:
		addGameScore(1000);
		break;
	case SCORE_2000:
		addGameScore(2000);
		break;
	case SCORE_5000:
		addGameScore(5000);
		break;
	case SCORE_10K:
		addGameScore(10000);
		break;

	case END_OF_RAINBOW:
		if (!m_ballStatus[type].isEnabled)
		{
			m_ballStatus[type].isEnabled = true;
			m_ballStatus[type].periodInFrame = 20.0f;
		}
		else
		{
			m_ballStatus[type].periodInFrame += 20.0f;
		}
		break;

	case STICKY_PADDLE:
		if (!m_ballStatus[type].isEnabled)
		{
			if (m_ballStatus[GHOSTLY_PADDLE].isEnabled)
			{
				m_ballStatus[GHOSTLY_PADDLE].isEnabled = false;
				m_ballStatus[GHOSTLY_PADDLE].periodInFrame = 0.0f;
			}
			if (m_ballStatus[FROZEN_PADDLE].isEnabled)
			{
				m_ballStatus[FROZEN_PADDLE].isEnabled = false;
				m_ballStatus[FROZEN_PADDLE].periodInFrame = 0.0f;
			}
			m_ballStatus[type].isEnabled = true;
			m_ballStatus[type].periodInFrame = 20.0f;
		}
		else
		{
			m_ballStatus[type].periodInFrame += 20.0f;
		}
		break;

	case ENERGY_BALL:
		if (!m_ballStatus[type].isEnabled)
		{
			if (m_ballStatus[EXPLOSIVE_BALL].isEnabled)
			{
				m_ballStatus[EXPLOSIVE_BALL].isEnabled = false;
				m_ballStatus[EXPLOSIVE_BALL].periodInFrame = 0.0f;
			}
			if (m_ballStatus[WEAK_BALL].isEnabled)
			{
				m_ballStatus[WEAK_BALL].isEnabled = false;
				m_ballStatus[WEAK_BALL].periodInFrame = 0.0f;
			}
			m_ballStatus[type].isEnabled = true;
			m_ballStatus[type].periodInFrame = 5.0f;
		}
		else
		{
			m_ballStatus[type].periodInFrame += 5.0f;
		}
		break;

	case BONUS_FLOOR:
		if (!m_ballStatus[type].isEnabled)
		{
			m_ballStatus[type].isEnabled = true;
			m_ballStatus[type].periodInFrame = 10.0f;
		}
		else
		{
			m_ballStatus[type].periodInFrame += 10.0f;
		}
		break;

	case WEAPON:
		if (!m_ballStatus[type].isEnabled)
		{
			m_ballStatus[type].isEnabled = true;
			m_ballStatus[type].periodInFrame = 5.0f;
		}
		else
		{
			m_ballStatus[type].periodInFrame += 5.0f;
		}
		break;

	case SPEED_DOWN:
		if (!m_ballStatus[type].isEnabled)
		{
			if (m_ballStatus[SPEED_UP].isEnabled)
			{
				m_ballStatus[SPEED_UP].isEnabled = false;
				m_ballStatus[SPEED_UP].periodInFrame = 0;
			}
			m_ballStatus[type].isEnabled = true;
			m_ballStatus[type].periodInFrame = 20.0f;
		}
		else
		{
			m_ballStatus[type].periodInFrame += 20.0f;
		}
		break;

	case SPEED_UP:
		if (!m_ballStatus[type].isEnabled)
		{
			if (m_ballStatus[SPEED_DOWN].isEnabled)
			{
				m_ballStatus[SPEED_DOWN].isEnabled = false;
				m_ballStatus[SPEED_DOWN].periodInFrame = 0;
			}
			m_ballStatus[type].isEnabled = true;
			m_ballStatus[type].periodInFrame = 20.0f;
		}
		else
		{
			m_ballStatus[type].periodInFrame += 20.0f;
		}
		break;

	case EXPLOSIVE_BALL:
		if (!m_ballStatus[type].isEnabled)
		{
			if (m_ballStatus[ENERGY_BALL].isEnabled)
			{
				m_ballStatus[ENERGY_BALL].isEnabled = false;
				m_ballStatus[ENERGY_BALL].periodInFrame = 0;
			}
			if (m_ballStatus[WEAK_BALL].isEnabled)
			{
				m_ballStatus[WEAK_BALL].isEnabled = false;
				m_ballStatus[WEAK_BALL].periodInFrame = 0;
			}
			m_ballStatus[type].isEnabled = true;
			m_ballStatus[type].periodInFrame = 10.0f;
		}
		else
		{
			m_ballStatus[type].periodInFrame += 10.0f;
		}
		break;

	case BONUS_MAGNET:
		if (!m_ballStatus[type].isEnabled)
		{
			if (m_ballStatus[MALUS_MAGNET].isEnabled)
			{
				m_ballStatus[MALUS_MAGNET].isEnabled = false;
				m_ballStatus[MALUS_MAGNET].periodInFrame = 0;
			}
			m_ballStatus[type].isEnabled = true;
			m_ballStatus[type].periodInFrame = 20.0f;
		}
		else
		{
			m_ballStatus[type].periodInFrame += 20.0f;
		}
		break;

	case MALUS_MAGNET:
		if (!m_ballStatus[type].isEnabled)
		{
			if (m_ballStatus[BONUS_MAGNET].isEnabled)
			{
				m_ballStatus[BONUS_MAGNET].isEnabled = false;
				m_ballStatus[BONUS_MAGNET].periodInFrame = 0;
			}
			m_ballStatus[type].isEnabled = true;
			m_ballStatus[type].periodInFrame = 20.0f;
		}
		else
		{
			m_ballStatus[type].periodInFrame += 20.0f;
		}
		break;

	case FROZEN_PADDLE:
		if (!m_ballStatus[type].isEnabled)
		{
			if (m_ballStatus[STICKY_PADDLE].isEnabled)
			{
				m_ballStatus[STICKY_PADDLE].isEnabled = false;
				m_ballStatus[STICKY_PADDLE].periodInFrame = 0;
			}
			if (m_ballStatus[GHOSTLY_PADDLE].isEnabled)
			{
				m_ballStatus[GHOSTLY_PADDLE].isEnabled = false;
				m_ballStatus[GHOSTLY_PADDLE].periodInFrame = 0;
			}
			m_ballStatus[type].isEnabled = true;
			m_ballStatus[type].periodInFrame = 1.0f;
		}
		else
		{
			m_ballStatus[type].periodInFrame += 1.0f;
		}
		break;

	case LIGHTS_OUT:
		if (!m_ballStatus[type].isEnabled)
		{
			m_ballStatus[type].isEnabled = true;
			m_ballStatus[type].periodInFrame = 20.0f;
		}
		else
		{
			m_ballStatus[type].periodInFrame += 20.0f;
		}
		break;

	case CHAOS:
		if (!m_ballStatus[type].isEnabled)
		{
			m_ballStatus[type].isEnabled = true;
			m_ballStatus[type].periodInFrame = 20.0f;
		}
		else
		{
			m_ballStatus[type].periodInFrame += 20.0f;
		}
		break;

	case GHOSTLY_PADDLE:
		if (!m_ballStatus[type].isEnabled)
		{
			if (m_ballStatus[STICKY_PADDLE].isEnabled)
			{
				m_ballStatus[STICKY_PADDLE].isEnabled = false;
				m_ballStatus[STICKY_PADDLE].periodInFrame = 0;
			}
			if (m_ballStatus[FROZEN_PADDLE].isEnabled)
			{
				m_ballStatus[FROZEN_PADDLE].isEnabled = false;
				m_ballStatus[FROZEN_PADDLE].periodInFrame = 0;
			}
			m_ballStatus[type].isEnabled = true;
			m_ballStatus[type].periodInFrame = 20.0f;
		}
		else
		{
			m_ballStatus[type].periodInFrame += 20.0f;
		}
		break;

	case WEAK_BALL:
		if (!m_ballStatus[type].isEnabled)
		{
			if (m_ballStatus[EXPLOSIVE_BALL].isEnabled)
			{
				m_ballStatus[EXPLOSIVE_BALL].isEnabled = false;
				m_ballStatus[EXPLOSIVE_BALL].periodInFrame = 0;
			}
			if (m_ballStatus[ENERGY_BALL].isEnabled)
			{
				m_ballStatus[ENERGY_BALL].isEnabled = false;
				m_ballStatus[ENERGY_BALL].periodInFrame = 0;
			}
			m_ballStatus[type].isEnabled = true;
			m_ballStatus[type].periodInFrame = 5.0f;
		}
		else
		{
			m_ballStatus[type].periodInFrame += 5.0f;
		}
		break;

	case RESET:
		resetAllBallStatus();
		break;

	case TIME_ADD:
		for (unsigned int i = 0; i < sizeof(m_ballStatus) / sizeof(*m_ballStatus); i++)
		{
			if (m_ballStatus[i].isEnabled)
			{
				m_ballStatus[i].periodInFrame += 7.0f;
			}
		}
		break;

	case RANDOM_EXTRA:
		int i = sizeof(m_ballStatus) / sizeof(*m_ballStatus);
		setGameStatus(BonusType(rand() % i));
		break;

	}
}

void GameDirector::resetAllBallStatus()
{
	for (unsigned int i = 0; i < sizeof(m_ballStatus) / sizeof(*m_ballStatus); i++)
	{
		m_ballStatus[i].isEnabled = false;
		m_ballStatus[i].periodInFrame = 0;

	}
}

void GameDirector::addBall(Ball* ball)
{
	if (m_arrayBalls)
	{
		m_arrayBalls->addObject(ball);
	}
}

void GameDirector::addBrick(Brick* brick)
{
	if (m_arrayBricks)
	{
		m_arrayBricks->addObject(brick);
	}
}

void GameDirector::addExtra(Extra* extra)
{
	if (m_arrayExtras)
	{
		m_arrayExtras->addObject(extra);
	}
}

void GameDirector::removeBall(Ball* ball)
{
	if (m_arrayBalls)
		m_arrayBalls->removeObject(ball);
}

void GameDirector::removeBrick(Brick* brick)
{
	if (m_arrayBricks)
	{
		m_arrayBricks->removeObject(brick);
	}
}

void GameDirector::removeExtra(Extra* extra)
{
	if (m_arrayExtras)
		m_arrayExtras->removeObject(extra);
}

void GameDirector::addPaddle(Paddle* paddle)
{
	m_paddle = paddle;
}

void GameDirector::addFloor(Floor* floor)
{
	m_floor = floor;
}