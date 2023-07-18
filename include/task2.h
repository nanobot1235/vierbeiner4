#include "Body.h"
#include "core2.h"


double test[] = {0, 0, normalHight};

Body b1;

exchange myExchange2;

boolean goal;

#if showTps
	unsigned long last = 0;
#endif

void setup2(){
	b1.begin(0);
}

byte n = 0;

void moveServos(){

	double motion[][4][3] ={
		{
			{ stepLength/2, 0, 0},
			{-stepLength/4, 0,  - stepHight},
			{-stepLength/4, 0,  - stepHight},
			{ stepLength/2, 0, 0}
		},
		{
			{ stepLength/4, 0,  - stepHight},
			{-stepLength/2, 0, 0},
			{-stepLength/2, 0, 0},
			{ stepLength/4, 0,  - stepHight}
		},
		{
			{-stepLength/4, 0,  - stepHight},
			{ stepLength/2, 0, 0},
			{ stepLength/2, 0, 0},
			{-stepLength/4, 0,  - stepHight}
		},
		{
			{-stepLength/2, 0, 0},
			{ stepLength/4, 0,  - stepHight},
			{ stepLength/4, 0,  - stepHight},
			{-stepLength/2, 0, 0}
		}
	};

	if(b1.moveto(motion[n], myExchange2.coords, myExchange2.MPU, Speed)) {
		n++;
		n *= (n != (sizeof(motion)/96));
	}
}

void loop2(){

	#if showTps
		myExchange2.tps = (millis() - last);
		last = millis();
	#endif

	myExchange2.goal = goal;

	myExchange2 = getNewExchange(&myExchange2, true);

	goal = b1.moveto(myExchange2.legCoords, myExchange2.coords, myExchange2.MPU, Speed);

	//moveServos();

	#if Debug_any
		DebugSerial.println();
	#endif
}