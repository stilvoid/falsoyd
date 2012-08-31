#include "SDL.h"
#include "math.h"
#include <vector>
#include "audio.h"

class Bonus
{

	protected:
		std::vector<SDL_Surface*> image;	//The sprite image
		SDL_Surface *back;		//Backing store for blits
		SDL_Surface *playfield;		//Where to blit it
		SDL_Rect loc;			//Position
		int lives;			//Obvious!
		int bonus_type;
		int play_w;			//width of playfield
		int play_h;			//Take a guess
		void die(void);

		Sound_Chunk *spawn_sound;
		Sound_Chunk *death_sound;
		Sound_Chunk *get_sound;
	public:
		Bonus(SDL_Surface *buffer,int pw, int ph);
		void init(void);
		void move(void);
		void draw(SDL_Rect vport);
		SDL_Rect get_loc(void);
		void hit(int i);
		int get_lives(void);
		int collide(SDL_Rect pos);
		bool on_map(SDL_Rect onmap);
};
