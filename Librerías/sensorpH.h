class sensorpH{
	public:
		sensorpH(byte pinph);
		float getpH();
	private:
		byte sensorPin;
};