#include "alien.h"

#include <iostream>

#include "world.h"

Alien::Alien(SDL_Surface *s,int pw,int ph,Shot* bulls,vector<SDL_Surface*> &blocks):Sprite(s,pw,ph,blocks)
{
	bullets=bulls;
	play_w=pw;
	play_h=ph;
}

void Alien::init(SDL_Rect pos,int l,int num,vector<SDL_Surface*> &blocks,int move,int s)
{
	if(World::audio_enabled == true)
	{
		death_sound=sound_chunk_load("alien_death.wav");
		hit_sound=sound_chunk_load("alien_hit.wav");
	}
	image=blocks;

	back=SDL_CreateRGBSurface(SDL_SWSURFACE,image[0]->w,image[0]->h,16,0,0,0,0);

	frame=0;
	maxframe=num;

	if(move==World::ROCK)
	{
		frame=rand()%maxframe;
	}

	x_loc=pos.x;
	y_loc=pos.y;

	SDL_Rect loc;
	loc.x=pos.x;
	loc.y=pos.y;

	move_type=move;
	speed=s+3;

	lives=l;
	inertia=25;
	direction=frame;
	turnspeed=30-speed;
	turncount=0;
	shoot_speed=50-speed;
	shoot_count=0;

	//Correction for bizarre error!
	bound.w-=image[0]->w/4;
	bound.h-=image[0]->h/4;

	dying=0;
}

void Alien::move(SDL_Rect pos,float tf)
{
	using namespace std;

	int x=0;
	int y=0;
	int y2=0;
	int thrust=0;
	bool shoot=0;
	int shoot_dir=0;

	dir=0;

	if(lives>0)
	{

		x=(int)(pos.x-x_loc);
		if(x>play_w/2){x=-(x-play_w/2);}
		if(x<play_w/-2){x=-(x+play_w/2);}

		y=(int)(y_loc-pos.y);
		if(y>play_h/2){y=-(y-play_h/2);}
		if(y<play_h/-2){y=-(y+play_h/2);}

		y2=(int)(pos.y-y_loc);
		if(y2>play_h/2){y2=-(y2-play_h/2);}
		if(y2<play_h/-2){y2=-(y2+play_h/2);}

		if( sqrt((float)((x*x)+(y2*y2))) < 500 )
		{
			in_range=1;
		}
		else
		{
			in_range=0;
		}

		if(y>0 && y>abs(x*2)) {shoot_dir=0;}
		else if(x>0 && x>abs(y*2)) {shoot_dir=2;}
		else if(y<0 && -y>abs(x*2)) {shoot_dir=4;}
		else if(x<0 && -x>abs(y*2)) {shoot_dir=6;}
		else if(x>0 && y>0) {shoot_dir=1;}
		else if(x>0 && y<0) {shoot_dir=3;}
		else if(x<0 && y<0) {shoot_dir=5;}
		else if(x<0 && y>0) {shoot_dir=7;}
		else {shoot_dir=0;}

		if(move_type==World::RANDOM)
		{
			dir=(rand()%3)-1;
			thrust=rand()%speed;
			if(rand()%200 < 2 && in_range)
			{
				shoot=1;
			}
			else
			{
				shoot=0;
			}
		}
		else if(move_type==World::ROCK)
		{
			dir=0;
			shoot=0;
			thrust=speed;
		}
		else if(move_type==World::FOLLOW)
		{
			dir=int(direction-shoot_dir);
			if(dir<0){dir+=8;}
			if(dir>0 && dir<5){dir=-1;}
			else if(dir>=5 && dir<8){dir=1;}

			thrust=rand()%speed;

			if(rand()%200 < 2 && in_range)
			{
				shoot=1;
			}
			else
			{
				shoot=0;
			}

		}
		else if(move_type==World::FOLLOW2)
		{
			dir=int(direction-shoot_dir);
			if(dir<0){dir+=8;}
			if(dir>0 && dir<5){dir=-1;}
			else if(dir>=5 && dir<8){dir=1;}

			thrust=rand()%speed;

			shoot=0;
		}
		else if(move_type==World::STILL)
		{
			dir=direction-shoot_dir;
			if(dir<0){dir+=8;}
			if(dir>0 && dir<5){dir=-1;}
			else if(dir>=5 && dir<8){dir=1;}

			thrust=0;

			if(in_range==1)
			{
				shoot=1;
			}
			else
			{
				shoot=0;
			}
		}
		else if(move_type==World::AVOID)
		{
			if( sqrt(((pos.x-x_loc)*(pos.x-x_loc))+((pos.y-y_loc)*(pos.y-y_loc))) < 400 )
			{
				dir=direction-shoot_dir;
				if(dir<0){dir+=8;}
				if(dir>0 && dir<5){dir=1;}
				else if(dir>4 && dir<8){dir-=1;}
				else{dir=0;}


				thrust=(rand()%speed)+2;

				shoot=0;
			}
			else
			{
				dir=0;
				thrust=0;
				shoot=0;
			}
		}
		else if(move_type==World::KAMIKAZE)
		{
			if( sqrt(((pos.x-x_loc)*(pos.x-x_loc))+((pos.y-y_loc)*(pos.y-y_loc))) < 300 )
			{
				dir=0;
				direction=shoot_dir;

				thrust=(rand()%speed)+3;

				shoot=0;
			}
			else
			{
				dir=direction-shoot_dir;
				if(dir<0){dir+=8;}
				if(dir>0 && dir<5){dir=-1;}
				else if(dir>4 && dir<8){dir=1;}
				else{dir=0;}

				thrust=0;
				if(in_range)
				{
					shoot=1;
				}
			}
		}

		if(turncount>0)
		{
			turncount--;
			dir=0;
		}
		else
		{
			turncount=(int)turnspeed;
		}

		if(inertia_count>0)
		{
			thrust=0;
			dir=0;
			inertia_count--;
		}

		direction+=dir;

		if(direction<0){direction+=maxframe;}
		if(direction>=maxframe){direction-=maxframe;}

		frame=direction;

		x_loc+=thrust*World::XM[direction*4]*tf;
		y_loc+=thrust*World::YM[direction*4]*tf;

		while(x_loc < 0) {x_loc+=play_w;}
		while(y_loc < 0) {y_loc+=play_h;}
		while(x_loc > play_w) {x_loc-=play_w;}
		while(y_loc > play_h) {y_loc-=play_h;}

		loc.x=(int)x_loc;
		loc.y=(int)y_loc;

		if(shoot && shoot_count<=0)
		{
			if(direction==0)
			{loc.x+=image[0]->w/2;}
			if(direction==1)
			{loc.x+=image[0]->w;}
			if(direction==2)
			{loc.x+=image[0]->w;loc.y+=image[0]->h/2;}
			if(direction==3)
			{loc.x+=image[0]->w;loc.y+=image[0]->h;}
			if(direction==4)
			{loc.x+=image[0]->w/2;loc.y+=image[0]->h;}
			if(direction==5)
			{loc.y+=image[0]->h;}
			if(direction==6)
			{loc.y+=image[0]->h/2;}
			if(direction==7)
			{}

			bullets->fire(shoot_dir*4,loc,"alien");
			shoot_count=shoot_speed;
		}
		else
		{
			shoot_count--;
		}
	}
}
