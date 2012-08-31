#include "map.h"
#include "bonus.h"
#include "score.h"
#include <iostream>

#include "alien.h"

#include "ship.h"

using namespace std;

class Level
{
	private:
	SDL_Surface *levelmap;		//The game area
	SDL_Rect viewport;		//Where on the screen do we need to be?
	SDL_Surface *screen;		//The screen!

	int play_w;
	int play_h;

	int map_size;
	SDL_Surface *hud;			//The HUD (far radar etc)
	SDL_Surface *hud_back;			//Where the HUD is arranged (for centring)

	vector<SDL_Surface*> player_img;//The player sprites.
	vector<SDL_Surface*> alien_img;	//The aliens sprites.
	vector<SDL_Surface*> alien2_img;//The 2nd aliens sprites.
	vector<SDL_Surface*> alien3_img;
	vector<SDL_Surface*> alien4_img;
	vector<SDL_Surface*> alien5_img;
	vector<SDL_Surface*> alien6_img;
	vector<SDL_Surface*> alien7_img;
	vector<SDL_Surface*> alien8_img;
	vector<SDL_Surface*> alien9_img;

	SDL_Rect temp;			//temporary storage

	Ship *player;			//The player
	Alien *baddy[World::num_aliens];
	Bonus *bonuses;
	Map *game_map;			//The map
	Score score_display;	//Score displayer

	Shot* bullets;

	int level_aliens;		//How many aliens on *this* level
	int complete;			//How far along are we?

	int alien_stop;
	int player_stop;

	int player_random;

	int radar_static;

	int total_kills;

	void radar(void);

	public:
		Level(SDL_Surface *s,SDL_Rect v,int depth);
		~Level(void);
		void init(int n,bool new_game);
		void move(int mx,int my,bool thrusting,bool shoot,bool death,bool nuke,float tf,int ms);
		void collisions(void);
		int draw(void);
};

