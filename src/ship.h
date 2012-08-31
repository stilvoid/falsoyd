#include "sprite.h"

class Ship: public Sprite
{
	private:
		Shot* bullets;
		float thrust_speed;
		int max_lives;
		int shoot_count;
		int max_shoot_speed;
		float shoot_speed;
		int death_count;
		int death_max;
		int play_w;
		int play_h;
		bool death_on;
		int reverse_count;
		int weapon;
		float x_vel;
		float y_vel;
		double speed;
	public:
		Ship(SDL_Surface *s,int pw,int ph,Shot* bulls,vector<SDL_Surface*> &blocks);
		void init(SDL_Rect pos,int l,int num);
		void move(int mx,int my,bool thrusting,bool shoot,bool death,float tf);
		bool collide(SDL_Rect pos);
		int get_death(void);
		float get_lifeGrade(void);
		void give_bonus(int b);
};
