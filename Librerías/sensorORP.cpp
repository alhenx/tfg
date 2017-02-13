#include <Arduino.h>
#include "sensorORP.h"

#define VOLTAGE 5.00    //Voltaje del sistema.
#define ArrayLenth  40    //Numero de lecturas.
#define OFFSET 0	//Numero de calibracion.

double orpValue;	//Valor final de ORP.
int orpArray[ArrayLenth];	//Vector donde se guardan las lecturas.

//Constructor de la clase que recibe el pin.
sensorORP::sensorORP(byte pin){
	orpPin=pin;
}

//Funcion que calcula el ORP.
float sensorORP::getORP(){
	int orpArrayIndex=0;
	//Se recopilan las lecturas en un vector.
	for(int i=0;i<ArrayLenth;i++){
		orpArray[orpArrayIndex++]=analogRead(orpPin);    
	}
	//Se realiza la media y se guarda el valor.
	orpValue=((30*(double)VOLTAGE*1000)-(75*avergearray(orpArray, ArrayLenth)*VOLTAGE*1000/1024))/75-OFFSET; 
	return orpValue;
}

//Funcion que ordena el vector y calcula la media.
double sensorORP::avergearray(int* arr, int number){
	int i;
	int max,min;
	double avg;
	long amount=0;
	if(arr[0]<arr[1]){
		min = arr[0];max=arr[1];
	}
	else{
		min=arr[1];max=arr[0];
	}
	for(i=2;i<number;i++){
		if(arr[i]<min){
			amount+=min;
			min=arr[i];
		}else {
			if(arr[i]>max){
				amount+=max;
				max=arr[i];
			}else{
				amount+=arr[i];
			}
		}
	}
	avg = (double)amount/(number-2);
	return avg;
}