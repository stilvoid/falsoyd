#include "bonus.h"

#include "world.h"

Bonus::Bonus(SDL_Surface *buffer, int pw, int ph)
{
	play_w = pw;
	play_h = ph;

	playfield=buffer;
	char tempfile[100];

	if(World::audio_enabled == true)
	{
		spawn_sound=sound_chunk_load("bonus_spawn.wav");
		death_sound=sound_chunk_load("bonus_death.wav");
	}

	for(int i=0;i<World::bonus_types;i++)
	{
		sprintf(tempfile,"%s/gfx/bonus/%d.bmp",PREFIX,i);
		image.push_back(SDL_LoadBMP(tempfile));
		SDL_SetColorKey(image[i],SDL_SRCCOLORKEY|SDL_RLEACCEL,
			SDL_MapRGB(image[i]->format,0x00,0x00,0x00));
	}

}

void Bonus::init(void)
{
	back=SDL_CreateRGBSurface(SDL_SWSURFACE,image[0]->w,image[0]->h,16,0,0,0,0);

	loc.x=10;
	loc.y=10;

	SDL_BlitSurface(playfield,&loc,back,NULL);

	bonus_type=0;

	lives=0;
}

void Bonus::move(void)
{
	lives--;
	if(lives > 0)
	{
		SDL_BlitSurface(playfield,&loc,back,NULL);
	}
	else
	{
		if(lives < -20-rand()%30)
		{
			lives=1000;
			loc.x=rand()%(playfield->w-image[0]->w);
			loc.y=rand()%(playfield->h-image[0]->h);
			bonus_type=(rand()%World::bonus_types)+1;
			if(World::audio_enabled == true)
			{
				sound_chunk_free(death_sound);
				sound_play(spawn_sound);
				if(bonus_type==World::HEALTH)
				{
					death_sound=sound_chunk_load("bonus.wav");
				}
				else if(bonus_type==World::SPEED)
				{
					death_sound=sound_chunk_load("bonus.wav");
				}
				else if(bonus_type==World::TURN)
				{
					death_sound=sound_chunk_load("bonus.wav");
				}
				else if(bonus_type==World::SHOOT)
				{
					death_sound=sound_chunk_load("bonus.wav");
				}
				else if(bonus_type==World::SLOW)
				{
					death_sound=sound_chunk_load("bonus.wav");
				}
				else if(bonus_type==World::RAND)
				{
					death_sound=sound_chunk_load("haha.wav");
				}
				else if(bonus_type==World::REVERSE)
				{
					death_sound=sound_chunk_load("haha.wav");
				}
				else if(bonus_type==World::WEAPON)
				{
					death_sound=sound_chunk_load("weapon.wav");
				}
				else if(bonus_type==World::STATIC)
				{
					death_sound=sound_chunk_load("haha.wav");
				}
				else if(bonus_type==World::INVINCIBLE)
				{
					death_sound=sound_chunk_load("weapon.wav");
				}
			}
		}
	}
}

void Bonus::draw(SDL_Rect vport)
{
	if (lives > 0)
	{
		SDL_Rect temp;

		//Drawing bit (nicked from sprite routine)
		{
			//left
			if(loc.x+image[0]->w >= vport.x && loc.x < play_w)
			{
				temp.x=(int)loc.x-vport.x;
			}
			//right
			else if(loc.x <= vport.w-(play_w-vport.x))
			{
				temp.x=(int)loc.x+(play_w-vport.x);
			}
			//top
			if(loc.y+image[0]->h >= vport.y && loc.y < play_h)
			{
				temp.y=(int)loc.y-vport.y;
			}
			//bottom
			else if(loc.y <= vport.h-(play_h-vport.y))
			{
				temp.y=(int)loc.y+(play_h-vport.y);
			}

			if(on_map(vport))
			{
				SDL_BlitSurface(image[bonus_type-1],NULL,playfield,&temp);
			}
		}
	}
}

void Bonus::hit(int i)
{
	lives-=i;
	if (lives<=0)
	{
		die();
	}
}

SDL_Rect Bonus::get_loc(void)
{
	return loc;
}

void Bonus::die(void)
{
	loc.x=playfield->w+back->w*2;
	loc.y=playfield->h+back->h*2;
	if(World::audio_enabled == true)
	{
		sound_play(death_sound);
	}
}

int Bonus::get_lives(void)
{
	return lives;
}

int Bonus::collide(SDL_Rect pos)
{
	int value=0;
	int x=loc.x+loc.w/2;
	int y=loc.y+loc.h/2;

	if(lives > 0)
	{
		if(x > pos.x && x < pos.x+pos.w &&
				y > pos.y && y < pos.y+pos.h)
		{
			hit(get_lives());
			value=bonus_type;
			if(World::audio_enabled == true)
			{	
				sound_play(death_sound);
			}
			bonus_type=0;
		}
	}

	return value;
}

bool Bonus::on_map(SDL_Rect onmap)
{
	bool on_x=0;
	bool on_y=0;

	if((loc.x+image[0]->w >= onmap.x && loc.x < play_w) ||
			(loc.x <= onmap.w-(play_w-onmap.x)))
	{
		on_x=1;
	}
	if((loc.y+image[0]->h >= onmap.y && loc.y < play_h) ||
			(loc.y <= onmap.h-(play_h-onmap.y)))
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
