#include <Arduino.h>
#include "sensorEC.h"
#include <OneWire.h>

#define StartConvert 0
#define ReadTemperature 1

const byte numReadings = 20;     //Numero de lecturas realizadas.
byte DS18B20_Pin = A3;	//Pin del sensor de temperatura.
unsigned int readings[numReadings];	//Vector donde se guardan los valores.
unsigned long AnalogValueTotal = 0;	//Valor de las lecturas.
unsigned int AnalogAverage = 0,averageVoltage=0;	//Valor medio de las lecturas y voltaje.
float temperature,ECcurrent;	//Temperatura y EC final.
 
//Iniciacion del sensor de temperatura.
OneWire ds(DS18B20_Pin);  

//Constructor de la clase que recibe el pin.
sensorEC::sensorEC(byte pinec){
	ECsensorPin = pinec;
}

//Funcion que calcula el EC y Temperatura.
void sensorEC::getVal(){
	byte index = 0;
	//Se realiza la lectura de datos, se guardan en el vector y se realiza la media.
	for(int i=0;i<numReadings;i++){
		AnalogValueTotal = AnalogValueTotal - readings[index];
		readings[index] = analogRead(ECsensorPin);
		AnalogValueTotal = AnalogValueTotal + readings[index];
		index = index + 1;
		AnalogAverage = AnalogValueTotal / numReadings;
	}

	//Se lee la temperatura y se resetea la memoria para la siguiente lectura de temperatura.
	temperature = TempProcess(ReadTemperature); 
	TempProcess(StartConvert);

	//Se realiza la conversion a EC mediante las formulas de compensacion facilitadas por el fabricante.
	averageVoltage=AnalogAverage*(float)5000/1024;
	float TempCoefficient=1.0+0.0185*(temperature-25.0); 
	float CoefficientVolatge=(float)averageVoltage/TempCoefficient;   
	if(CoefficientVolatge<=448)ECcurrent=6.84*CoefficientVolatge-64.32; 
	else if(CoefficientVolatge<=1457)ECcurrent=6.98*CoefficientVolatge-127; 
	else ECcurrent=5.3*CoefficientVolatge+2278;
	ECcurrent/=1000;
	ecVal=(float)ECcurrent;
	tempVal=(float)temperature;
}

//Funcion facilitada por el fabricante que calcula la temperatura.
float sensorEC::TempProcess(bool ch) {
	static byte data[12];
	static byte addr[8];
	static float TemperatureSum;
	if(!ch){
		ds.reset();
		ds.select(addr);
		ds.write(0x44,1);
	}
	else{  
		byte present = ds.reset();
		ds.select(addr);    
		ds.write(0xBE);         
		for (int i = 0; i < 9; i++) {
			data[i] = ds.read();
		}         
		ds.reset_search();           
		byte MSB = data[1];
		byte LSB = data[0];        
		float tempRead = ((MSB << 8) | LSB);
		TemperatureSum = tempRead / 16;
	}
	return TemperatureSum;  
	}
