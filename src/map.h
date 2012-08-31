#include <stdlib.h>

class Map
{
	private:
		SDL_Rect vport;
		SDL_Surface* screen;

		SDL_Surface** plane;
		SDL_Surface* buffer;

		void generate(void);

		int play_w;
		int play_h;
		int play_x;
		int play_y;

		float* x;
		float* y;

		int depth;

	public:
		void init(SDL_Surface* target,SDL_Surface* buff,int pw,int ph,SDL_Rect viewport,char* dname);
		void push(float xr,float yr);
		void warp(int xr,int yr);
		SDL_Rect get_loc(void);
		void draw(void);
};

SDL_Rect Map::get_loc(void)
{
	SDL_Rect p;
	p.x=(int)x;
	p.y=(int)y;
	p.w=play_w;
	p.h=play_h;

	return p;
}

void Map::init(SDL_Surface* target,SDL_Surface* buff,int pw,int ph,SDL_Rect viewport,char* dname)
{
	screen=target;

	depth=2;

	plane=new SDL_Surface*[depth];

	for(int i=0;i<depth;i++)
	{
		plane[i]=SDL_CreateRGBSurface(SDL_HWSURFACE,screen->w,screen->h,16,0,0,0,0);
	}
	buffer=SDL_CreateRGBSurface(SDL_SWSURFACE,screen->w,screen->h,16,0,0,0,0);

	x=new float[depth];
	y=new float[depth];

	for(int i=0;i<depth;i++){x[i]=0;y[i]=0;}

	play_w=pw;
	play_h=ph;
	play_x=0;
	play_y=0;

	
	generate();
}

void Map::push(float xr,float yr)
{
	for(int i=0;i<depth;i++)
	{
		x[i]+=xr*((float)(depth-i)/depth);
		y[i]+=yr*((float)(depth-i)/depth);

		while(x[i]<0){x[i]+=screen->w;}
		while(y[i]<0){y[i]+=screen->h;}
		while(x[i]>=screen->w){x[i]-=screen->w;}
		while(y[i]>=screen->h){y[i]-=screen->h;}
	}
}

void Map::warp(int xr, int yr)
{
	float dx=0;
	float dy=0;

	dx=xr-play_x;
	dy=yr-play_y;

	if(dx > play_w/2)
	{
		dx=play_x+(play_w-xr);
	}
	if(dx < play_w/-2)
	{
		dx=xr+(play_w-play_x);
	}
	if(dy > play_h/2)
	{
		dy=play_y+(play_h-yr);
	}
	if(dy < play_h/-2)
	{
		dy=yr+(play_h-play_y);
	}

	play_x=xr;
	play_y=yr;

	push(dx/1,dy/1);
}

void Map::generate(void)
{
	SDL_Rect temp;
	int col=0;
	int rndcol=0;

	temp.x=0;
	temp.y=0;
	temp.w=screen->w;
	temp.h=screen->h;

	SDL_FillRect(screen,&temp,SDL_MapRGB(screen->format,0x00,0x00,0x00));

	for(int i=0;i<depth;i++)
	{
		temp.x=0;
		temp.y=0;
		temp.w=buffer->w;
		temp.h=buffer->h;
		SDL_FillRect(buffer,&temp,SDL_MapRGB(buffer->format,0x00,0x00,0x00));
		for(int j=0;j<(plane[i]->w * plane[i]->h)/(500*((float)(depth-i)/depth));j++)
		{
			temp.x=rand()%plane[i]->w;
			temp.y=rand()%plane[i]->h;
			temp.w=1;
			temp.h=1;
			col=(int)((float)(rand()%200)*((float)(depth-i)/depth)+56);

			if(rand()%100 < 5)
			{
				temp.w=2;
				temp.h=2;
			}

			rndcol=rand()%13-7;

			SDL_FillRect(plane[i],&temp,SDL_MapRGB(plane[i]->format,col+rndcol,col-rndcol,col-rndcol));
		}
		SDL_SetColorKey(plane[i],SDL_SRCCOLORKEY,SDL_MapRGB(plane[i]->format,0x00,0x00,0x00));
	}
}

void Map::draw(void)
{
	SDL_Rect loc;
	SDL_Rect dest;

	int j;

	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0x00,0x00,0x00));

	for(int i=depth-1;i>=0;i--)
	{
		//Top

		//Left side
		loc.x=(int)x[i];
		loc.y=(int)y[i];
		loc.w=screen->w-(int)x[i];
		loc.h=screen->h-(int)y[i];

		dest.x=0;
		dest.y=0;
		dest.w=loc.w;
		dest.h=loc.h;

		SDL_BlitSurface(plane[i],&loc,screen,&dest);

		//Right side
		loc.x=0;
		loc.y=(int)y[i];
		loc.w=(int)x[i];
		loc.h=screen->h-(int)y[i];

		dest.x=screen->w-(int)x[i];
		dest.y=0;
		dest.w=loc.w;
		dest.h=loc.h;

		SDL_BlitSurface(plane[i],&loc,screen,&dest);

		//Bottom
			
		//Left side
		loc.x=(int)x[i];
		loc.y=0;
		loc.w=screen->w-(int)x[i];
		loc.h=(int)y[i];

		dest.x=0;
		dest.y=screen->h-(int)y[i];
		dest.w=loc.w;
		dest.h=loc.w;

		SDL_BlitSurface(plane[i],&loc,screen,&dest);

		//Right side
		loc.x=0;
		loc.y=0;
		loc.w=(int)x[i];
		loc.h=(int)y[i];

		dest.x=screen->w-(int)x[i];
		dest.y=screen->h-(int)y[i];
		dest.w=loc.w;
		dest.h=loc.w;

		SDL_BlitSurface(plane[i],&loc,screen,&dest);
	}
}
