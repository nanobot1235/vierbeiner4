#include "exchange.h"

int animation_n = 0;
double animationMotion[4][3];

boolean lastnext = false;

byte animate(int size, boolean next){

	animation_n += next && !lastnext;

	lastnext = next;

	animation_n %= size/(sizeof(double)*12);

	return animation_n;
}