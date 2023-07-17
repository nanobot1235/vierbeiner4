class IO{
	private:

	HardwareSerial *serialInUse;

	double standertbodycoords[2][3] = {
		{0, 0, normalHight},
		{0, 0, 0}
	};

	public:
	double bodycoords[2][3];

	bool datalog = false;
	void begin(HardwareSerial *serialtoUse, int timeout){
		serialInUse = serialtoUse;
		serialInUse->setTimeout(timeout);
	}

	void input( double bodycoords[2][3]){
		memcpy(this->bodycoords, bodycoords, sizeof(this->bodycoords));

		char use;
		float value;

		if (serialInUse->available() > 0){
			//if (problem.forif(10)){
				value = serialInUse->parseFloat();
				use = serialInUse->read();
				if (datalog){
					serialInUse->print(use);
					serialInUse->println(value);
				}

				switch (use) {

					case '/':
						datalog = !datalog;
					break;

					case 'r':
						 memcpy(bodycoords, standertbodycoords, 48);
					break;

					case 'x':
						bodycoords[0][0] += value;
					break;

					case 'y':
						bodycoords[0][1] += value;
					break;

					case 'z':
						bodycoords[0][2] += value;
					break;

					case 'q':
						bodycoords[1][0] += value;
					break;

					case 'w':
						bodycoords[1][1] += value;
					break;

					case 'e':
						bodycoords[1][2] += value;
					break;
				}
			//}
		}
	}
};

class IOBT{
	private:

	BluetoothSerial *serialInUse;

	double standertbodycoords[2][3] = {
		{0, 0, normalHight},
		{0, 0, 0}
	};

	public:
	double bodycoords[2][3];

	bool datalog = false;
	void begin(BluetoothSerial *serialtoUse, int timeout){
		serialInUse = serialtoUse;
		serialInUse->setTimeout(timeout);
	}

	void input( double bodycoords[2][3]){
		memcpy(this->bodycoords, bodycoords, sizeof(this->bodycoords));

		char use;
		float value;

		if (serialInUse->available() > 1){
			//if (problem.forif(10)){
				value = serialInUse->parseFloat();
				use = serialInUse->read();

				for (byte b = 0; b < 2; b++){
					for (double d : this->bodycoords[b]){
						DebugSerial.println(d);
					}
				}

				if (datalog){
					serialInUse->print(use);
					serialInUse->println(value);
				}

				switch (use) {

					case '/':
						datalog = !datalog;
					break;

					case 'r':
						 memcpy(bodycoords, standertbodycoords, 48);
					break;

					case 'x':
						bodycoords[0][0] += value;
					break;

					case 'y':
						bodycoords[0][1] += value;
					break;

					case 'z':
						bodycoords[0][2] += value;
					break;

					case 'q':
						bodycoords[1][0] += value;
					break;

					case 'w':
						bodycoords[1][1] += value;
					break;

					case 'e':
						bodycoords[1][2] += value;
					break;
				}

				for (byte b = 0; b < 2; b++){
					for (double d : this->bodycoords[b]){
						DebugSerial.println(d);
					}
				}
			//}
		}
	}
};
