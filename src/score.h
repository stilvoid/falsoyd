#include "SDL.h"

class Score //Used for drawing text on screen (oddly enough)
{
	private:
		SDL_Surface* screen;
		SDL_Rect viewport;
		SDL_Surface* font_img;
		int char_width;
	public:
		Score(void);
		void Init(SDL_Surface* s,SDL_Rect v);
		void Print(int score);
};