Level::Level(SDL_Surface *s,SDL_Rect v, int depth)
{
	char tempfile[100];
	srand(time(NULL));

	screen=s;
	viewport=v;

	//Load in the player images
	for(int i=0;i<32;i++)
	{
		sprintf(tempfile,"%s/gfx/%s/%d.bmp",PREFIX,"player",i);
		player_img.push_back(SDL_LoadBMP(tempfile));
		SDL_SetColorKey(player_img[i],SDL_SRCCOLORKEY|SDL_RLEACCEL,
				SDL_MapRGB(player_img[i]->format,0x00,0x00,0x00));
	}

	//Load in the alien images
	for(int i=0;i<8;i++)
	{
		sprintf(tempfile,"%s/gfx/%s/%d.bmp",PREFIX,"alien",i);
		alien_img.push_back(SDL_LoadBMP(tempfile));
		SDL_SetColorKey(alien_img[i],SDL_SRCCOLORKEY|SDL_RLEACCEL,
				SDL_MapRGB(alien_img[i]->format,0xff,0xff,0xff));
	}
	for(int i=0;i<8;i++)
	{
		sprintf(tempfile,"%s/gfx/%s/%d.bmp",PREFIX,"alien2",i);
		alien2_img.push_back(SDL_LoadBMP(tempfile));
		SDL_SetColorKey(alien2_img[i],SDL_SRCCOLORKEY|SDL_RLEACCEL,
				SDL_MapRGB(alien2_img[i]->format,0x00,0x00,0x00));
	}
	for(int i=0;i<8;i++)
	{
		sprintf(tempfile,"%s/gfx/%s/%d.bmp",PREFIX,"alien3",i);
		alien3_img.push_back(SDL_LoadBMP(tempfile));
		SDL_SetColorKey(alien3_img[i],SDL_SRCCOLORKEY|SDL_RLEACCEL,
				SDL_MapRGB(alien3_img[i]->format,0x00,0x00,0x00));
	}
	for(int i=0;i<8;i++)
	{
		sprintf(tempfile,"%s/gfx/%s/%d.bmp",PREFIX,"alien4",i);
		alien4_img.push_back(SDL_LoadBMP(tempfile));
		SDL_SetColorKey(alien4_img[i],SDL_SRCCOLORKEY|SDL_RLEACCEL,
				SDL_MapRGB(alien4_img[i]->format,0x00,0x00,0x00));
	}
	for(int i=0;i<8;i++)
	{
		sprintf(tempfile,"%s/gfx/%s/%d.bmp",PREFIX,"alien5",i);
		alien5_img.push_back(SDL_LoadBMP(tempfile));
		SDL_SetColorKey(alien5_img[i],SDL_SRCCOLORKEY|SDL_RLEACCEL,
				SDL_MapRGB(alien5_img[i]->format,0xff,0xff,0xff));
	}
	for(int i=0;i<8;i++)
	{
		sprintf(tempfile,"%s/gfx/%s/%d.bmp",PREFIX,"alien6",i);
		alien6_img.push_back(SDL_LoadBMP(tempfile));
		SDL_SetColorKey(alien6_img[i],SDL_SRCCOLORKEY|SDL_RLEACCEL,
				SDL_MapRGB(alien6_img[i]->format,0xff,0xff,0xff));
	}
	for(int i=0;i<8;i++)
	{
		sprintf(tempfile,"%s/gfx/%s/%d.bmp",PREFIX,"alien7",i);
		alien7_img.push_back(SDL_LoadBMP(tempfile));
		SDL_SetColorKey(alien7_img[i],SDL_SRCCOLORKEY|SDL_RLEACCEL,
				SDL_MapRGB(alien7_img[i]->format,0x00,0x00,0x00));
	}
	for(int i=0;i<8;i++)
	{
		sprintf(tempfile,"%s/gfx/%s/%d.bmp",PREFIX,"alien8",i);
		alien8_img.push_back(SDL_LoadBMP(tempfile));
		SDL_SetColorKey(alien8_img[i],SDL_SRCCOLORKEY|SDL_RLEACCEL,
				SDL_MapRGB(alien8_img[i]->format,0x00,0x00,0x00));
	}
	for(int i=0;i<8;i++)
	{
		sprintf(tempfile,"%s/gfx/%s/%d.bmp",PREFIX,"alien9",i);
		alien9_img.push_back(SDL_LoadBMP(tempfile));
		SDL_SetColorKey(alien9_img[i],SDL_SRCCOLORKEY|SDL_RLEACCEL,
				SDL_MapRGB(alien9_img[i]->format,0x00,0x00,0x00));
	}

	//Init the stuff0rz
	//levelmap=SDL_CreateRGBSurface(SDL_HWSURFACE,32*120,32*40,16,0,0,0,0);
	levelmap=SDL_CreateRGBSurface(SDL_HWSURFACE,screen->w,screen->h,16,0,0,0,0);

	int hudsize=70;

	map_size=1;

	play_w=levelmap->w*5;
	play_h=levelmap->h*4;

	game_map=new Map();

	game_map->init(screen,levelmap,play_w,play_h,viewport,"spid");

	SDL_Rect temp=game_map->get_loc();

	play_w=temp.w;
	play_h=temp.h;
	
	hud=SDL_CreateRGBSurface(SDL_HWSURFACE,(int)((float)((float)play_w/play_h)*hudsize),hudsize,16,0,0,0,0);
	hud_back=SDL_CreateRGBSurface(SDL_HWSURFACE,hud->w,hud->h,16,0,0,0,0);

	bullets = new Shot(screen,play_w,play_h);

	player = new Ship(screen,play_w,play_h,bullets,player_img);

	bonuses = new Bonus(screen,play_w,play_h);

	for(int i=0;i<World::num_aliens;i++)
	{
		baddy[i] = new Alien(screen,play_w,play_h,bullets,alien_img);
	}

	score_display.Init(screen,viewport);
	
	complete=0;

	alien_stop=0;
	player_stop=0;

	player_random=0;
	radar_static=0;

	total_kills=0;
}

