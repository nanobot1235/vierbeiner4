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

	void begin(float mapping[4], byte pins){

		#if DebugLeg
			this->pins = pins;
		#endif

		this->roto.begin (mapping, pins++);
		this->upper.begin(mapping, pins++);
		this->lower.begin(mapping, pins  );
	}

	void moveto_left(double coords[3], double leg_length){
		double servo_deg[3];
		servo_deg[0] = 90-(atan2((coords[1]),(coords[2]))*180/PI);
		servo_deg[2] = 180-(atan2((leg_length),(sqrt(LegsegmentLengthSq-sq(leg_length))))*2*180/PI);
		servo_deg[1] = 90+((servo_deg[2]/2)-((atan2((-coords[0]),(sqrt(sq(coords[1])+sq(coords[2])))))*180/PI));

		this->roto.move(servo_deg[0]);
		this->upper.move(servo_deg[1]);
		this->lower.move(servo_deg[2]);

		#if DebugLeg
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

	void moveto_right(double coords[3], double leg_length){
		double servo_deg[3];
		servo_deg[0] = 90-(atan2((coords[1]),(coords[2]))*180/PI);
		servo_deg[2] = (atan2((leg_length),(sqrt(LegsegmentLengthSq-sq(leg_length))))*2*180/PI);
		servo_deg[1] = 180-(90+((servo_deg[2]/2)-((atan2((-coords[0]),(sqrt(sq(coords[1])+sq(coords[2])))))*180/PI)));

		this->roto.move(servo_deg[0]);
		this->upper.move(servo_deg[1]);
		this->lower.move(servo_deg[2]);

		#if DebugLeg
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
