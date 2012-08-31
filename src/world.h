#ifndef WORLD_H
#define WORLD_H

#include "config.h"
#include <math.h>

#define VERSION		"0.9"

class World
{
	public:
	static const float S;
	static const float L;
	static const float M;

	static const int num_bullets = 100;
	static const int num_aliens = 300;

	static const int swodads = 8;

	static bool audio_enabled;
	static bool music_enabled;

	enum movements
	{
		RANDOM,
		FOLLOW,
		STILL,
		AVOID,
		KAMIKAZE,
		FOLLOW2,
		ROCK
	};

	enum bonuses
	{
		SPEED,
		TURN,
		SHOOT,
		HEALTH,
		SLOW,
		RAND,
		REVERSE,
		WEAPON,
		STATIC,
		INVINCIBLE
	};

	static const int bonus_types = 10;
	
	static const float XM[32];
	static const float YM[32];
};

#endif