void Level::init(int n,bool new_game)
{
	int diff=0;

	SDL_Rect temp;

	//game_map->init(screen,play_w,play_h,viewport,"spid");

	level_aliens=rand()%((play_w*play_h)/100000)*n/(diff+1);

	while(level_aliens > World::num_aliens)
	{
		diff++;
		level_aliens-=World::num_aliens;
	}

	temp.x=play_w/2;
	temp.y=play_h/2;

	if(new_game)
	{
		player->init(temp,1,32);
	}
	else
	{
		player->init(temp,0,32);
	}

	int* nasty=new int;
	*nasty=rand()%40;

	if(*nasty==1)
	{
		level_aliens=World::num_aliens;
		for(int i=0;i<level_aliens;i++)
		{
			temp.x=rand()%play_w;
			temp.y=rand()%play_h;
			baddy[i]->init(temp,1+diff,8,alien8_img,World::FOLLOW2,rand()%(diff+1));
		}
	}
	else if(*nasty==2)
	{
		level_aliens=7;
		for(int i=0;i<level_aliens;i++)
		{
			temp.x=rand()%play_w;
			temp.y=rand()%play_h;
			baddy[i]->init(temp,7+diff,8,alien7_img,rand()%5,rand()%3+1);
		}
	}
	else
	{
	for(int i=0;i<level_aliens;i++)
	{
		temp.x=rand()%play_w;
		temp.y=rand()%play_h;
		if(rand()%100+level_aliens/6 > 120)
		{
			baddy[i]->init(temp,1,8,alien8_img,World::FOLLOW,(int)(((float)level_aliens/World::num_aliens)*(rand()%5+5)));
		}
		else if(rand()%100 > 95)
		{
			baddy[i]->init(temp,30,8,alien9_img,World::ROCK,-2);
		}
		else if(rand()%100+level_aliens/6 > 90)
		{
			baddy[i]->init(temp,rand()%5+1,8,alien7_img,World::RANDOM,(int)(((float)level_aliens/World::num_aliens)*(rand()%5+5)));
		}
		else if(rand()%100+level_aliens/6 > 110)
		{
			baddy[i]->init(temp,1+diff,8,alien6_img,World::KAMIKAZE,(int)(((float)level_aliens/World::num_aliens)*(rand()%5+5)));
		}
		else if(rand()%100+level_aliens/6 > 130)
		{
			baddy[i]->init(temp,1+diff,8,alien5_img,World::KAMIKAZE,(int)(((float)level_aliens/World::num_aliens)*(rand()%5+5)));
		}
		else if(rand()%100+level_aliens/6 > 100)
		{
			baddy[i]->init(temp,1+diff,8,alien4_img,World::AVOID,(int)(((float)level_aliens/World::num_aliens)*(rand()%5+5)));
		}
		else if(rand()%100+level_aliens/6 > 105)
		{
			baddy[i]->init(temp,1+diff+rand()%2,8,alien2_img,World::FOLLOW,(int)(((float)level_aliens/World::num_aliens)*(rand()%5+5)));
		}
		else if(rand()%100 > 90)
		{
			baddy[i]->init(temp,1+diff,8,alien3_img,World::AVOID,(int)(((float)level_aliens/World::num_aliens)*(rand()%5+5)));
		}
		else if(rand()%100 > 80)
		{
			baddy[i]->init(temp,1+diff,8,alien3_img,World::RANDOM,(int)(((float)level_aliens/World::num_aliens)*(rand()%5+5)));
		}
		else
		{
			baddy[i]->init(temp,1+diff,8,alien_img,World::RANDOM,(int)(((float)level_aliens/World::num_aliens)*(rand()%5+5)));
		}
	}
	}

	bonuses->init();

	bullets->init();

	complete=0;

	delete nasty;
}

void Level::move(int mx,int my,bool thrusting,bool shoot,bool death,bool nuke,float tf,int ms)
{
	map_size=ms;

	SDL_Rect temp;
	int i;

	if(nuke)
	{
		for(i=0;i<level_aliens;i++)
		{
			baddy[i]->hit(1);
		}
	}

	bullets->move(tf);

	temp=player->get_loc();


	if(alien_stop==0)
	{
		for(i=0;i<level_aliens;i++)
		{
			baddy[i]->move(temp,tf);
		}
	}
	else{alien_stop--;}

	bonuses->move();

	if(player_stop==0 && player_random==0)
	{
		player->move(mx,my,thrusting,shoot,death,tf);
	}
	else if(player_random>0)
	{
		player->move(0,0,0,0,0,tf);
		player_random--;
	}
	else if(player_stop>0){player_stop--;}

	temp=player->get_loc();

	game_map->warp(temp.x-viewport.w/2,temp.y-viewport.h/2);
}

