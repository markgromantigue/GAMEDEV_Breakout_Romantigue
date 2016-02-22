#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "constant.h"

USING_NS_CC;

class GameScene : public CCScene
{
public:
	GameScene();
	virtual void runThisTest();

	virtual void menuCallback(CCObject* pSender);
};



class GameLayer : public CCLayer
{
private:
	Ball* m_ball;
	Paddle* m_paddle;
	Brick* m_brick;
	CCPoint m_ballVelocity;

public:
	GameLayer();
	~GameLayer();

	void addBrick(CCPoint point);

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


};

#endif