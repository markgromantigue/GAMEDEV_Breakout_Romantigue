/*
 * constant.h
 *
 *  Created on: Sep 23, 2013
 *      Author: ADIC
 */

#ifndef CONSTANT_H_
#define CONSTANT_H_

#include <string>


//scale factor for sprite. original LBreakout2 sprites (including ball and paddle) are too small to display in the device
const float SCALE_FACTOR = 2.0f;

//scale factor for brick/bouns
const float BRICK_SCALE_FACTOR = 1.5f;

static std::string HomeScreenMenuNames[] ={
		"Start",
};

enum HomeScreenMenuButton
{
	SCENE_GAME = 0,
	SCENE_HELP,
	SCENE_SETTING,
	SCENE_ABOUT,
};

static std::string GameMenuNames[] = {
		"Start",
};

enum GameMenuButton
{
	GAME_MENU_START = 0,
	GAME_MENU_LEVELS,
	GAME_MENU_DIFFICULTY,
	GAME_MENU_PLAYER,
};

enum BallColor
{
	GREEN_BALL = 0,
	BLUE_BALL,
	RED_BALL,
	DEEPGREY_BALL,
	GREY_BALL,
};


enum PaddleType
{
	PADDLE_NORMAL = 0,
	PADDLE_ATTACH,
	PADDLE_FROZEN,
	PADDLE_DISABLED,
};

enum BrickType
{
	BRICK_SHIELDED = 0,
	BRICK_METAL,
	BRICK_CHAOTIC,
	BRICK_STRONG_1,
	BRICK_STRONG_2,
	BRICK_STRONG_3,
	BRICK_SHADOW,
	BRICK_REGENERATING_1,
	BRICK_REGENERATING_2,
	BRICK_REGENERATING_3,
	BRICK_BROWN,
	BRICK_ORANGE,
	BRICK_GREEN,
	BRICK_BLUE,
	BRICK_YELLOW,
	BRICK_PINK,
	BRICK_WHITE,
	BRICK_GREY,
	BRICK_TNT,
	BRICK_GROWING,
	BRICK_STRONG_4,
	BRICK_INVISIBLE,
};

enum BonusType //bonus or malus
{
	SCORE_200 = 0,
	SCORE_500,
	SCORE_1000,
	SCORE_2000,
	SCORE_5000,
	SCORE_10K,
	END_OF_RAINBOW, //bonus: if you destroy a brick with no bonus assigned it will release 1000 points extra score
	SHRINK_PADDLE,	//malus: shrinks your paddle about one paddle title (18 pixels)
	EXPAND_PADDLE,	//bonus: expands your paddle about one title (the maximum size depends on the chosen difficulty level)
	EXTRA_LIFE,		//bonus: gives you one extra life (paddle)
	STICKY_PADDLE,	//bonus: balls touching the paddle will keep attached until you press fire
	ENERGY_BALL,	//bonus: balls won't be reflected at a brick but go directly through it
	EXTRA_BALL,		//bonus: creates a new ball
	BONUS_FLOOR,	//bonus: closes the screen so no balls can get lost
	FROZEN_PADDLE,	//malus: freezes the paddle (no movement possible)
	WEAPON,			//bonus: adds a weapon to the paddle
	RANDOM_EXTRA,	//gives you any of the listed extras
	SPEED_UP,		//malus: accelerates all balls to maximum speed
	SPEED_DOWN,		//bonus: decelerate all balls to minimum speed
	JOKER,			//bonus: collects all bonus on screen instantly and destroy all malus. doubles score and time of collected bonus
	LIGHTS_OUT,		//malus: total darkness. all you see is a shade of paddle, balls and extras
	CHAOS,			//malus: balls are reflected randomly at bricks (but not at paddle)
	GHOSTLY_PADDLE,	//malus: paddle disappears when not moving and won't reflect any balls then (nor does it collect bonuses)
	RESET,			//all active bonuses and maluses will be reset
	TIME_ADD,		//all active bonuses and maluses will last seven seconds longer
	EXPLOSIVE_BALL,	//bonus: when hitting a brick balls will causing an explosion destroying all surrounding bricks
	BONUS_MAGNET,	//bonus: while this bonus is active the paddle will attract other bonuses
	MALUS_MAGNET,	//malus: while this malus is active the paddle will attract other maluses
	WEAK_BALL,		//malus: with this malus active there is a 40% chance that a ball won't damage a brick
	BONUS_COUNT,
};

struct BallStatus
{
	bool isEnabled;
	float periodInFrame;
};


#endif /* CONSTANT_H_ */
