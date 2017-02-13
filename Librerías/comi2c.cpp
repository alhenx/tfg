#include "comi2c.h"
#include <Wire.h>

//Constructor que recibe la direccion del esclavo.
comi2c::comi2c(byte id){
	addr=id;	//Se asigna el identificador del esclavo para su posterior uso.
}

//Funcion que desempaqueta la respuesta para transformarla en un entero.
int comi2c::receiveResponse(){
	Wire.beginTransmission(addr);	//Se inicia la transmision con el esclavo.
	int receivedValue = Wire.read() << 8 | Wire.read();		//Desempaqueta la respuesta.
	Wire.endTransmission(true);		//Finaliza la transmision.
	return receivedValue;  
}

//Funcion que envia la orden al esclavo.
float comi2c::request(String op){
	int type=0;	//Existen dos tipos dependiendo del tipo de dato. 1 = int, 0 = float.
	byte DataPacket[1];
	if(op=="ph"){
		DataPacket[0] = 0;
		type=1;
	}
	if(op=="orp"){
		DataPacket[0] = 1;
		type=0;
	}
	if(op=="ec"){
		DataPacket[0] = 2;
		type=1;
	}
	if(op=="temp"){
		DataPacket[0] = 3;
		type=1;
	}
	if(op=="comp"){
		DataPacket[0] = 4;
		type=0;
	}
	if(op=="ledon"){
		DataPacket[0] = 10;
		type=0;
	}
	if(op=="ledoff"){
		DataPacket[0] = 11;
		type=0;
	}
	Wire.beginTransmission(addr);	//Inicia la transmision para enviar el comando.
	Wire.write(DataPacket, 1);		//Envia el comando.
	Wire.endTransmission(true);		//Finaliza la transmision.
	delay(10);
	float response = receiveResponse();		//Cuando la respuesta es desempaquetada se recibe.
	//Dependiendo del tipo de dato se realiza la siguiente operacion.
	if(type==0){
		return response;
	}else return response/100;
}