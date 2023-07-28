class IO{
	private:

	HardwareSerial *serialInUse;

	double standertbodycoords[2][3] = {
		{0, 0, normalHight},
		{0, 0, 0}
	};

	public:
	double bodycoords[2][3];
	double walk[3];

	bool datalog = false;
	void begin(HardwareSerial *serialtoUse, int timeout){
		serialInUse = serialtoUse;
		serialInUse->setTimeout(timeout);
	}

	void input( double bodycoords[2][3], double walk[3]){
		memcpy(this->bodycoords, bodycoords, sizeof(this->bodycoords));
		memcpy(this->walk, walk, sizeof(this->walk));

		char use;
		float value;

		if (serialInUse->available() > 0){
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

				case 'o':
					if(digitalRead(sensePin)){
						digitalWrite(resetPin, HIGH);
						delay(50);
						digitalWrite(resetPin, LOW);
					}
					else{
						digitalWrite(setPin, HIGH);
						delay(50);
						digitalWrite(setPin, LOW);
					}
				break;

				case 'x':
					bodycoords[0][0] = value + standertbodycoords[0][0];
				break;

				case 'y':
					bodycoords[0][1] = value + standertbodycoords[0][1];
				break;

				case 'z':
					bodycoords[0][2] = value + standertbodycoords[0][2];
				break;

				case 'q':
					bodycoords[1][0] = value + standertbodycoords[1][0];
				break;

				case 'w':
					bodycoords[1][1] = value + standertbodycoords[1][1];
				break;

				case 'e':
					bodycoords[1][2] = value + standertbodycoords[1][2];
				break;

				case 'f':
					walk[0] = value;
				break;

				case 'g':
					walk[1] = value;
				break;

				case 'h':
					walk[2] = value;
				break;
			}

		}
	}
};
