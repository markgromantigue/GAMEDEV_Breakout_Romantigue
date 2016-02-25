#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "constant.h"
#include <algorithm>
#include <vector>
#include <string>

using namespace std;
using namespace cocos2d;

const EventKeyboard::KeyCode LEFT_ARROW = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
const EventKeyboard::KeyCode RIGHT_ARROW = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;

class Ball;
class Paddle;
class Brick;

USING_NS_CC;

class GameScene : public CCScene
{
public:
	GameScene();
	virtual void runThisTest();

	virtual void menuCallback(CCObject* pSender);
};

class GameAreaDef
{
public:
	static CCRect getFingerTouchRect();
	static CCRect getBallMoveRect();
	static CCRect getAdDisplayRect();
	static CCRect getScoreRect();

	static float getGameAreaLeftMost();
	static float getGameAreaRightMost();

private:
	static void lazyInit();
	static CCRect touchRect;
	static CCRect BallMoveRect;
	static CCRect AdDisplayRect;
	static CCRect ScoreRect;
};

class Brick : public CCSprite
{
public:
	Brick();
	~Brick();


	void setBrickStatu(BrickType type);

	static Brick* createBrick(CCTexture2D* aTexture);
	CCRect rect();

	bool collideWithBall(Ball* ball);
	void brickCrashedByBall(Ball* ball);

	void callbackRemoveBrick();

	inline void setBonusType(BonusType type) { m_bonusType = type; };
	inline BonusType getBonusType() { return m_bonusType; };

	inline BrickType getBrickType() { return m_brickType; };
private:
	BrickType m_brickType;
	BonusType m_bonusType;
};

typedef enum tagPaddleState
{
	kPaddleStateGrabbed,
	kPaddleStateUngrabbed
} PaddleState;

class Paddle : public CCSprite
{
	PaddleState        m_state;
	int leftMost, rightMost;
	int m_paddleLength;

public:
	Paddle(void);
	virtual ~Paddle(void);

	CCRect rect();
	bool initWithTexture(CCTexture2D* aTexture);
	virtual void onEnter();
	virtual void onExit();
	bool containsTouchLocation(CCTouch* touch);
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(EventKeyboard::KeyCode k, Event * e);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	virtual CCObject* copyWithZone(CCZone *pZone);

	virtual void touchDelegateRetain();
	virtual void touchDelegateRelease();

	static Paddle* paddleWithTexture(CCTexture2D* aTexture);
};

class Floor : public CCSprite
{
	PaddleState        m_state;
	int leftMost, rightMost;
	int m_floorLength;

public:
	Floor(void);
	virtual ~Floor(void);

	CCRect rect();
	bool initWithTexture(CCTexture2D* aTexture);
	virtual CCObject* copyWithZone(CCZone *pZone);

	static Floor* floorWithTexture(CCTexture2D* aTexture);
};

class Extra : public CCSprite
{
	CCSprite* m_shadow; //shadow follows the extra sprite
	CCPoint m_velocity;
	BonusType m_bonus;	//bonus type associated with this extra

public:
	Extra();
	~Extra();

	void setBonusType(BonusType type);
	inline BonusType getBonusType() { return m_bonus; };

	//virtual void draw(void);

	static Extra* createExtra(CCTexture2D* aTexture);
	void collidewithPaddle(Paddle* paddle);
	void collidewithFloor(Floor* floor);
	void move(float delta);
};

class Ball : public CCSprite
{
	CCPoint m_velocity;
	CCSprite* m_shadow; //shadow follows the ball

public:
	Ball(void);
	virtual ~Ball(void);

	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

	float radius();
	//BOOL initWithTexture(CCTexture2D* aTexture);
	//virtual void setTexture(CCTexture2D* newTexture);
	void move(float delta);
	void collideWithPaddle(Paddle* paddle);
	void collideWithFloor(Floor* floor);
	void setBallColor(BallColor color);


public:
	inline void setVelocity(CCPoint velocity) { m_velocity = velocity; }
	inline CCPoint getVelocity() { return m_velocity; }

public:
	static Ball* ballWithTexture(CCTexture2D* aTexture, CCRect rect);
};



class GameLayer : public CCLayer
{
private:
	Ball* m_ball;
	Paddle* m_paddle;
	Floor* m_floor;
	Brick* m_brick;
	CCPoint m_ballVelocity;

public:
	GameLayer();
	~GameLayer();
	vector<EventKeyboard::KeyCode> heldKeys;

	void addBrick(CCPoint point);
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void update(float delta);
};

class GameDirector
{
public:
	GameDirector();
	virtual ~GameDirector();

	void init();
	void logicUpdate(float delta);
	void setGameStatus(BonusType bonusType);
	void resetAllBallStatus();

	void addBall(Ball* ball);
	void addBrick(Brick* brick);
	void addExtra(Extra* extra);
	void addPaddle(Paddle* paddle);
	void addFloor(Floor* floor);

	void removeBall(Ball* ball);
	void removeBrick(Brick* brick);
	void removeExtra(Extra* extra);

	inline BallStatus* getBallStatus(BonusType bonusType) { return &m_ballStatus[bonusType]; };

	inline void addGameScore(int score) { m_gameScore += score; };
	inline void resetGameScore() { m_gameScore = 0; };
	inline unsigned int getGameScore() { return m_gameScore; };

	inline CCArray* getArrayBalls() { return m_arrayBalls; };
	inline CCArray* getArrayBricks() { return m_arrayBricks; };
	inline CCArray* getArrayExtras() { return m_arrayExtras; };

	static GameDirector* sharedGameDirector();
private:
	BallStatus m_ballStatus[BONUS_COUNT];
	unsigned int m_gameScore;
	CCArray* m_arrayBalls;
	CCArray* m_arrayBricks;
	CCArray* m_arrayExtras;
	Paddle* m_paddle;
	Floor* m_floor;


};

#endif