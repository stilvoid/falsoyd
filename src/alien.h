#include "sprite.h"

class Alien: public Sprite
{
	private:
		Shot* bullets;
		int shoot_count;
		int shoot_speed;
		int move_type;
		int speed;
		bool in_range;
		int dir;
		int play_w;
		int play_h;
	public:
		Alien(SDL_Surface* s,int pw,int ph,Shot* bulls,vector<SDL_Surface*> &blocks);
		void init(SDL_Rect pos,int l,int num,vector<SDL_Surface*> &blocks,int move,int s);
		void move(SDL_Rect pos,float tf);
};
