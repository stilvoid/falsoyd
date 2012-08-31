#include "score.h"
#include "math.h"
#include <string>
#include "config.h"

Score::Score(void)
{
	using namespace std;

	string filename = (string)PREFIX + "/gfx/score_font.bmp";

	font_img = SDL_LoadBMP (filename.c_str());
	SDL_SetColorKey(font_img,SDL_SRCCOLORKEY|SDL_RLEACCEL,
		SDL_MapRGB(font_img->format,0,0,0xff));

	char_width = font_img->w / 10;
};

void Score::Init(SDL_Surface* s,SDL_Rect v)
{
	screen=s;
	viewport=v;
}

void Score::Print (int score)
{
	using namespace std;

	score*=100; //Just makes it look more shoot-em-up like
	string text="";
	
	int size=0;
	int max_size=0;
	int temp=0;
	
	while(score >= pow(10,size))
	{
		size++;
	}
	
	max_size=size;
	
	for(int i=0;i<max_size;i++)
	{
		temp=0;
		while(score >= pow(10,size-1))
		{
			score-=(int)pow(10,size-1);
			temp++;
		}
		size--;
		
		text+=(char)(temp+48);
	}
	
	if(text==""){text="0";}
	
	SDL_Rect from;
	SDL_Rect to;
	
	from.y=0;
	from.w=char_width;
	from.h=font_img->h;
	
	to.x=10;
	to.y=viewport.y+viewport.h-10-font_img->h;
	to.w=char_width;
	to.h=font_img->h;

	for (int i = 0; i < (signed)text.length (); i++)
	{		
		from.x=((int)text[i] - 48) * char_width;
		SDL_BlitSurface (font_img, &from, screen, &to);
		to.x+=char_width;
	}
}
