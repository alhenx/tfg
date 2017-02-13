#include <Arduino.h>
#include "sensorpH.h"

#define OFFSET 0	//Numero de calibracion.

int buf[10];    //Vector que almacena 10 medidas para realizar su media.

//Constructor de la clase que recibe el pin.
sensorpH::sensorpH(byte pinph){
	sensorPin = pinph;
}

//Funcion que calcula el pH.
float sensorpH::getpH(){
	int temp;   //Variable temporal para el calculo de la media.
	//Se recopilan 10 datos y se guardan en un vector.
	for(int i=0;i<10;i++){
		buf[i]=analogRead(sensorPin);
		delay(10);
	}
	//Se ordena el vector de menor a mayor.
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			if(buf[i]>buf[j]){
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	unsigned long int avgValue=0;   //Variable que almacena el valor medio de las medidas.
	//Se escogen las 6 medidas centrales y se hace la media.
	for(int i=2;i<8;i++){
		avgValue+=buf[i];
	}
	avgValue=avgValue/6;
	//Se convierte el valor en milivoltios y luego en el valor pH.
	float phValue=(float)avgValue*5.0/1024;
	phValue=3.5*phValue+OFFSET;
	return phValue;
}