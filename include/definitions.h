
#define I2C_SDA 21
#define I2C_SCL 22

#define DebugSerial Serial

#define showTps true
#define printTps false
#define targeTps 60

#define Debug false
#define DebugMPU false
#define DebugServo false
#define DebugLeg true
#define DebugBody false
#define DebugCoords false

#define Servo90 true

//Servo mapping
	#define map1_1 65
	#define map1_2 430

	#define map2_1 120
	#define map2_2 370


//body size:
	#define thighLength 76    //?
	#define thighLengthSq 5776  //?               //LegsegmentLength*LegsegmentLength
	#define lowerlegLength 100    //?
	#define lowerlegLengthSq 10000  //?               //LegsegmentLength*LegsegmentLength
	#define sholuderToThighOffset 10				  //offset between the center of rotation for the shoulder and the center of rotation for the thigh
	#define shoulderOffset 35.267
	#define shoulderToCenter_x 123.5 //?
	#define shoulderToCenter_y 65  //?
	#define shoulderToCenterDiagonal      140.89//?   //sqrt(sq(shoulderToCenter_x) + sq(shoulderToCenter_y))
	#define shoulderToCenterDiagonalAngel 62.5//?     //atan2(shoulderToCenter_x/shoulderToCenter_y)
	#define minLowerLegAngel 3.1 //?                   //the minimum angel that the lower leg kann be to the ground
	#define minLowerLegAngelSafty 2
	#define LegMin  20.0
	#define LegMax  167.5
	#define normalHight 117.480
	#define stepHight 25
	#define stepLength 75

//modes:
	#define balanceMode 1
	#define Speed 0.5  //mm/Millisekunden
	#define Speeddevide 1000000000


//overides:
	#if Debug
		#define DebugMPU true
		#define DebugServo true
		#define DebugLeg true
		#define DebugBody true
	#endif

	#define Debug_any (Debug || DebugMPU || DebugServo || DebugLeg || DebugBody || DebugCoords)

	#if DebugBody
		#define DebugCoords true
	#endif

	#if printTps
		#define showTps true
	#endif