void Level::collisions(void)
{
	int bonus_type=0;
	int temp_hits=0;

	for(int i=0;i<level_aliens;i++)
	{
		if(baddy[i]->get_lives())
		{
			temp_hits=bullets->collide(baddy[i]->get_loc(),"alien");
			baddy[i]->hit(temp_hits);
			total_kills+=temp_hits;
			if(player->collide(baddy[i]->get_loc()))
			{
				total_kills--;
				player->hit(1);
				baddy[i]->hit(1);
			}
		}
	}

	temp_hits=bullets->collide(player->get_loc(),"player");
	total_kills-=temp_hits;
	player->hit(temp_hits);

	bonus_type=bonuses->collide(player->get_loc());

	if(bonus_type == World::RAND)
	{
		bonus_type=rand()%World::bonus_types;
	}

	if(bonus_type == World::SLOW)
	{
		if(rand()%100>20){alien_stop=1000;}
		else{player_stop=500;}
	}

	if(bonus_type == World::STATIC)
	{
		radar_static=rand()%3000+100;
	}

	if(bonus_type == World::RAND)
	{
		player_random=1000;
	}

	player->give_bonus(bonus_type);
		
}

int Level::draw(void)
{
	SDL_Rect onmap=player->get_loc();
	onmap.x-=viewport.w/2;
	onmap.y-=viewport.h/2;
	while(onmap.x<0){onmap.x+=play_w;}
	while(onmap.y<0){onmap.y+=play_h;}
	while(onmap.x>play_w){onmap.x-=play_w;}
	while(onmap.y>play_h){onmap.y-=play_h;}
	onmap.w=viewport.w;
	onmap.h=viewport.h;

	game_map->draw();

	bonuses->draw(onmap);

	bullets->draw(onmap);

	for(int i=0;i<level_aliens;i++)
	{
		baddy[i]->draw(onmap);
	}

	player->draw(onmap);

	complete=0;

	for(int i=0;i<level_aliens;i++)
	{
		if(!baddy[i]->get_lives())
		{
			complete++;
		}
	}
	complete=(int)((float)complete/level_aliens*100);

	if(player->get_lives() == 0)
	{
		complete=-1;
	}

	radar();
	
	if(total_kills<0){total_kills=0;}
	score_display.Print(total_kills);

	return complete;
}

