#ifndef SHOT_H
#define SHOT_H

#include "SDL.h"
#include "math.h"
#include <vector>

#include "audio.h"

#include "world.h"

#include <stdio.h>

using namespace std;

class Shot
{
	private:
		SDL_Surface *aimage;		//Alien bullet
		SDL_Surface *pimage;		//Player bullet
		SDL_Surface *back[World::num_bullets];	//Backing store for blits
		SDL_Surface *screen;		//Where to blit it
		int play_w;
		int play_h;
		float x_loc[World::num_bullets];
		float y_loc[World::num_bullets];
		char* owner[World::num_bullets];
		SDL_Rect bound;			//Where are we allowed to go?
		SDL_Rect loc[World::num_bullets];
		SDL_Rect temp;
		int lives[World::num_bullets];		//Obvious!
		int direction[World::num_bullets];	//Direction of travel
		int speed[World::num_bullets];
		Sound_Chunk *shot_sound;
		Sound_Chunk *alien_shot;
		Sound_Chunk *bomb_sound;
	public:
		Shot(SDL_Surface *s,int pw,int ph);
		void fire(int dir,SDL_Rect pos,char* who);
		void move(float tf);
		void paste(void);
		void draw(SDL_Rect vport);
		void init(void);
		int collide(SDL_Rect pos,char* who);
		vector<SDL_Rect> get_locs(void);
		bool on_map(SDL_Rect onmap, int i);
};

#endif
