class sensorORP{
	public:
		sensorORP(byte pin);
		float getORP();
	private:
		double avergearray(int* arr, int number);
		byte orpPin;
};	