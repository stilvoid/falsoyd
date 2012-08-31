class Sight
{

	protected:
		SDL_Surface* image;		//The sprite image
		SDL_Surface* back;		//Backing store for blits
		SDL_Surface* playfield;		//Where to blit it
		SDL_Rect loc;			//Position
	public:
		Sight(SDL_Surface *buffer);
		void init(void);
		void move(SDL_Rect pos);
		void paste(void);
		void draw(void);
};

Sight::Sight(SDL_Surface *buffer)
{
	playfield=buffer;

	char temps[100];
	sprintf(temps,"%s/gfx/interface/sights.bmp",PREFIX);
	image=SDL_LoadBMP(temps);
	SDL_SetColorKey(image,SDL_SRCCOLORKEY|SDL_RLEACCEL,
		SDL_MapRGB(image->format,0xff,0xff,0xff));

	back=SDL_CreateRGBSurface(SDL_SWSURFACE,image->w,image->h,16,0,0,0,0);

}

void Sight::init(void)
{
	loc.x=playfield->w*2;
	loc.y=playfield->h*2;
	loc.w=image->w;
	loc.h=image->h;

	SDL_BlitSurface(playfield,&loc,back,NULL);
}

void Sight::move(SDL_Rect pos)
{
	loc.x=pos.x-image->w/2;
	loc.y=pos.y-image->h/2;
	SDL_BlitSurface(playfield,&loc,back,NULL);
}

void Sight::paste(void)
{
	SDL_BlitSurface(back,NULL,playfield,&loc);
}

void Sight::draw(void)
{
	SDL_BlitSurface(image,NULL,playfield,&loc);
}
