#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "definitions.h"
#include "animation.h"

Adafruit_MPU6050 mpu;
HardwareSerial *serialInUse = &Serial;

const int dataa = 50;

class MPU{
	private:

	int counter;


	double pitch_average_arr[dataa];
	double roll_average_arr[dataa];

	double calabration[2];


	public:

	bool datalog = false;

	double pitch;
	double roll;

	double pitch_average;
	double roll_average;

	void setup(int sda, int scl){
		Wire.begin (sda, scl);   // sda= GPIO_0 /scl= GPIO_4
		while (!mpu.begin()) {
			#if Debug || DebugMPU
				DebugSerial.println("Failed to find MPU6050 chip");
			#endif
		}
		mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
		mpu.setGyroRange(MPU6050_RANGE_250_DEG);
		mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
	}

	void reset_calabration(){
		calabration[0] = 0;
		calabration[1] = 0;
	}
	void calebrade(){
		get_data();

		calabration[0] = pitch;
		calabration[1] = roll;
	}

	void average(){
		get_data();

		pitch_average = 0;
		roll_average = 0;

		counter++;

		counter *= (counter != dataa);

		pitch_average_arr[counter] = pitch;
		roll_average_arr[counter] = roll;

		for (double d : pitch_average_arr){
			pitch_average += d;
		}

		for (double d : roll_average_arr){
			roll_average += d;
		}

		pitch_average = pitch_average / dataa;
		roll_average  = roll_average /  dataa;


		#if DebugMPU
			DebugSerial.print("  pitch:");
			DebugSerial.print(pitch);
			DebugSerial.print("  roll:");
			DebugSerial.print(roll);
			DebugSerial.print("  pitch_a:");
			DebugSerial.print(pitch_average);
			DebugSerial.print("  roll_a:");
			DebugSerial.println(roll_average);
		#endif
	}
	//private:

	void get_data(){
		sensors_event_t a, g, temp;
		mpu.getEvent(&a, &g, &temp);

		pitch = (-(atan2(a.acceleration.x, sqrt(a.acceleration.y*a.acceleration.y + a.acceleration.z*a.acceleration.z))*180.0)/M_PI) -calabration[0];
		roll = ((atan2(a.acceleration.y, a.acceleration.z)*180.0)/M_PI) -calabration[1];
	}
};
