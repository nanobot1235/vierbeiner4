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

void walking(double stepLength){

	double motion[][4][3] ={
		{
			{ 0, 0,  - stepHight},
			{ stepLength/3, 0, 0},
			{			 0, 0, 0},
			{-stepLength/3, 0, 0}
		},
		{
			{-stepLength/2, -balence_offset, 0},
			{ stepLength/2, -balence_offset, 0},
			{ stepLength/6, -balence_offset, 0},
			{-stepLength/6, -balence_offset, 0}
		},
		{
			{-stepLength/3, 0, 0},
			{ 0, 0,  - stepHight},
			{ stepLength/3, 0, 0},
			{			 0, 0, 0}
		},
		{
			{-stepLength/6, balence_offset, 0},
			{-stepLength/2, balence_offset, 0},
			{ stepLength/2, balence_offset, 0},
			{ stepLength/6, balence_offset, 0}
		},
		{
			{			 0, 0, 0},
			{-stepLength/3, 0, 0},
			{ 0, 0,  - stepHight},
			{ stepLength/3, 0, 0}
		},
		{
			{ stepLength/6, -balence_offset, 0},
			{-stepLength/6, -balence_offset, 0},
			{-stepLength/2, -balence_offset, 0},
			{ stepLength/2, -balence_offset, 0}
		},
		{
			{ stepLength/3, 0, 0},
			{			 0, 0, 0},
			{-stepLength/3, 0, 0},
			{ 0, 0,  - stepHight}
		},
		{
			{ stepLength/2, balence_offset, 0},
			{ stepLength/6, balence_offset, 0},
			{-stepLength/6, balence_offset, 0},
			{-stepLength/2, balence_offset, 0}
		}
	};
	byte test = animate(sizeof(motion), goal);
	memcpy(myExchange2.legCoords, motion[test], sizeof(myExchange2.legCoords));
}

void loop2(){

	#if showTps
		myExchange2.tps = (millis() - last);
		last = millis();
	#endif

	


	myExchange2 = getNewExchange(&myExchange2, true);

	goal = b1.moveto(myExchange2.legCoords, myExchange2.coords, myExchange2.MPU, Speed);

	if (myExchange2.walk[0] != 0){
		walking(myExchange2.walk[0]);
	}

	//moveServos();

	#if Debug_any
		DebugSerial.println();
	#endif
}