#include "HardwareSerial.h"
#include "BluetoothSerial.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "definitions.h"
#include "MPU.h"

BluetoothSerial SerialBT;

Adafruit_PWMServoDriver pwm;

class Servo{
	private:
		byte pin;

	public:

		static void init(){
			Wire.begin(I2C_SDA, I2C_SCL);
			Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);
			pwm.begin();
			pwm.setOscillatorFrequency(27000000);
			pwm.setPWMFreq(50);  // Analog servos run at ~50 Hz updates
		}

		void begin(byte pin){
			this->pin = pin;
		}

		float move1(float deg){

			float out = map_f(deg, 0, 180, map1_1, map1_2);
			byte test = pwm.setPWM(pin, 0, out);

			#if DebugServo
				DebugSerial.print("   Servo response:");
				DebugSerial.println(test);
			#endif

			return (((test == 0) * out) - (test*1000));
		}

		float move2(float deg){

			float out = map_f(deg, 0, 180, map2_1, map2_2);
			byte test = pwm.setPWM(pin, 0, out);

			#if DebugServo
				DebugSerial.print("   Servo response:");
				DebugSerial.println(test);
			#endif

			return (((test == 0) * out) - (test*1000));
		}

		float map_f(float x, float in_min, float in_max, float out_min, float out_max) {
			return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		}
};