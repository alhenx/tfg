#include <Wire.h> //Libreria I2C.
#include <SD.h> //Libreria para manejar la SD.

//Librerias de sensores.
#include <sensorORP.h>  
#include <sensorpH.h> 
#include <sensorEC.h>
#include <HMC5883L_Simple.h>

//Librerias para el GPS.
#include <TinyGPS++.h>
#include <SoftwareSerial.h>


const byte SlaveDeviceId = 8; //ID del esclavo para la conexion I2C.
byte LastMasterCommand = 0; //Variable donde se guarda el comando enviado por el maestro.
const byte pinSD = 4;  //Variable con el pin al que se conecta la SD.
//Contador utilizado para el guardado en la SD.
long intervalo = 3000;        
long tiempo = 0;
long tiempoAnterior = 0;

//Constructores de los objetos sensores pasandole los pines al que estan conectados.
sensorORP sensor_orp(A2);
sensorEC sensor_ec(A1);
sensorpH sensor_ph(A0);
HMC5883L_Simple Compass;
TinyGPSPlus gps;
SoftwareSerial ss(3, 2);

//Inicializacion de variables
int valorORP, valorEC, valorCompass;  //Variables para los sensores.
float valorpH, valorTemp;
float valorLongitud, valorLatitud; //Variables para el GPS.
File myFile;  //Variable para el archivo de la SD.

void setup(){
  //Inicializacion de los componentes.
  sensor_orp.setup();
  sensor_ec.setup();
  sensor_ph.setup(); 
  //Opciones para calibrar el compass y muestre el verdadero norte.
  Compass.SetDeclination(23, 35, 'E');  
  Compass.SetSamplingMode(COMPASS_SINGLE);
  Compass.SetScale(COMPASS_SCALE_130);
  Compass.SetOrientation(COMPASS_HORIZONTAL_X_NORTH);  
  ss.begin(4800);     //Inicio del GPS.
  SD.begin(pinSD);    //Inicio de la SD.

  pinMode(9, OUTPUT); //Pin para el sistema de iluminacion configurado como salida.
  Wire.begin(SlaveDeviceId);  //Se inicia la conexion I2C con su ID.
  Wire.onReceive(receiveDataPacket); //Funcion para recibir peticiones I2C. 
  Wire.onRequest(slavesRespond);  //Funcion para responder por I2C.
}
  
void loop(){
  delay(100); //Delay de seguridad.

  //Funciones de las librerías de sensores para obtener los valores y tenerlos listos en caso de que el maestro los pida.
  valorORP = sensor_orp.getORP();
  valorpH = sensor_ph.getpH();
  sensor_ec.getVal();
  valorEC = sensor_ec.getEC();
  valorTemp = sensor_ec.getTemp();
  valorCompass=Compass.GetHeadingDegrees();

  

  //Cada 30 segundos se guarda la información en la SD.
  tiempo = millis();

    if(tiempo - tiempoAnterior > intervalo) {
      tiempoAnterior = tiempo;   
      //Si la comunicación con el GPS esta disponible se reciben los datos y se guardan.
      if (ss.available() > 0){
        valorLongitud = gps.location.lng();
        valorLatitud = gps.location.lat();
      }
  
      //Se abre el archivo y se guarda la información.
      myFile = SD.open("info.txt", FILE_WRITE);
      if (myFile) {
        myFile.print("Posición: ");
        myFile.print(valorLongitud);
        myFile.print(", ");
        myFile.print(valorLatitud);
        myFile.print(" - pH: ");
        myFile.print(valorpH);
        myFile.print(" - ORP: ");
        myFile.print(valorORP);
        myFile.print("mV - EC: ");
        myFile.print(valorEC);
        myFile.print("ms - Temperatura: ");
        myFile.print(valorTemp);
        myFile.println(" ºC");
      }
      myFile.close();
    }
}

//Funcion que recibe el comando de la accion a ejecutar del maestro.
void receiveDataPacket(int howMany){
  LastMasterCommand = Wire.read();
}

//Funcion para responder al maestro con el dato solicitado.
void slavesRespond(){
  int returnValue = 0; //Variable que se va a enviar al maestro como respuesta.

  //Switch encargado de guardar en la variable el valor deseado para enviarla posteriormente.
  //0 = pH
  //1 = ORP
  //2 = EC
  //3 = Temperatura
  //4 = Brujula
  //Y de realizar las siguientes funciones.
  //10 = Encender las luces
  //11 = Apagar las luces
  switch(LastMasterCommand){
    case 0: 
      returnValue = valorpH*100;
      break;
    case 1: 
      returnValue = valorORP;
      break;
    case 2: 
      returnValue = valorEC*100;
      break;
    case 3:
      returnValue = valorTemp*100;
      break;
    case 4:
      returnValue = valorCompass;
      break;
    case 10:
      digitalWrite(9, HIGH);
      break;
    case 11:
      digitalWrite(9, LOW);
      break;
  }

  //Se divide el dato en un buffer de dos bytes para poder enviar valores negativos y mayores a 255 a traves de I2C.
  byte buffer[2];            
  buffer[0] = returnValue >> 8;
  buffer[1] = returnValue & 255;
  Wire.write(buffer, 2); //Se envia el buffer al maestro.
}
