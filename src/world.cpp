#include "world.h"

bool World::audio_enabled = true;
bool World::music_enabled = true;

const float World::S = sqrt((float)1/6);
const float World::L = sqrt((float)5/6);
const float World::M = sqrt((float)1/2);
	
//Movement stuff     Up        Right       Down         Left
const float World::XM[32]={0,0.2,0.38,0.56,0.71,0.83,0.92,0.98,1,0.98,0.92,0.83,0.71,0.56,0.38,0.2,0,-0.2,-0.38,-0.56,-0.71,-0.83,-0.92,-0.98,-1,-0.98,-0.92,-0.83,-0.71,-0.56,-0.38,-0.2};

const float World::YM[32]={-1,-0.98,-0.92,-0.83,-0.71,-0.56,-0.38,-0.2,0,0.2,0.38,0.56,0.71,0.83,0.92,0.98,1,0.98,0.92,0.83,0.71,0.56,0.38,0.2,0,-0.2,-0.38,-0.56,-0.71,-0.83,-0.92,-0.98};
