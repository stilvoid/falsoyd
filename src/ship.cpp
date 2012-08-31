#include <time.h>

#include "ship.h"

Ship::Ship(SDL_Surface *s,int pw,int ph,Shot* bulls,vector<SDL_Surface*> &blocks):Sprite(s,pw,ph,blocks)
{
	if(World::audio_enabled == true)
	{
		death_sound=sound_chunk_load("player_death.wav");
		hit_sound=sound_chunk_load("player_hit.wav");
	}
	bullets=bulls;

	death_max=1000;

	turnspeed=0;
	shoot_speed=0;
	thrust_speed=0;
	max_lives=10;
	lives=0;
#ifndef WIN32
	srand(time(NULL));
#endif
	weapon=rand()%World::swodads;

	play_w=pw;
	play_h=ph;
}

void Ship::init(SDL_Rect pos,int l,int num)
{
	weapon=rand()%World::swodads;

	if(l==1)
	{
		turnspeed=2;
		max_shoot_speed=10;
		thrust_speed=2;
		lives=max_lives;
		weapon=rand()%World::swodads;
	}

	frame=0;
	maxframe=num;

	x_loc=pos.x;
	y_loc=pos.y;

	SDL_Rect loc;
	loc.x=pos.x;
	loc.y=pos.y;

	death_count=death_max;
	death_on=0;

	if(lives<=0)
	{
		max_lives--;
		lives=max_lives;
	}

	inertia=50;
	inertia_count=0;
	direction=frame;
	speed=0;
	shoot_count=0;

	reverse_count=0;

	x_vel=0;
	y_vel=0;

	dying=0;
	
	shoot_speed = max_shoot_speed;
}

