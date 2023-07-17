
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


//body size:
	#define LegsegmentLength 200    //?
	#define LegsegmentLengthSq 40000  //?               //LegsegmentLength*LegsegmentLength
	#define shoulderOffset 22.5
	#define shoulderToCenter_x 300 //?
	#define shoulderToCenter_y 150  //?
	#define shoulderToCenterDiagonal      335.41//?   //sqrt(sq(shoulderToCenter_x) + sq(shoulderToCenter_y))
	#define shoulderToCenterDiagonalAngel 63.434//?   //atan2(shoulderToCenter_x/shoulderToCenter_y)
	#define minLowerLegAngel 10 //?                   //the minimum angel that the lower leg kann be to the ground
	#define LegMin  130.0
	#define LegMax  397.5
	#define normalHight 282.84
	#define stepHight 100
	#define stepLength 200

//modes:
	#define balanceMode 1
	#define Speed 1.25  //mm/Millisekunden
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