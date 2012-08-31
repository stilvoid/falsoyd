#define START 0
#define QUIT 1
#define FULLSCREEN 2
#define HELP 3

#include <vector>

#include "config.h"

const int X[40]={0,-20,-40,-60,-80,-100,-120,-140,-160,-180,
		 -200,-180,-160,-140,-120,-100,-80,-60,-40,-20,
		 0,20,40,60,80,100,120,140,160,180,
		 200,180,160,140,120,100,80,60,40,20};

const int Y[40]={-150,-135,-120,-105,-90,-75,-60,-45,-30,-15,
		 0,15,30,45,60,75,90,105,120,135,
		 150,135,120,105,90,75,60,45,30,15,
		 0,-15,-30,-45,-60,-75,-90,-105,-120,-135};

class Menu
{
	private:
		SDL_Surface* screen;		//The screen!
		SDL_Rect viewport;		//The places we can draw on.

		SDL_Surface* background;	//Background image.
		std::vector<SDL_Surface*> image;	//The button images.
		std::vector<SDL_Surface*> back;	//Backing stores.
		void do_help(void);
	public:
		Menu(SDL_Surface* s,SDL_Rect v);
		vector<int> do_menu(void);
};

Menu::Menu(SDL_Surface* s,SDL_Rect v)
{
	screen=s;
	viewport=v;

	char tempfile[100];

	for(int i=0;i<4;i++)
	{
		sprintf(tempfile,"%s/gfx/%s/%d.bmp",PREFIX,"menu",i);
                image.push_back(SDL_LoadBMP(tempfile));
	        SDL_SetColorKey(image[i],SDL_SRCCOLORKEY|SDL_RLEACCEL,
		        SDL_MapRGB(image[i]->format,0x00,0x00,0x00));
	}

	for(int i=0;i<(signed)image.size();i++)
	{
		back.push_back(SDL_CreateRGBSurface(SDL_SWSURFACE,image[i]->w,image[i]->h,
				16,0,0,0,0));
	}

	char temps[100];
	sprintf(temps,"%s/gfx/menu/back.bmp",PREFIX);
	background=SDL_LoadBMP(temps);

	viewport.x=0;
	viewport.y=0;
	viewport.w=screen->w;
	viewport.h=screen->h;
}

vector<int> Menu::do_menu(void)
{
	Uint32 lt=0;
	Uint32 ct=0;

	int dir=0;
	int count=0;
	int choice=0;
	bool select=0;
	bool done=0;

	int num_options=4;

	vector<int> loc;
	SDL_Rect temp;

	vector<int> options;

	for(int i=0;i<num_options;i++)
	{
		loc.push_back(i*(40/num_options));
	}

	for(int i=0;i<10;i++)
	{
		options.push_back(0);
	}

	SDL_BlitSurface(background,NULL,screen,NULL);

	int x = 0;

	while(done==0 || dir!=0)
	{
		x=0;

		SDL_Event event;

		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_MOUSEMOTION)
			{
				if(dir == 0)
				{
					x = event.motion.xrel;
				}
			}
		}

		if(x < -1)
		{
			choice--;
			dir+=40/loc.size();
		}
		else if(x > 1)
		{
			choice++;
			dir-=40/loc.size();
		}

		if(SDL_GetMouseState(NULL,NULL)&SDL_BUTTON(1))
		{
			select=1;
		}

		if(select)
		{
			while(choice<0){choice+=loc.size();}
			while(choice>=(signed)loc.size()){choice-=loc.size();}

			if(choice==START){options[START]=1;done=1;}
			if(choice==QUIT){options[QUIT]=1;done=1;}
			if(choice==FULLSCREEN){options[FULLSCREEN]=1;done=1;}
			if(choice==HELP){do_help();}
			select=0;
		}

		if(count==0)
		{
			if(dir<0)
			{
				for(int i=0;i<(signed)loc.size();i++)
				{
					loc[i]--;
					if(loc[i]<0){loc[i]=39;}
				}
				dir++;
			}

			if(dir>0)
			{
				for(int i=0;i<(signed)loc.size();i++)
				{
					loc[i]++;
					if(loc[i]==40){loc[i]=0;}
				}
				dir--;
			}

			count=(40/loc.size())-abs(dir);
			if(count<=0){count=1;}
			//count=1;
		}
		else
		{
			count--;
		}

		for(int i=0;i<(signed)loc.size();i++)
		{
			temp.x=viewport.x+viewport.w/2-image[0]->w/2+X[loc[i]];
			temp.y=viewport.y+viewport.h/2-image[0]->h/2+Y[loc[i]];
			SDL_SetAlpha(image[i],SDL_SRCALPHA|SDL_RLEACCEL,256-(int)(256*((float)temp.y/screen->h)));
			SDL_BlitSurface(screen,&temp,back[i],NULL);
			SDL_BlitSurface(image[i],NULL,screen,&temp);
		}

		SDL_Flip(screen);

		for(int i=0;i<(signed)loc.size();i++)
		{
			temp.x=viewport.x+viewport.w/2-image[0]->w/2+X[loc[i]];
			temp.y=viewport.y+viewport.h/2-image[0]->h/2+Y[loc[i]];
			SDL_BlitSurface(back[i],NULL,screen,&temp);
		}

		ct=SDL_GetTicks();

		if(ct-lt < 10)
		{
			SDL_Delay(10-(ct-lt));
		}

		lt=SDL_GetTicks();

	}

	return options;

}

void Menu::do_help(void)
{
	int num_screens=1;
	bool done=0;

	SDL_Surface* helpscreen[num_screens];
	SDL_Surface* backup;

	char temps[100];
	for(int i=0;i<num_screens;i++)
	{
		sprintf(temps,"%s/gfx/menu/help%d.bmp",PREFIX,i);
		helpscreen[i]=SDL_LoadBMP(temps);
	}
	backup=SDL_LoadBMP(temps);
	SDL_BlitSurface(screen,NULL,backup,NULL);

	for(int i=0;i<num_screens;i++)
	{
		done=0;

		SDL_BlitSurface(helpscreen[i],NULL,screen,NULL);
		SDL_Flip(screen);

		while(done==0)
		{
			SDL_PumpEvents();
			if(SDL_GetMouseState(NULL,NULL)&SDL_BUTTON(1))
			{
				done=1;
			}
		}
	}

	SDL_BlitSurface(backup,NULL,screen,NULL);
	delete backup;
}
