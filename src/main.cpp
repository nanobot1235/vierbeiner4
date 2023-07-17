#include "task2.h"
#include "Serial.h"

TaskHandle_t task_loop1;

exchange myExchange;

IO PC;
IOBT BT;

MPU mpu2;


#if printTps
	double tpsS[100];
	byte tpscounter;
#endif

double coords[2][3] = {
	{0, 0, normalHight},
	{0, 0, 0}
};

void setupCore2();
void senseBalance();
static void esploop1(void* pvParameters);
void xTaskCreatePinnedToCore(void* pvParameters);

void setup() {

	PC.begin(&Serial, 50);
	BT.begin(&SerialBT, 50);

	// put your setup code here, to run once:
	Serial.begin(115200);
	SerialBT.begin("Vierbeiner3");

	mpu2.setup(I2C_SDA, I2C_SCL);

	memcpy(myExchange.coords, coords, 48);
	myExchange = getNewExchange(&myExchange, false);

	#if showTps
		pinMode(17, OUTPUT);
	#endif

	setupCore2();
/*
	pwm.begin();*/
}

void walk(){
	double motion[][4][3] ={
		{
			{ stepLength/2, 0, 0},
			{-stepLength/3, 0,  - stepHight},
			{-stepLength/3, 0,  - stepHight},
			{ stepLength/2, 0, 0}
		},
		{
			{ stepLength/3, 0,  - stepHight},
			{-stepLength/2, 0, 0},
			{-stepLength/2, 0, 0},
			{ stepLength/3, 0,  - stepHight}
		},
		{
			{-stepLength/3, 0,  - stepHight},
			{ stepLength/2, 0, 0},
			{ stepLength/2, 0, 0},
			{-stepLength/3, 0,  - stepHight}
		},
		{
			{-stepLength/2, 0, 0},
			{ stepLength/3, 0,  - stepHight},
			{ stepLength/3, 0,  - stepHight},
			{-stepLength/2, 0, 0}
		}
	};

	byte test = animate(sizeof(motion), myExchange.goal);
	myExchange.goal = false;
	memcpy(myExchange.legCoords, motion[test], sizeof(myExchange.legCoords));
}

#if showTps
	boolean lastTps;
#endif

void loop() {
	PC.input(coords);
	memcpy(myExchange.coords, PC.bodycoords, sizeof(myExchange.coords));
	BT.input(coords);
	memcpy(myExchange.coords, BT.bodycoords, sizeof(myExchange.coords));

	#if showTps
		boolean tps = myExchange.tps > (1000/targeTps);
		#if printTps
			tpsS[tpscounter++] = 1000/(myExchange.tps + 0.00000000000001);
			tpscounter *= tpscounter!=sizeof(tpsS)/sizeof(double);

			double sum = 0;

			for (double d : tpsS){
				sum += d;
			}

			sum /= (sizeof(tpsS)/sizeof(double));
			DebugSerial.println(sum);
		#endif
		if (lastTps != tps) {
			digitalWrite(17, tps);
			lastTps = !lastTps;
		}
	#endif

	walk();

	//myExchange.coords = coords;
	senseBalance();
	myExchange = getNewExchange(&myExchange, false);
}

void senseBalance(){
	mpu2.average();
	myExchange.MPU[0] = mpu2.pitch_average;
	myExchange.MPU[1] = mpu2.roll_average;
}

void setupCore2() {
	bigLock = NULL;
	xTaskCreatePinnedToCore(
		esploop1,               /* Task function. */
		"loop1",                /* name of task. */
		10000,                  /* Stack size of task */
		NULL,                   /* parameter of the task */
		1,                      /* priority of the task */
		&task_loop1,            /* Task handle to keep track of created task */
		!xPortGetCoreID());     /* pin task to core 0 */
	bigLock = xSemaphoreCreateMutex();
}


static void esploop1(void* pvParameters) {
	setup2();

	while (1) {
		loop2();
	}
}