#include "servo.h"

class Leg {
	private:
	Servo roto;
	Servo upper;
	Servo lower;

	#if DebugLeg
		byte pins;
	#endif

	public:

	void begin(byte pins){

		#if DebugLeg
			this->pins = pins;
		#endif

		this->roto.begin (pins++);
		this->upper.begin(pins++);
		this->lower.begin(pins  );
	}


	void moveto(double coords[3], double leg_length){
		double servo_deg[3];

		leg_length *= 2;
		
		double leg_lengthSq = sq(leg_length);
		double diagonal_length = sqrt(sq(coords[1]) + sq(coords[2]));

		double legangle = atan2(coords[0], (diagonal_length - sholuderToThighOffset))*180/PI;

		servo_deg[0] = 90 - atan2(coords[1], coords[2])*180/PI;

		servo_deg[1] =90  + (  legangle + (acos((leg_lengthSq + thighLengthSq    - lowerlegLengthSq) / (2 * leg_length * thighLength   ))*180/PI));
		servo_deg[2] =90  + (- legangle + (acos((leg_lengthSq + lowerlegLengthSq - thighLengthSq   ) / (2 * leg_length * lowerlegLength))*180/PI));

		this->roto .move1(servo_deg[0]);
		this->upper.move2(servo_deg[1]);
		this->lower.move1(servo_deg[2]);

		#if DebugLeg
			DebugSerial.print("\tleg_length");
			DebugSerial.print(pins/3);
			DebugSerial.print(" :");
			DebugSerial.print(leg_length);
			DebugSerial.print("\tLegangle");
			DebugSerial.print(pins/3);
			DebugSerial.print(" :");
			DebugSerial.print(legangle);
			DebugSerial.print("\tS");
			DebugSerial.print(pins);
			DebugSerial.print(" :");
			DebugSerial.print(servo_deg[0]);
			DebugSerial.print("\tS");
			DebugSerial.print(pins+1);
			DebugSerial.print(" :");
			DebugSerial.print(servo_deg[1]);
			DebugSerial.print("\tS");
			DebugSerial.print(pins+2);
			DebugSerial.print(" :");
			DebugSerial.print(servo_deg[2]);
		#endif
	}
};