void Ship::move(int mx,int my,bool thrusting,bool shoot,bool death,float tf)
{
	invincible_count--;

	if(invincible_count==0)
	{
		for(int i=0;i<(signed)image.size();i++)
		{
			SDL_SetAlpha(image[i],SDL_SRCALPHA|SDL_RLEACCEL,255);
		}
	}

	if(shoot)
	{
		if(shoot_count==0)
		{
			shoot_speed*=1.1;
		}
	}
	else
	{
		shoot_speed--;
		if(shoot_speed < max_shoot_speed)
		{
			shoot_speed=max_shoot_speed;
		}
	}

	mx-=image[0]->w/2;
	my-=image[0]->h/2;

	my*=-1;

	int dir=0;
	float thrust=0;
	bool turbo=true; //I feel the need, the need for speed.

	//FIXME
	float h = hypot(mx,my);

	float angle = 0;

	if(mx < 0)
	{
		angle=-acos(-my/-h);
	}
	else
	{
		angle=acos(my/h);
	}

	int to_frame = (int)(32 * angle/(2*M_PI));

	if(to_frame < 0) to_frame = 32+to_frame;

	/*
	if(shoot==0){turbo=1;} //I think turbo should be automatic
	if(death_on){turbo=0;} //Except when using a special weapon!
	*/

	if(thrusting)
	{
		thrust = hypot(mx,my);

		thrust /= hypot(screen->w/2,screen->h/2) * 2;
	}
	else
	{
		thrust=0;
	}

	speed=sqrt(x_vel*x_vel+y_vel*y_vel);

	int diff = to_frame - frame;

	if(hypot(mx,my) < 25)
	{
		dir=0;
	}
	else
	{
		if(diff > 0 && diff <= 16)
		{
			dir=1;
		}
		else if(diff < -16 && diff >=-32)
		{
			dir=1;
		}
		else if(diff==0)
		{
			dir=0;
		}
		else
		{
			dir=-1;
		}
	}

	while(speed > thrust_speed)
	{
		x_vel*=0.98;
		y_vel*=0.98;
		speed=sqrt(x_vel*x_vel+y_vel*y_vel);
	}

	//NEW! We want things to slow down now.
	//FIXME
	x_vel*=0.999;
	y_vel*=0.999;
	
	if(reverse_count>0)
	{
		dir=-dir;
		reverse_count--;
	}

	if(death && death_count == death_max)
	{
		death_on=1;
	}
	if(death_count==0)
	{
		death_on=0;
	}

	if(death_on && weapon==1) //Whirlwind
	{
		dir=1;
		shoot=1;
		shoot_count-=(int)(shoot_speed/3);
		death_count-=3;
		if(death_count<0){death_count=0;}
		thrust=0;
		x_vel=0;
		y_vel=0;
		turncount-=(int)((turnspeed*death_count)/(2*death_max));
	}
	else if(death_on && weapon==0) //Triad
	{
		if(shoot==0){turbo=1;}
		if(shoot && shoot_count<=0)
		{
			thrust--;

			death_count-=death_max/50;
			if(death_count<0){death_count=0;}

			int fdir=frame-1;
			if(turncount<=0)
			{
				fdir+=dir;
			}
			int shdir=fdir;

			for(int i=0;i<2;i++)
			{
				shdir=fdir+i*2;
				if(shdir<0){shdir+=maxframe;}
				if(shdir>=maxframe){shdir-=maxframe;}

				loc.x=(int)x_loc+image[0]->w/2;
				loc.y=(int)y_loc+image[0]->h/2;

				loc.x+=(int)(World::XM[shdir]*image[0]->w/2);
				loc.y+=(int)(World::YM[shdir]*image[0]->h/2);

				bullets->fire(shdir,loc,"player");
			}
		}
	}
	else if(death_on && weapon==4) //Death blossom
	{
		shoot=1;
		dir=-3;
		shoot_count-=(int)shoot_speed;
		turncount=0;
		death_count-=5;
		if(death_count<0){death_count=0;}
		thrust=0;
		x_vel=0;
		y_vel=0;
	}
	else if(death_on && weapon==5) //Backfire
	{
		if(shoot==0){turbo=1;}
		thrust++;
		if(death_count/20 == death_count/(float)20)
		{
			int shdir=frame+(maxframe/2)+dir-4; //Behind you!
			for(int i=0;i<3;i++)
			{
				shdir+=2;
				if(shdir<0){shdir+=maxframe;}
				if(shdir>=maxframe){shdir-=maxframe;}

				loc.x=(int)x_loc+image[0]->w/2;
				loc.y=(int)y_loc+image[0]->h/2;

				loc.x+=(int)(World::XM[shdir]*image[0]->w/2);
				loc.y+=(int)(World::YM[shdir]*image[0]->h/2);
				
				bullets->fire(shdir,loc,"player");
			}
		}
		death_count-=1;
		if(death_count<0){death_count=0;}
	}
	else if(death_on && weapon==3) //Death ray
	{
		shoot=1;
		shoot_count-=(int)(shoot_speed/4);
		death_count-=2;
		if(death_count<0){death_count=0;}
		if(rand()%100 < 15)
		{
			dir=rand()%3-1;
		}
	}
	else if(death_on && weapon==7) //wave
	{
		loc.x=(int)x_loc+image[0]->w/2;
		loc.y=(int)y_loc+image[0]->h/2;

		loc.x+=(int)(World::XM[frame]*image[0]->w/2);
		loc.y+=(int)(World::YM[frame]*image[0]->h/2);

		if(shoot)
		{
			bullets->fire(frame+dir,loc,"wave");
			shoot=0;
			death_count-=10;
			thrust=-thrust;
		}
		else
		{
			turbo=1;
		}
	}
	else if(death_on && weapon==2) //Death star
	{
		invincible_count=200;

		dir=0;
		shoot=0;
		death_count-=2;
		if(death_count<0){death_count=0;}
		thrust=0;
		x_vel=0;
		y_vel=0;
		if(death_count==death_max-2)
		{
			for(int i=0;i<(signed)image.size();i++)
		        {
				SDL_SetAlpha(image[i],SDL_SRCALPHA|SDL_RLEACCEL,128);
			}
			for(int i=0;i<32;i++)
			{
				loc.x=(int)x_loc+image[0]->w/2;
				loc.y=(int)y_loc+image[0]->h/2;

				if(i==0)
				{
					bullets->fire(i,loc,"firstbomb");
				}
				else
				{
					bullets->fire(i,loc,"bomb");
				}
			}
		}
	}
	else if(death_on && weapon==6) //Shockwave
	{
		death_count=0;
		for(int i=0;i<32;i++)
		{
			loc.x=(int)x_loc+image[0]->w/2;
			loc.y=(int)y_loc+image[0]->h/2;

			bullets->fire(i,loc,"blade");
		}
		x_vel=0;
		y_vel=0;
	}
	else
	{
		if(death_count<0){death_count=0;}

		death_count++;
		if(death_count>death_max)
		{
			death_count=death_max;
		}
		if(death_count < death_max/5 && weapon!=0 && weapon!=5 && weapon!=6 && weapon!=7)
		{
			dir=0;
			thrust=0;
			x_vel=0;
			y_vel=0;
			shoot=0;
		}
	}

	if(turbo)
	{
		thrust*=3;
	}

	if(dir==0)
	{
		turncount=0;
	}
	else
	{
		if(turncount>0)
		{
			turncount--;
			dir=0;
		}
		else
		{
			turncount=(int)turnspeed;
		}
	}

	frame+=dir;

	if(frame<0){frame+=maxframe;}
	if(frame>=maxframe){frame-=maxframe;}

	direction=frame;

	x_vel+=thrust*thrust_speed*World::XM[direction]/inertia;
	y_vel+=thrust*thrust_speed*World::YM[direction]/inertia;

	x_loc+=x_vel*tf;
	y_loc+=y_vel*tf;

	while(x_loc < 0) {x_loc+=play_w;}
	while(y_loc < 0) {y_loc+=play_h;}
	while(x_loc > play_w) {x_loc-=play_w;}
	while(y_loc > play_h) {y_loc-=play_h;}
	
	loc.x=(int)x_loc;
	loc.y=(int)y_loc;

	if(shoot && shoot_count<=0)
	{
		loc.x+=image[0]->w/2;
		loc.y+=image[0]->h/2;

		loc.x+=(int)(World::XM[direction]*image[0]->w/2);
		loc.y+=(int)(World::YM[direction]*image[0]->h/2);

		bullets->fire(direction,loc,"player");
		shoot_count=(int)shoot_speed;
	}
	else
	{
		shoot_count--;
	}
}

