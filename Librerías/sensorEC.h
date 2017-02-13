class sensorEC{
	public:
		sensorEC(byte pinec);
		void getVal();
		float getEC(){return ecVal;}
		float getTemp(){return tempVal;}
	private:
		byte ECsensorPin;
		float ecVal;
		float tempVal;
		float TempProcess(bool ch);
};