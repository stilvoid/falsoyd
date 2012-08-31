#include "shot.h"

#include <iostream>

Shot::Shot(SDL_Surface *s,int pw,int ph)
{
	if(World::audio_enabled == true)
	{
		shot_sound=sound_chunk_load("shot.wav");
		alien_shot=sound_chunk_load("alien_shot.wav");
		bomb_sound=sound_chunk_load("bomb.wav");
	}

	play_w=pw;
	play_h=ph;

	screen=s;

	char temps[100];
	sprintf(temps,"%s/gfx/bullet/player.bmp",PREFIX);
	pimage=SDL_LoadBMP(temps);
	SDL_SetColorKey(pimage,SDL_SRCCOLORKEY|SDL_RLEACCEL,
		SDL_MapRGB(pimage->format,0xff,0xff,0xff));

	sprintf(temps,"%s/gfx/bullet/alien.bmp",PREFIX);
	aimage=SDL_LoadBMP(temps);
	SDL_SetColorKey(aimage,SDL_SRCCOLORKEY|SDL_RLEACCEL,
			SDL_MapRGB(aimage->format,0xff,0xff,0xff));

	for(int i=0;i<World::num_bullets;i++)
	{
		back[i]=SDL_CreateRGBSurface(SDL_SWSURFACE,pimage->w,pimage->h,16,0,0,0,0);
	}

	for(int i=0;i<World::num_bullets;i++)
	{
		lives[i]=0;
		direction[i]=0;
		x_loc[i]=0;
		y_loc[i]=0;
		speed[i]=10;
	}
}

void Shot::init(void)
{
	for(int i=0;i<World::num_bullets;i++)
	{
		lives[i]=0;
	}
}

void Shot::fire(int dir,SDL_Rect pos,char* who)
{
	bool found=0;

	for(int i=0;i<World::num_bullets;i++)
	{
		if(lives[i] == 0 && found==0)
		{
			lives[i]=50;

			x_loc[i]=pos.x-pimage->w/2;
			y_loc[i]=pos.y-pimage->h/2;
			loc[i].x=(int)x_loc[i];
			loc[i].y=(int)y_loc[i];

			direction[i]=dir;

			owner[i]=who;

			if(World::audio_enabled == true)
			{
				if(strcmp(who,"player")==0 || strcmp(who,"wave")==0){sound_play(shot_sound);}
			}
			if(strcmp(who,"firstbomb")==0)
			{
				if(World::audio_enabled == true)
				{
					sound_play(bomb_sound);
				}
				owner[i]="bomb";
				lives[i]=640;
			}
			if(strcmp(who,"bomb")==0){lives[i]=640;}
			if(strcmp(who,"mine")==0){lives[i]=1000;}
			
			found=1;
		}
	}
}

void Shot::move(float tf)
{
	for(int i=0;i<World::num_bullets;i++)
	{
		if(lives[i] > 0)
		{
			lives[i]--;

			if(strcmp(owner[i],"bomb")==0)
			{
				x_loc[i]+=0.5*speed[i]*World::XM[direction[i]]*World::YM[direction[i]]*World::XM[direction[i]]*tf;
				y_loc[i]+=0.5*speed[i]*World::YM[direction[i]]*World::XM[direction[i]]*World::YM[direction[i]]*tf;
				if((float)lives[i]/6==(int)(lives[i]/6))
				{
					direction[i]++;
				}
				if(direction[i]>=32){direction[i]=0;}
			}
			else if(strcmp(owner[i],"blade")==0)
			{
				x_loc[i]+=speed[i]*World::XM[direction[i]]*tf;
				y_loc[i]+=speed[i]*World::YM[direction[i]]*tf;
				if((float)lives[i]/8==(int)(lives[i]/8))
				{
					direction[i]--;
				}
				if(direction[i]<0){direction[i]=31;}
			}
			else if(strcmp(owner[i],"wave")==0)
			{
				if((float)lives[i]/2==(int)(lives[i]/2))
				{
					direction[i]+=(int)(rand()%3-1);
				}

				if(direction[i]<0){direction[i]=31;}
				if(direction[i]>=32){direction[i]=0;}

				x_loc[i]+=speed[i]*World::XM[direction[i]]*tf;
				y_loc[i]+=speed[i]*World::YM[direction[i]]*tf;
			}
			else
			{
				x_loc[i]+=speed[i]*World::XM[direction[i]]*tf;
				y_loc[i]+=speed[i]*World::YM[direction[i]]*tf;
			}

			while(x_loc[i] < 0){x_loc[i]+=play_w;}
			while(y_loc[i] < 0){y_loc[i]+=play_h;}
			while(x_loc[i] >= play_w){x_loc[i]-=play_w;}
			while(y_loc[i] >= play_h){y_loc[i]-=play_h;}

			//TRAP

			loc[i].x=(int)x_loc[i];
			loc[i].y=(int)y_loc[i];
		}
	}
}

