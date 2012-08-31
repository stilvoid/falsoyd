#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <sstream>
#if defined	WIN32
#include <windows.h>
#endif

#include "SDL/SDL.h"

#include "level.h"

#include "menu.h"

int xres=640;
int yres=480;
int bpp=16;

#if defined	WIN32
int APIENTRY WinMain(HINSTANCE hInstance,
			HINSTANCE hPrevInstance,
			LPSTR lpCmdLine,
			int nCmdShow)
{
	char* c[0];
	main(0,c);
	return 0;
}
#endif

void print_help(void);

int main(int argc,char* argv[])
{
	World::audio_enabled = true;
	World::music_enabled = true;

	bool fullscreen=0;

	int depth=3;

	float speed=1; //desired fps
	float ct=0; //current time
	float lt=0; //Last time
	double tf=0; //Time factor

	int mus_vol=128;

	int frames=0; //Frame count

	if(argc > 1)
	{
	for(int i=1;i<argc;i++)
	{
		if(strcmp(argv[i],"-f")==0)
		{
			fullscreen=1;
		}
		else if(strcmp(argv[i],"-h")==0 || strcmp(argv[i],"-help")==0 || strcmp(argv[i],"--help")==0)
		{
			print_help();
		}
		else
		{
			cout << endl << "Invalid switch \"" << argv[i] << "\"" << endl;
			print_help();
			i=argc;
		}
	}
	}

	srand(time(NULL));

	SDL_Surface *screen;		//The screen itself!
	SDL_Surface *background;	//For the background image.
	SDL_Surface *pause_img;		//The pause image.
	SDL_Surface *mouse_img;		//Image used for the mouse pointer
	SDL_Rect viewport;		//The size of the viewport.
	SDL_Rect temp;
	SDL_Rect mouse_pos;

	vector<int> options;

	//Init SDL
	if(SDL_Init(SDL_INIT_JOYSTICK|SDL_INIT_VIDEO)<0)
	{
		printf("Unable to init SDL: %s\n",
				SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);

	if(SDL_Init(SDL_INIT_AUDIO)<0)
	{
		printf("Unable to init SDL audio: %s\n",
				SDL_GetError());
		World::audio_enabled = false;
	}
	else
	{
		if(audio_open() == 0)
		{
			World::audio_enabled = false;
		}
	}
	
	char temps[100];

if(World::audio_enabled == true)
{
	cout << "Setting up audio" << endl;

	static Mix_Music *music = NULL;
	
	sprintf(temps,"%s/sounds/falsoyd.mod",PREFIX);
	music = Mix_LoadMUS(temps);
	if(music==NULL)
	{
		cerr << "Couldn't load " << temps << ": " << SDL_GetError() << "\n";
		World::music_enabled = false;
	}

	sound_enable(1);
	sound_volume(127);

	if(World::music_enabled == true)
	{
		Mix_PlayMusic(music,-1);
		Mix_VolumeMusic(mus_vol);
	}
}
	
	//Init the screen
	screen=SDL_SetVideoMode(xres,yres,bpp,
			SDL_HWSURFACE);
	if(screen==NULL)
	{
		printf("Unable to set %dx%d video: %s\n",
				xres,yres,SDL_GetError());
		exit(1);
	}

	if(fullscreen)
	{
		 screen=SDL_SetVideoMode(xres,yres,bpp,SDL_FULLSCREEN|SDL_HWSURFACE);
	}

	sprintf(temps,"%s/gfx/splash.bmp",PREFIX);
	SDL_Surface* splash=SDL_LoadBMP(temps);
	SDL_BlitSurface(splash,NULL,screen,NULL);
	delete splash;

	SDL_Flip(screen);

	sprintf(temps,"%s/gfx/back.bmp",PREFIX);
	background=SDL_LoadBMP(temps);
	SDL_BlitSurface(background,NULL,screen,NULL);

	sprintf(temps,"%s/gfx/interface/pause.bmp",PREFIX);
	pause_img=SDL_LoadBMP(temps);
	SDL_SetColorKey(pause_img,SDL_SRCCOLORKEY|SDL_RLEACCEL,
			SDL_MapRGB(pause_img->format,0x00,0x00,0x00));

	sprintf(temps,"%s/gfx/interface/sights.bmp",PREFIX);
	mouse_img=SDL_LoadBMP(temps);
	SDL_SetColorKey(mouse_img,SDL_SRCCOLORKEY|SDL_RLEACCEL,
			SDL_MapRGB(mouse_img->format,0xff,0xff,0xff));

	SDL_ShowCursor(SDL_DISABLE);
	SDL_WM_GrabInput(SDL_GRAB_ON);

	int baddies=1;

	bool done=0;	//Are we done yet?

	int direction=0;	//Which direction are we going?
	int thrust=0;		//And are we moving?

	bool shoot=0;		//Are we shooting?
	bool turbo=0;
	bool death=0;
	bool thrusting=0;

	bool nuke=0;

	bool pause=1;

	bool in_menu=1;

	int complete=0;

	viewport.x=0;//20;
	viewport.y=0;//20;
	viewport.w=xres;//-40;
	viewport.h=yres;//-90;

	int move_x=0;
	int move_y=0;
	
	//FIXME

	Menu main_menu(screen,viewport);

	Level levelone(screen,viewport,depth);
	levelone.init(baddies,1);

	SDL_FillRect(screen,&viewport,SDL_MapRGB(screen->format,
				0x00,0x00,0x00));

	if(speed==0){speed=20;}
	else if(speed==1){speed=10;}
	else if(speed==2){speed=5;}
	else{speed=10;}

	int map_size=1;

	//AND THE MAIN LOOP
	while(done==0)
	{
		SDL_Event event;

		while(SDL_PollEvent(&event))
		{
			if (event.type==SDL_QUIT) {done=1;}
			if (event.type==SDL_KEYDOWN)
			{
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						in_menu=1;
						break;
					case SDLK_s:
						SDL_SaveBMP(screen,"out.bmp");
						break;
					case SDLK_p:
						pause=!pause;
						if(World::music_enabled == true)
						{

						if(pause)
						{
							Mix_VolumeMusic(127);
						}
						else
						{
							Mix_VolumeMusic(mus_vol);
						}
						}
						break;
					case SDLK_n:
						nuke=1;
						break;
					case SDLK_1:
						map_size=1;
						break;
					case SDLK_2:
						map_size=2;
						break;
					case SDLK_3:
						map_size=3;
						break;
					case SDLK_4:
						map_size=4;
						break;
					default:
						break;
				}
			}
			if (event.type==SDL_KEYUP)
			{
				if(event.key.keysym.sym==SDLK_f)
				{
					if(fullscreen == 0)
					{
						screen=SDL_SetVideoMode(xres,yres,16,SDL_FULLSCREEN|SDL_HWSURFACE);
						fullscreen=1;
					}
					else
					{
						screen=SDL_SetVideoMode(xres,yres,16,SDL_HWSURFACE);
						fullscreen=0;
					}
					SDL_BlitSurface(background,NULL,screen,NULL);
				}
			}
			if(event.type == SDL_MOUSEMOTION)
			{
				move_x = event.motion.x;
				move_y = event.motion.y;

				mouse_pos.x = move_x - mouse_img->w/2;
				mouse_pos.y = move_y - mouse_img->h/2;
			}
		}

		while(in_menu==1)
		{
			options=main_menu.do_menu();
			in_menu=0;
			if(options[QUIT]==1){done=1;}
			if(options[START]==1)
			{
				baddies=1;
				levelone.init(baddies,1);
				pause=0;
			}
			if(options[FULLSCREEN]==1)
			{
				if(fullscreen == 0)
			        {
					screen=SDL_SetVideoMode(xres,yres,16,SDL_FULLSCREEN|SDL_HWSURFACE);
				        fullscreen=1;
				 }
				 else
				 {
					 screen=SDL_SetVideoMode(xres,yres,16,SDL_HWSURFACE);
				         fullscreen=0;
				  }
				  //SDL_BlitSurface(background,NULL,screen,NULL);
				  in_menu=1;
			}
			SDL_BlitSurface(background,NULL,screen,NULL);
		}

		if(SDL_GetMouseState(NULL,NULL)&SDL_BUTTON(3))
		{
			thrusting=1;
		}
		else
		{
			thrusting=0;
		}

		if(SDL_GetMouseState(NULL,NULL)&SDL_BUTTON(1))
		{
			shoot=1;
		}
		else
		{
			shoot=0;
		}

		if(SDL_GetMouseState(NULL,NULL)&SDL_BUTTON(2))
		{
			death=1;
		}
		else
		{
			death=0;
		}

		ct=SDL_GetTicks();
		tf=(ct-lt)/speed;
		lt=ct;
		frames++;

		if(pause == 0)
		{
			levelone.move(move_x-screen->w/2,move_y-screen->h/2,thrusting,shoot,death,nuke,tf,map_size);

			levelone.collisions();

			complete=levelone.draw();
		}
		else
		{
			temp.x=viewport.x+(viewport.w/2)-(pause_img->w/2);
			temp.y=viewport.y+(viewport.h/2)-(pause_img->h/2);
			temp.x+=rand()%51-25;
			temp.y+=rand()%51-25;
			levelone.draw();
			SDL_BlitSurface(pause_img,NULL,screen,&temp);
			SDL_Flip(screen);
			while(SDL_PollEvent(&event))
			{
				if(event.type == SDL_KEYDOWN)
				{
					pause = false;
				}
			}
			SDL_Delay(50);
		}

		nuke=0;

		if(complete == 100)
		{
			baddies+=1;

			SDL_FillRect(screen,&viewport,SDL_MapRGB(screen->format,
						0xff,0xff,0xff));
			SDL_Flip(screen);
			levelone.init(baddies,0);

		/*	thrust=0;
			direction=0;
			shoot=0;
			turbo=0;
			death=0;
			*/
		}
		if(complete < 0)
		{
			SDL_FillRect(screen,&viewport,SDL_MapRGB(screen->format,
						0xaa,0x00,0x00));
			SDL_Flip(screen);
			levelone.init(baddies,0);
		}

		//Draw the mouse pointer
		SDL_BlitSurface(mouse_img,NULL,screen,&mouse_pos);

		if(done==0)
		{
			SDL_Flip(screen);
		}
	}

if(World::audio_enabled == true)
{
	audio_close();
}

	return 0;
}

void print_help(void)
{
	cout << endl;
	cout << "Falsoyd " << VERSION << ": Help" << endl;
	cout << endl;
	cout << "Usage: falsoyd [-f] [-h|-help|--help]" <<endl;
	cout << "   " << "-h|-help|--help" << "\t" << "Display this message" << endl;
	cout << "   " << "-f" << "\t\t\t" << "Start Falsoyd in fullscreen mode" << endl;
	cout << endl;

	exit(1);
}
