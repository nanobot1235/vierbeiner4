#include "task2.h"
#include "Serial.h"

TaskHandle_t task_loop1;

exchange myExchange;

IO PC;
IO BT;

MPU mpu2;

#if Servo90
	Servo calibrate0;
	Servo calibrate90;
	Servo calibrate180;
#endif

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

	#if Servo90
		calibrate0  .begin(13);
		calibrate90 .begin(14);
		calibrate180.begin(15);
	#endif

	PC.begin(&Serial, 50);
	BT.begin(&Serial2, 50);

	Serial.begin(115200);
	Serial2.begin(115200);

	mpu2.setup(I2C_SDA, I2C_SCL);

	memcpy(myExchange.coords, coords, 48);
	myExchange = getNewExchange(&myExchange, false);

	#if showTps
		pinMode(tpsPin, OUTPUT);
	#endif

	pinMode(setPin, OUTPUT);
	pinMode(resetPin, OUTPUT);

	digitalWrite(setPin, LOW);
	digitalWrite(resetPin, HIGH);
	digitalWrite(resetPin, LOW);

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

	#if Servo90
		calibrate0  .move1(0);
		calibrate90 .move1(90);
		calibrate180.move1(180);
	#endif

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
			digitalWrite(tpsPin, tps);
			lastTps = tps;
		}
	#endif

	//walk();

	//myExchange.coords = coords;
	senseBalance();
	myExchange = getNewExchange(&myExchange, false);
}

void senseBalance(){
	mpu2.average();
	myExchange.MPU[1] = mpu2.pitch_average;
	myExchange.MPU[0] = mpu2.roll_average;
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