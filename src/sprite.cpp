#include "sprite.h"

#include "world.h"

using namespace std;

Sprite::Sprite(SDL_Surface *s,int pw,int ph,vector<SDL_Surface*> &blocks)
{
	play_w=pw;
	play_h=ph;

	screen=s;

	image=blocks;

	back=SDL_CreateRGBSurface(SDL_SWSURFACE,image[0]->w,image[0]->h,16,0,0,0,0);

	bound.x=0;
	bound.y=0;
	bound.w=play_w-image[0]->w;
	bound.h=play_h-image[0]->h;
	invincible_count=0;
	inertia_count=0;
	dying=0;
}

void Sprite::stop(void)
{
	//thrust=0;
}

void Sprite::set_loc(SDL_Rect pos)
{
	x_loc=pos.x;
	y_loc=pos.y;
	//SDL_BlitSurface(playfield,&pos,back,NULL);
}

void Sprite::draw(SDL_Rect vport)
{
	if (lives > 0 || dying>0)
	{
		if(dying>0)
		{
			for(int i=0;i<(signed)image.size();i++)
			{
				SDL_SetAlpha(image[i],SDL_SRCALPHA|SDL_RLEACCEL,dying);
			}
		}

		SDL_Rect temp;

		//Drawing bit
		{
			//left
			if(x_loc+image[0]->w >= vport.x && x_loc < play_w)
			{
				temp.x=(int)x_loc-vport.x;
			}
			//right
			else if(x_loc <= vport.w-(play_w-vport.x))
			{
				temp.x=(int)x_loc+(play_w-vport.x);
			}
			//top
			if(y_loc+image[0]->h >= vport.y && y_loc < play_h)
			{
				temp.y=(int)y_loc-vport.y;
			}
			//bottom
			else if(y_loc <= vport.h-(play_h-vport.y))
			{
				temp.y=(int)y_loc+(play_h-vport.y);
			}

			if(on_map(vport))
			{
				SDL_BlitSurface(image[frame],NULL,screen,&temp);
			}
		}

		loc.x=(int)x_loc;
		loc.y=(int)y_loc;
		
		if(dying>0)
		{
			for(int i=0;i<(signed)image.size();i++)
			{
				SDL_SetAlpha(image[i],SDL_SRCALPHA|SDL_RLEACCEL,SDL_ALPHA_OPAQUE);
			}
			dying-=8;
		}
	}
}

void Sprite::hit(int i)
{
	if(invincible_count<=0)
	{
		lives-=i;
		if (lives<=0)
		{
			dying=256;
			die();
		}
		else if(i>0)
		{
			if(World::audio_enabled == true)
			{
				sound_play(hit_sound);
			}
			inertia_count=30;
		}
	}
}

SDL_Rect Sprite::get_loc(void)
{
	loc.x=(int)x_loc;
	loc.y=(int)y_loc;
	loc.w=image[0]->w;
	loc.h=image[0]->h;
	return loc;
}

void Sprite::die(void)
{
	lives=0;
	dying=256;
	if(World::audio_enabled == true)
	{
		sound_play(death_sound);
	}
}

int Sprite::get_lives(void)
{
	return lives;
}

bool Sprite::on_map(SDL_Rect onmap)
{
	bool on_x=0;
	bool on_y=0;

	if((x_loc+image[0]->w >= onmap.x && x_loc < play_w) ||
			(x_loc <= onmap.w-(play_w-onmap.x)))
	{
		on_x=1;
	}
	if((y_loc+image[0]->h >= onmap.y && y_loc < play_h) ||
			(y_loc <= onmap.h-(play_h-onmap.y)))
	{
		on_y=1;

	}

	if(on_x && on_y)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
