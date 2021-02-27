#include <SPI.h>
//#include <SD.h>
#include "SdFat.h"
SdFat SD;

#define SD_CS_PIN SS

void setup() {
 Serial.begin(9600);
 Serial.println("Initializing SD card...");

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
 
  
}

void loop() {
  pregunta_respuesta();
  delay(5000);

}



String leer_archivo(String ruta){ //elije una linea randomica 
  File dataFile;
  String dato;
  ruta += ".txt";
  dataFile = SD.open(ruta);
  if (dataFile){
      String dataLine = dataFile.readStringUntil('\n'); //Lee la primera línea del .txt
      dataLine = dataFile.readStringUntil('\n'); //Lee la primera línea del .txt
      int cant_lineas = dataLine.substring(0,dataLine.indexOf('\\')).toInt(); //Convierte el valor de la prímera línea a String
      randomSeed(analogRead(A0));
      int numero = random(1, cant_lineas + 1);// suma 3 , las 2 primeras no tienen datos y la ultima no esta incluida en el random
      int linea = 2;
      while (dataFile.available() && linea < numero+2){
        dataLine = dataFile.readStringUntil('\n');
        linea++;
      }
      dato = dataLine;
      dataFile.close();
    }else{
      Serial.println("Error al abrir el archivo");}//mostrar en lcd o led
  return dato;
}


String leer_archivo(String ruta, int num_linea){
  File dataFile;
  String dato;
  ruta += ".txt";
  dataFile = SD.open(ruta);
  if (dataFile){
      String dataLine = dataFile.readStringUntil('\n'); //Lee la primera línea del .txt
      dataLine = dataFile.readStringUntil('\n'); //Lee la primera línea del .txt
      int linea = 2;
      while (dataFile.available() && linea < num_linea+2){
        dataLine = dataFile.readStringUntil('\n');
        linea++;
      }
      dato = dataLine;
      dataFile.close();
    }else{
      Serial.println("Error al abrir el archivo");}//mostrar en lcd o led
  return dato;
}


String elegir_pregunta(String ruta){// nombre del mazo
  String pregunta=leer_archivo(ruta);
  //Serial.println(pregunta);
  return pregunta;

}

String * respuestas;
void buscar_respuesta(String ruta, int num_preg){//En la posicion 0 esta la respuesta correcta devuelve el resultado en variable global respuestas
   String cr = leer_archivo(ruta,0);
   int cant_resp = cr.substring(0, cr.indexOf('\\')).toInt();
   //String respuestas [cant_resp];
   respuestas = new String [cant_resp];
   int indice=cant_resp*(num_preg-1)+2;
   for  (int i=0 ; i<4 ; i++){
    //respuestas[i]= leer_archivo(ruta,indice);
    Serial.println(leer_archivo(ruta,indice+i));
    }
   //return respuestas;
  }


String ruta_respuesta="res";
String ruta_pregunta="pre";
boolean pregunta_respuesta(){
  boolean resultado=false;
  String pregunta=elegir_pregunta(ruta_pregunta);
  Serial.println(pregunta);
  int num_pregunta = pregunta.substring(0,pregunta.indexOf(';')).toInt();
  buscar_respuesta(ruta_respuesta, num_pregunta);
    
  return resultado;
  }
