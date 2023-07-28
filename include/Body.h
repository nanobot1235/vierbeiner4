#include "Legs.h"

class Body {
	private:
	Leg front_left;
	Leg front_right;
	Leg back_left;
	Leg back_right;


	double leg_min;
	double newleg_min[4];
	double leg_length[4];
	double orientation;
	double coords[4][3];
	double lastcoords[4][3];
	unsigned long lastMove;

	float mapf(float x, float in_min, float in_max, float out_min, float out_max){
			return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

	void newleg_length(double coords[4][3]){
 
		leg_length[0] = sqrt(sq(coords[0][0])   + sq(sqrt(sq(coords[0][1])  + sq(coords[0][2])) - sholuderToThighOffset))/2;
		leg_length[1] = sqrt(sq(coords[1][0])   + sq(sqrt(sq(coords[1][1])  + sq(coords[1][2])) - sholuderToThighOffset))/2;
		leg_length[2] = sqrt(sq(coords[2][0])   + sq(sqrt(sq(coords[2][1])  + sq(coords[2][2])) - sholuderToThighOffset))/2;
		leg_length[3] = sqrt(sq(coords[3][0])   + sq(sqrt(sq(coords[3][1])  + sq(coords[3][2])) - sholuderToThighOffset))/2;

		{
			double L3 = (minLowerLegAngel+orientation+minLowerLegAngelSafty)*PI/180; //3
			double sin3 = sin(L3); //3
			double cos3 = cos(L3); //3

			for(byte i = 0; i<4 ; i++){
				if (coords[i][0] < 0){
					newleg_min[0] = sqrt(
										sq(coords[i][0]) //7²
										+sq(
											(sin3 * lowerlegLength) //2
											+sqrt(
												thighLengthSq 
												-sq(
													(cos3 * lowerlegLength) //1
													+coords[i][0] //7
												) //4²
											) //5
										) //6²
									); //8 bzw. leglength
				}
			}
		}


		#if DebugBody
			DebugSerial.println("leg_length:");
			for (double d : leg_length){
				DebugSerial.println(d*2);
			}
		#endif

		#if DebugBody
			DebugSerial.println("newleg_min:");
			for (double d : newleg_min){
				DebugSerial.println(d);
			}
		#endif


		leg_min = newleg_min[0];
		for (double d : newleg_min){
			leg_min = min(d, leg_min);
		}

		double minVal = max(LegMin/2, leg_min/2);
		double maxVal = LegMax/2;


		for (double d : leg_length){
			minVal= min(d, minVal);
			maxVal= max(d, maxVal);
		}

		leg_length[0] = (double)(mapf(leg_length[0],minVal ,maxVal,max(LegMin, leg_min)/2,LegMax/2));
		leg_length[1] = (double)(mapf(leg_length[1],minVal ,maxVal,max(LegMin, leg_min)/2,LegMax/2));
		leg_length[2] = (double)(mapf(leg_length[2],minVal ,maxVal,max(LegMin, leg_min)/2,LegMax/2));
		leg_length[3] = (double)(mapf(leg_length[3],minVal ,maxVal,max(LegMin, leg_min)/2,LegMax/2));

		//leg_length[2] += offsets[2];

		//leg_length[2] = min(leg_length[2], 45);

		//Serial.println(leg_length[2]);

		#if DebugBody
			DebugSerial.println("leg_length:");
			for (double d : leg_length){
				DebugSerial.println(d*2);
			}
		#endif
	}

	void coordconvert(double bodycoords[2][3], double legcoords[4][3]){

		double rotation_length_for_z[3];                         //the amount of movment needed for the rotation on the z axis(xyz)

		//rotation:
			rotation_length_for_z[0] = sin(bodycoords[1][1]/180*PI) * shoulderToCenter_x;
			rotation_length_for_z[1] = sin(bodycoords[1][0]/180*PI) * shoulderToCenter_y;

			rotation_length_for_z[2] = (sin(bodycoords[1][2]/180*PI) * shoulderToCenterDiagonal) / cos(bodycoords[1][2]/2/180*PI);                              //calculate the amount of moment needed for rotating on the z axis
		//end

		coords[0][2] =  legcoords[0][2] + bodycoords[0][2] + rotation_length_for_z[0] + rotation_length_for_z[1];
		coords[0][0] =  legcoords[0][0] + bodycoords[0][0] + (sin(bodycoords[1][1]/180*PI)*(coords[0][2])) + cos((shoulderToCenterDiagonalAngel - bodycoords[1][2]/2)/180*PI) * rotation_length_for_z[2];
		coords[0][1] =  legcoords[0][1] + (bodycoords[0][1]) + (shoulderOffset) + (sin(bodycoords[1][0]/180*PI)*(coords[0][2])) - sin((shoulderToCenterDiagonalAngel + bodycoords[1][2]/2)/180*PI) * rotation_length_for_z[2];                       //the shoulderOffset is added to position the foot unter the shoulder

		coords[1][2] =  legcoords[1][2] + bodycoords[0][2] + rotation_length_for_z[0] - rotation_length_for_z[1];
		coords[1][0] =  legcoords[1][0] + bodycoords[0][0] + (sin(bodycoords[1][1]/180*PI)*(coords[1][2])) - cos((shoulderToCenterDiagonalAngel - bodycoords[1][2]/2)*-1/180*PI) * rotation_length_for_z[2];
		coords[1][1] =  legcoords[1][1] + (bodycoords[0][1]) - (shoulderOffset) + (sin(bodycoords[1][0]/180*PI)*(coords[1][2])) + sin((shoulderToCenterDiagonalAngel + bodycoords[1][2]/2)*-1/180*PI) * rotation_length_for_z[2];                    //the shoulderOffset is added to position the foot unter the shoulder

		coords[2][2] =  legcoords[2][2] + bodycoords[0][2] - rotation_length_for_z[0] + rotation_length_for_z[1];
		coords[2][0] =  legcoords[2][0] + bodycoords[0][0] + (sin(bodycoords[1][1]/180*PI)*(coords[2][2])) + cos((shoulderToCenterDiagonalAngel - bodycoords[1][2]/2)/180*PI) * rotation_length_for_z[2];
		coords[2][1] = -legcoords[2][1] + (bodycoords[0][1]*-1) - (shoulderOffset) - (sin(bodycoords[1][0]/180*PI)*(coords[2][2])) - (sin((shoulderToCenterDiagonalAngel + bodycoords[1][2]/2)/180*PI) * rotation_length_for_z[2]);                  //the shoulderOffset is added to position the foot unter the shoulder

		coords[3][2] =  legcoords[3][2] + bodycoords[0][2] - rotation_length_for_z[0] - rotation_length_for_z[1];
		coords[3][0] =  legcoords[3][0] + bodycoords[0][0] + (sin(bodycoords[1][1]/180*PI)*(coords[3][2])) - cos((shoulderToCenterDiagonalAngel - bodycoords[1][2]/2)*-1/180*PI) * rotation_length_for_z[2];
		coords[3][1] = -legcoords[3][1] + (bodycoords[0][1]*-1) + (shoulderOffset) - (sin(bodycoords[1][0]/180*PI)*(coords[3][2])) + sin((shoulderToCenterDiagonalAngel + bodycoords[1][2]/2)*-1/180*PI) * rotation_length_for_z[2];                 //the shoulderOffset is added to position the foot unter the shoulder



	}

	boolean maxSpeed(double maxspeed){

		double movement[4];
		double absCoords[4][3];

		for (byte j=0; j<4; j++){
			for (byte i=0; i<3; i++){
				absCoords[j][i] = coords[j][i] - lastcoords[j][i];
			}
		}


		for (byte b = 0; b < 4; b++){
			movement[b] = sqrt(sq(absCoords[b][0]) + sq(absCoords[b][1]) + sq(absCoords[b][2]));
		}

		double maxspeed2 = maxspeed * (this->lastMove - millis()) / Speeddevide;

		this->lastMove = millis();

		double highest = maxspeed2;

		for (byte b = 0; b < 4; b++){
			highest = max(highest, movement[b]);
		}

		for (byte j = 0; j < 4; j++){
			for(byte i = 0; i < 3; i++){
				absCoords[j][i] *= (maxspeed2/highest);
				coords[j][i] = lastcoords[j][i] + absCoords[j][i];
			}
		}

		return (maxspeed2 == highest);
	}

	void moveto_backkend(double newcoords[4][3]){
		memcpy(lastcoords, newcoords, sizeof(lastcoords));
		newleg_length(newcoords);

		#if DebugCoords
			for (int i = 0; i<4; i++){
				for (int j = 0; j<3; j++){
					char c = i+97;
					char c2 = j+120;
					DebugSerial.print(c);
					DebugSerial.print(c2);
					DebugSerial.print(": ");
					DebugSerial.print(newcoords[i][j]);
					DebugSerial.print(",\t");
				}
			}
		#endif

		front_left .moveto_left (newcoords[0], leg_length[0]);
		front_right.moveto_right(newcoords[1], leg_length[1]);
		back_left  .moveto_left (newcoords[2], leg_length[2]);
		back_right .moveto_right(newcoords[3], leg_length[3]);
	}

	public:

	void begin(byte pins){
		Servo::init();

		front_left .begin(pins   );
		front_right.begin(pins+=3);
		back_left  .begin(pins+=3);
		back_right .begin(pins+=3);
	}

	boolean moveto(double newcoords[2][3], double balance[2], double maxspeed){
		double d[4][3];
		return moveto(newcoords, d, balance, maxspeed);
	}

	boolean moveto(double legcoords[4][3], double newcoords[2][3], double balance[2], double maxspeed){
		this->orientation = balance[1];

		double bodycoords[2][3];
		double newlegcoords[4][3];

		memcpy(bodycoords, newcoords, sizeof(bodycoords));
		memcpy(newlegcoords, legcoords, sizeof(newlegcoords));

		balance[0] += bodycoords[1][0];
		balance[1] += bodycoords[1][1];

		#if(balanceMode == 0)
			bodycoords[1][0] += balance[0];
			bodycoords[1][1] += balance[1];
		#endif

		#if(balanceMode == 1)
			bodycoords[0][1] += (tan(balance[0]/180*PI)*bodycoords[0][2]);
			bodycoords[0][0] -= (tan(balance[1]/180*PI)*bodycoords[0][2]);
		#endif

		#if(balanceMode == 2)
			bodycoords[1][0] += balance[0];
			bodycoords[1][1] += balance[1];

			bodycoords[0][1] += (tan(balance[0]/180*PI)*bodycoords[0][2]);
			bodycoords[0][0] -= (tan(balance[1]/180*PI)*bodycoords[0][2]);
		#endif

		boolean b = true;
		coordconvert(bodycoords, newlegcoords);
		b = maxSpeed(maxspeed);
		moveto_backkend(coords);
		return b;
	}
};