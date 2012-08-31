#ifndef SPRITE_H
#define SPRITE_H

#include "SDL.h"
#include "math.h"
#include <vector>
#include "audio.h"
#include <stdio.h>

#include "shot.h"

class Sprite
{
	protected:
		std::vector<SDL_Surface*> image;	//The sprite image
		SDL_Surface *back;		//Backing store for blits
		SDL_Surface *screen;
		int play_w;
		int play_h;
		float x_loc;
		float y_loc;
		SDL_Rect bound;			//Where are we allowed to go?
		SDL_Rect loc;			//temporary storage
		int frame;			//Which anim frame are we on?
		int maxframe;			//How many frames do we have?
		int direction;			//And which direction to face?
		int speed;			//Duh!
		int lives;			//Obvious!
		float turnspeed;			//Turning speed
		int turncount;			//Make sure we don't turn too quickly
		int inertia;
		int inertia_count;
		int invincible_count;
		void die(void);
		//Shot* bullets;
		Sound_Chunk *death_sound;
		Sound_Chunk *hit_sound;
		int dying;
	public:
		Sprite(SDL_Surface *s,int pw,int ph,std::vector<SDL_Surface*> &blocks);
		void stop(void);
		void paste(SDL_Rect vport);
		void draw(SDL_Rect vport);
		SDL_Rect get_loc(void);
		void set_loc(SDL_Rect pos);
		void hit(int i);
		int get_lives(void);
		bool on_map(SDL_Rect onmap);
};

#endif