void Level::radar(void)
{
	vector<SDL_Rect> bull_locs;

	int temp;

	SDL_Rect sp=player->get_loc();
	sp.x+=player_img[0]->w/2;
	sp.y+=player_img[0]->h/2;
	SDL_Rect pixel;
	SDL_Rect out;

	int x_away=10000;
	int y_away=10000;

	pixel.w=1;
	pixel.h=1;

	out.y=0;
	out.h=hud->h;
	out.w=hud->w;
	out.x=0;

	if(radar_static<=0)
	{
		SDL_FillRect(hud,NULL,SDL_MapRGB(hud->format,0x00,0x55,0x00));
	}
	else
	{
		SDL_FillRect(hud,NULL,SDL_MapRGB(hud->format,0x00,0x00,0x00));
	}

	out.y++;
	out.x++;
	out.w-=2;
	out.h-=2;

if(radar_static<=0)
{
	SDL_Rect rloc; //radar location
	rloc=player->get_loc();

	rloc.x-=play_w/(map_size*2);
	rloc.y-=play_h/(map_size*2);
	rloc.w=play_w/map_size;
	rloc.h=play_h/map_size;

	while(rloc.x<0){rloc.x+=play_w;}
	while(rloc.y<0){rloc.y+=play_h;}
	while(rloc.x>play_w){rloc.x-=play_w;}
	while(rloc.y>play_h){rloc.y-=play_h;}

	//Baddies
	for(int i=0;i<level_aliens;i++)
	{
		if(baddy[i]->get_lives())
		{
			pixel=baddy[i]->get_loc();

			//determine the nearest one
			if(sqrt((float)((pixel.x-sp.x)*(pixel.x-sp.x) + (pixel.y-sp.y)*(pixel.y-sp.y))) < sqrt((float)(x_away*x_away + y_away*y_away)))
			{
				x_away = pixel.x - sp.x + pixel.w/2;
				y_away = pixel.y - sp.y + pixel.h/2;
			}

			if( ( (pixel.x > rloc.x && pixel.x < rloc.x+rloc.w) || (pixel.x+play_w > rloc.x && pixel.x+play_w < rloc.x+rloc.w ) )
					&&
			    ( (pixel.y > rloc.y && pixel.y < rloc.y+rloc.h) || (pixel.y+play_h > rloc.y && pixel.y+play_h < rloc.y+rloc.h ) ) )
			{
				//Baddy is on map portion
				if(pixel.x < rloc.x)
				{
					pixel.x=(int)(hud->w*((pixel.x+play_w-rloc.x)/(float)rloc.w));
				}
				else
				{
					pixel.x=(int)(hud->w*((pixel.x-rloc.x)/(float)rloc.w));
				}
				if(pixel.y < rloc.y)
				{
					pixel.y=(int)(hud->h*((pixel.y+play_h-rloc.y)/(float)rloc.h));
				}
				else
				{
					pixel.y=(int)(hud->h*((pixel.y-rloc.y)/(float)rloc.h));
				}
				pixel.w=1;
				pixel.h=1;
			
				SDL_FillRect(hud,&pixel,SDL_MapRGB(screen->format,
					0x00,0xff,0x00));
			}
		}
	}

	//Bullets
	vector<SDL_Rect> bpixel=bullets->get_locs();
	for(int i=0;i<bpixel.size();i++)
	{
		pixel.x=bpixel[i].x;
		pixel.y=bpixel[i].y;

		if( ( (pixel.x > rloc.x && pixel.x < rloc.x+rloc.w) || (pixel.x+play_w > rloc.x && pixel.x+play_w < rloc.x+rloc.w ) )
				&&
		  ( (pixel.y > rloc.y && pixel.y < rloc.y+rloc.h) || (pixel.y+play_h > rloc.y && pixel.y+play_h < rloc.y+rloc.h ) ) )
		{
			//Bullet is on map portion
			if(pixel.x < rloc.x)
			{
				pixel.x=(int)(hud->w*((pixel.x+play_w-rloc.x)/(float)rloc.w));
			}
			else
			{
				pixel.x=(int)(hud->w*((pixel.x-rloc.x)/(float)rloc.w));
			}
			if(pixel.y < rloc.y)
			{
				pixel.y=(int)(hud->h*((pixel.y+play_h-rloc.y)/(float)rloc.h));
			}
			else
			{
				pixel.y=(int)(hud->h*((pixel.y-rloc.y)/(float)rloc.h));
			}
			pixel.w=1;
			pixel.h=1;
		
			SDL_FillRect(hud,&pixel,SDL_MapRGB(screen->format,
				0xdd,0x00,0xdd));
		}
	}
	
	if(bonuses->get_lives() > 0)
	{
	//Bonus
	pixel=bonuses->get_loc();

	if( ( (pixel.x > rloc.x && pixel.x < rloc.x+rloc.w) || (pixel.x+play_w > rloc.x && pixel.x+play_w < rloc.x+rloc.w ) )
			&&
	  ( (pixel.y > rloc.y && pixel.y < rloc.y+rloc.h) || (pixel.y+play_h > rloc.y && pixel.y+play_h < rloc.y+rloc.h ) ) )
	{
		//Bonus is on map portion
		if(pixel.x < rloc.x)
		{
			pixel.x=(int)(hud->w*((pixel.x+play_w-rloc.x)/(float)rloc.w));
		}
		else
		{
			pixel.x=(int)(hud->w*((pixel.x-rloc.x)/(float)rloc.w));
		}
		if(pixel.y < rloc.y)
		{
			pixel.y=(int)(hud->h*((pixel.y+play_h-rloc.y)/(float)rloc.h));
		}
		else
		{
			pixel.y=(int)(hud->h*((pixel.y-rloc.y)/(float)rloc.h));
		}
		pixel.w=1;
		pixel.h=1;
	
		SDL_FillRect(hud,&pixel,SDL_MapRGB(screen->format,
			0xff,0x00,0x00));
	}
	}

	//Draw a dot for the player
	pixel.w=1;
	pixel.h=1;
	pixel.x=hud->w/2;
	pixel.y=hud->h/2;
	SDL_FillRect(hud,&pixel,SDL_MapRGB(screen->format,0xff,0xff,0xff));
}
else
{
	for(int i=0;i<300;i++)
	{
		pixel.x=out.x+rand()%out.w;
		pixel.y=out.y+rand()%out.h;
		pixel.w=1;
		pixel.h=1;

		temp=rand()%256;

		SDL_FillRect(hud,&pixel,SDL_MapRGB(screen->format,
					temp,temp,temp));
	}
	radar_static--;
}

	//Lives
	pixel.x=screen->w/2-166;
	pixel.y=screen->h-20;
	pixel.w=104;
	pixel.h=10;
	SDL_FillRect(screen,&pixel,SDL_MapRGB(screen->format,
				0x99,0x99,0x99));

	pixel.x+=1;
	pixel.y+=1;
	pixel.w-=2;
	pixel.h-=2;
	SDL_FillRect(screen,&pixel,SDL_MapRGB(screen->format,
				0x00,0x00,0x00));

	pixel.x+=1;
	pixel.y+=1;
	pixel.h-=2;
	pixel.w=(int)(player->get_lifeGrade()*100);
	SDL_FillRect(screen,&pixel,SDL_MapRGB(screen->format,
				(100-pixel.w)*2,pixel.w*2,0x00));

	//Special
	pixel.x=screen->w/2-52;
	pixel.y=screen->h-20;
	pixel.w=104;
	pixel.h=10;
	SDL_FillRect(screen,&pixel,SDL_MapRGB(screen->format,
				0x99,0x99,0x99));

	pixel.x+=1;
	pixel.y+=1;
	pixel.w-=2;
	pixel.h-=2;
	SDL_FillRect(screen,&pixel,SDL_MapRGB(screen->format,
				0x00,0x00,0x00));

	pixel.x+=1;
	pixel.y+=1;
	pixel.h-=2;
	pixel.w=player->get_death();
	SDL_FillRect(screen,&pixel,SDL_MapRGB(screen->format,
				pixel.w*2,0x00,(100-pixel.w)*2));

	//Complete
	pixel.x=screen->w/2+62;
	pixel.y=screen->h-20;
	pixel.w=104;
	pixel.h=10;
	SDL_FillRect(screen,&pixel,SDL_MapRGB(screen->format,
				0x99,0x99,0x99));

	pixel.x+=1;
	pixel.y+=1;
	pixel.w-=2;
	pixel.h-=2;
	SDL_FillRect(screen,&pixel,SDL_MapRGB(screen->format,
				0x00,0x00,0x00));

	pixel.x+=1;
	pixel.y+=1;
	pixel.h-=2;
	if(complete>=0){pixel.w=complete;}
	else{pixel.w=0;}
	SDL_FillRect(screen,&pixel,SDL_MapRGB(screen->format,
				0x66,pixel.w*2,(100-pixel.w)*2));
	
	out.x=10;
	out.y=10;
	out.w=hud->w;
	out.h=hud->h;

	
	SDL_SetAlpha(hud,SDL_SRCALPHA|SDL_RLEACCEL,200);
	SDL_BlitSurface(hud,NULL,screen,&out);
	SDL_SetAlpha(hud,SDL_SRCALPHA|SDL_RLEACCEL,255);

}

Level::~Level(void)
{
	if(total_kills<0){total_kills=0;}
	cout << "Score: " << total_kills*100 << endl;
}
