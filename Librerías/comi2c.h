#include <Arduino.h>

class comi2c{
	public:
		comi2c(byte id);
		float request(String op);
	private:
		int receiveResponse();
		byte addr;
};