void Shot::paste(void)
{
}

void Shot::draw(SDL_Rect vport)
{
	for(int i=0;i<World::num_bullets;i++)
	{
		if (lives[i] > 0)
		{
			//left
			if(x_loc[i]+pimage->w >= vport.x && x_loc[i] < play_w)
			{
				temp.x=(int)x_loc[i]-vport.x;
			}
			//right
			else if(x_loc[i] <= vport.w-(play_w-vport.x))
			{
				temp.x=(int)x_loc[i]+(play_w-vport.x);
			}
			//top
			if(y_loc[i]+pimage->h >= vport.y && y_loc[i] < play_h)
			{
				temp.y=(int)y_loc[i]-vport.y;
			}
			//bottom
			else if(y_loc[i] <= vport.h-(play_h-vport.y))
			{
				temp.y=(int)y_loc[i]+(play_h-vport.y);
			}

			temp.w=pimage->w;
			temp.h=pimage->h;
			
			if(on_map(vport,i))
			{
				if(strcmp(owner[i],"player")==0 || strcmp(owner[i],"wave")==0)
				{
					SDL_BlitSurface(pimage,NULL,screen,&temp);
				}
				else
				{
					SDL_BlitSurface(aimage,NULL,screen,&temp);
				}
			}
		}
	}
}

vector<SDL_Rect> Shot::get_locs(void)
{
	vector<SDL_Rect> locs;

	for(int i=0;i<World::num_bullets;i++)
	{
		if(lives[i] > 0)
		{
			locs.push_back(loc[i]);
		}
	}

	return locs;
}

int Shot::collide(SDL_Rect pos,char* who)
{
	int hits=0;

	pos.x=(int)(pos.x+(float)pos.w/10);
	pos.y=(int)(pos.y+(float)pos.h/10);
	pos.w=(int)(pos.w*0.8);
	pos.h=(int)(pos.w*0.8);

	for(int i=0;i<World::num_bullets;i++)
	{
		if(lives[i] > 0)
		{
			if(x_loc[i] > pos.x && x_loc[i] < pos.x+pos.w &&
					y_loc[i] > pos.y && y_loc[i] < pos.y+pos.h &&
					strcmp(who,owner[i]) != 0 &&
					!(strcmp(who,"player")==0 && strcmp(owner[i],"bomb")==0) &&
					!(strcmp(who,"player")==0 && strcmp(owner[i],"blade")==0) &&
					!(strcmp(who,"player")==0 && strcmp(owner[i],"wave")==0))
			{
				lives[i]=0;
				hits++;
				if(strcmp(owner[i],"bomb")==0){hits+=99;}
			}
		}
	}
	return hits;
}

bool Shot::on_map(SDL_Rect onmap,int i)
{
	bool on_x=0;
	bool on_y=0;

	if((x_loc[i]+pimage->w >= onmap.x && x_loc[i] < play_w) ||
			(x_loc[i] <= onmap.w-(play_w-onmap.x)))
	{
		on_x=1;
	}
	if((y_loc[i]+pimage->h >= onmap.y && y_loc[i] < play_h) ||
			(y_loc[i] <= onmap.h-(play_h-onmap.y)))
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