bool Ship::collide(SDL_Rect pos)
{
	pos.x=(int)(pos.x+(float)pos.w/10);
	pos.y=(int)(pos.y+(float)pos.h/10);
	pos.w=(int)(pos.w*0.8);
	pos.h=(int)(pos.w*0.8);

	loc.x=(int)(x_loc+(float)image[0]->w/10);
	loc.y=(int)(y_loc+(float)image[0]->h/10);
	loc.w=(int)(image[0]->w*0.8);
	loc.h=(int)(image[0]->h*0.8);

	if(lives > 0 && invincible_count<=0)
	{
		if(loc.x+loc.w > pos.x && loc.y+loc.h > pos.y &&
				loc.x < pos.x+pos.w && loc.y < pos.y+pos.h)
		{
			return 1;
		}
	}
	return 0;
}

int Ship::get_death(void)
{
	return death_count/10;
}

float Ship::get_lifeGrade(void)
{
	return (float)lives/max_lives;
}

void Ship::give_bonus(int b)
{
	if(b==World::SPEED)
	{
		thrust_speed+=1;
		if(thrust_speed>10){thrust_speed=10;}
	}
	else if(b==World::TURN)
	{
		turnspeed-=0.2;
		if(turnspeed<0){turnspeed=0;}
	}
	else if(b==World::SHOOT)
	{
		shoot_speed-=2;
		if(shoot_speed<3){shoot_speed=3;}
	}
	else if(b==World::HEALTH)
	{
		max_lives+=2;
		lives=max_lives;
	}
	else if(b==World::REVERSE)
	{
		reverse_count=1500;
	}
	else if(b==World::WEAPON)
	{
		weapon++;
		if(weapon>=World::swodads){weapon=0;}
	}
	else if(b==World::INVINCIBLE)
	{
		invincible_count=1000;
		for(int i=0;i<(signed)image.size();i++)
		{
			SDL_SetAlpha(image[i],SDL_SRCALPHA|SDL_RLEACCEL,128);
		}
	}
}
