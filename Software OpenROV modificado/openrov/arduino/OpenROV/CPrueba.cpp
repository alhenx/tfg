#include "AConfig.h"
#if(HAS_STD_PRUEBA)

#include "comi2c.h"
#include <Arduino.h>
#include "CPrueba.h"
#include "CPin.h"
#include "NConfigManager.h"
#include "NModuleManager.h"
#include "NCommManager.h"
#include "NDataManager.h"
#if(HAS_STD_CAPE)
#include "CCape.h"
#endif
#if(HAS_OROV_CONTROLLERBOARD_25)
#include "CControllerBoard.h"
#endif

namespace {
	//Variables para almacenar los valores.
	float valorpH, valorEC, valorTemp;
	int valorORP;
	//Inicializacion de la clase.
	comi2c maestro(8);
}

//Funcion que inicializa la configuracion inicial.
void CPrueba::Initialize() {
	NConfigManager::m_capabilityBitmask |= ( 1 << PRUEBA_CAPABLE );
}

//Funcion que se ejecuta continuamente.
void CPrueba::Update(CCommand& commandIn) {
	//Se manda la orden para recibir el valor de los sensores.
	valorpH=maestro.request("ph");
	valorORP=maestro.request("orp");
	valorEC=maestro.request("ec");
	valorTemp=maestro.request("temp");
	NDataManager::m_navData.HDGD=maestro.request("comp");	//Se modifica el valor de la brújula por defecto por el nuevo.
	//Se envian los valores para mostrarlos en la interfaz.
	Serial.print( F("VALORPH:"));
	Serial.print(valorpH);
	Serial.println(';');
	Serial.print( F("VALORORP:"));
	Serial.print(valorORP);
	Serial.println(';');
	Serial.print(F("VALOREC:"));
	Serial.print(valorEC);
	Serial.println(';');
	Serial.print(F("VALORTEMP:"));
	Serial.print(valorTemp);
	Serial.println(';');
	Serial.print(F("COMPASSYAW:"));
	Serial.print(NDataManager::m_navData.HDGD);
	Serial.println(';');
	//Si se recibe el comando se envia la orden de encender o apagar el sistema de iluminacion.
	if(commandIn.Equals("ledaux")) {
		int value = commandIn.m_arguments[1];
		if (value==1) maestro.request("ledon");
		else maestro.request("ledoff");
	}
}
#endif