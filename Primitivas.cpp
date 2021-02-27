#include "Primitivas.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "Servo.h"
#include "Matriz.h"
#include "SD.h"

SoftwareSerial Serial4(47, 49); // RX, TX  define el puerto serial para el jugador 4
SoftwareSerial Monitor(46, 48); // RX, TX  define el puerto serial para el monitor


void guardar_pos_carro(byte x, byte y){
    //guarda la posicion (x, y) del carro en las posciones 0 y 1 de la eprom 
    EEPROM.write(0, x);
    EEPROM.write(1, y);
  }

void leer_pos_carro(int &x, int &y){
    //lee las posiciones 0 y 1 de la eprom y las guarda en las pocisiones (x, y) del carro
    x = EEPROM.read(0);
    y = EEPROM.read(1);
  }


void color_led(char color, int rled, int gled, int bled){ //enciende el led rgb de acuerdo al color ingresado
  switch(color){
    case 'Y': {
                analogWrite(rled,200); // Si color == 'y' se enciende color amarillo
                analogWrite(gled,128); // 
                digitalWrite(bled,LOW);
    break;}
    case 'R': {
                digitalWrite(rled,HIGH); // Si color == 'r' se enciende color rojo
                digitalWrite(gled,LOW); // 
                digitalWrite(bled,LOW);
    break;}
    case 'G': {
                digitalWrite(rled,LOW); // Si color == 'g' se enciende color verde
                digitalWrite(gled,HIGH); //
                digitalWrite(bled,LOW);
    break;}
    case 'B': {
                digitalWrite(rled,LOW); // Si color == 'b' se enciende color azul
                digitalWrite(gled,LOW); // 
                digitalWrite(bled,HIGH);
    break;}
    case 'L': {
                digitalWrite(rled,LOW); // Si color == 'C' se enciende color celeste
                digitalWrite(gled,HIGH); 
                digitalWrite(bled,HIGH);
    break;}
    case 'W': {
                digitalWrite(rled,HIGH); // Si color == 'W' se enciende color blanco
                digitalWrite(gled,HIGH); 
                digitalWrite(bled,HIGH);
    break;}
    case 'P': {
                digitalWrite(rled,HIGH); // Si color == 'P' es color violeta
                digitalWrite(gled,LOW); 
                digitalWrite(bled,HIGH);
    break;}
    case 'O': {
                digitalWrite(rled,LOW); // Si color == 'O' se apagan
                digitalWrite(gled,LOW); 
                digitalWrite(bled,LOW);
    break;}
     
  }
}

void agarrar_ficha(Servo &servo){//carro agarra la ficha
    servo.write(115); //120 GRADOS 
    delay(300);
}

void soltar_ficha(Servo &servo){//carro suelta la ficha
    servo.write(180);
    delay(300);
}

boolean jugador_disponible(Jugador j){  //Determina si el jugador esta disponible con el modulo bluetooth
  pinMode(j.pin_jugador, INPUT);
  return digitalRead(j.pin_jugador);
}

void enviar_mensaje(Jugador j, String mensaje){
  switch(j.numero_jugador){
     case 1:{  Serial1.println(mensaje);
     break;}
     case 2:{  Serial2.println(mensaje);
     break;}
     case 3:{  Serial3.println(mensaje);
     break;}
     case 4:{  Serial4.println(mensaje);
     break;} 
  }
}

String recibir_mensaje(Jugador j){
  String mensaje ="";
  switch(j.numero_jugador){
     case 1:{
                mensaje = Serial1.readStringUntil('.');
     break;}
     case 2:{
                mensaje = Serial2.readStringUntil('.');
     break;}
     case 3:{
                mensaje = Serial3.readStringUntil('.');
     break;}
     case 4:{
                mensaje = Serial4.readStringUntil('.');
     break;}
  }

  return mensaje;
}

String tirar_moneda(Matriz matriz, byte num_mat, int tiempo){  //Tira una moneda en una matriz y determina el resultado obtenido
  String valor_moneda [2] = {"cruz", "cara"};
  int veloc_tir;
  if(tiempo==500){veloc_tir=20;}
  if(tiempo==250){veloc_tir=40;}
  if(tiempo==100){veloc_tir=60;}
  for(int i=0; i<veloc_tir; i++){
    matriz.dibujar_simbolo(num_mat,'X');//ver numero matriz
    delay(tiempo);
    matriz.dibujar_simbolo(num_mat,'O');
    delay(tiempo);
    }
  randomSeed(analogRead(A0)); //no conectar nada en A0
  int resultado = random(0,2);
  if(resultado==0){
    matriz.dibujar_simbolo(num_mat,'X');}//0 cruz
  else{
    matriz.dibujar_simbolo(num_mat,'O');}//1 cara
  return valor_moneda [resultado];
  }

/////////////////Matriz dado arcade
String tirar_dado_arcade(Matriz matriz, byte num_mat, int tiempo){  //Tira un dado arcade en una matriz y determina el resultado obtenido
  String valor_dado_arcade [5] = {"espada","escudo","casco","pregunta","corona"};
  int veloc_tir;
  if(tiempo==500){veloc_tir=20;}
  if(tiempo==250){veloc_tir=40;}
  if(tiempo==100){veloc_tir=60;}
  for(int i=0; i<veloc_tir; i++){
    matriz.dibujar_simbolo(num_mat,'E');//ver numero matriz
    delay(tiempo);
    matriz.dibujar_simbolo(num_mat,'S');
    delay(tiempo);    
    matriz.dibujar_simbolo(num_mat,'H');
    delay(tiempo);
    matriz.dibujar_simbolo(num_mat,'Q');
    delay(tiempo);  
    matriz.dibujar_simbolo(num_mat,'C');
    delay(tiempo);      
    }
  randomSeed(analogRead(A0)); //no conectar nada en A0
  int resultado = random(0,5);
  if(resultado==0){
    matriz.dibujar_simbolo(num_mat,'E');}//0 espada
  if(resultado==1){
    matriz.dibujar_simbolo(num_mat,'S');}//1 escudo
  if(resultado==2){
    matriz.dibujar_simbolo(num_mat,'H');}//2 casco
  if(resultado==3){
    matriz.dibujar_simbolo(num_mat,'Q');}//3 pregunta
  else{
    matriz.dibujar_simbolo(num_mat,'C');}//4 corona
  return valor_dado_arcade [resultado]; 
  }
///////////////////////////////////

//////////////////Matriz flechas
void flechas_matriz(Matriz matriz, byte num_mat, char pos){  //Dibuja diferentes flechas en la matriz
   switch (pos){
    case 'R' :{
        matriz.dibujar_simbolo(num_mat,'R');//Derecha
    break;}
    case 'U' :{
        matriz.dibujar_simbolo(num_mat,'U');//Arriba/Adelante
    break;}
    case 'L' :{
        matriz.dibujar_simbolo(num_mat,'L');//Izquierda
    break;}
    case 'D' :{
        matriz.dibujar_simbolo(num_mat,'D');//Abajo/Atras
    break;}
  }
}
//////////////////////////////////

byte tirar_dado(Matriz matriz, byte lados, byte num_mat, int tiempo){  //Tira un dado en una matriz y determina el resultado obtenido
  randomSeed(analogRead(A0));
  int veloc_tir;
  if(tiempo==500){veloc_tir=20;}
  if(tiempo==250){veloc_tir=40;}
  if(tiempo==100){veloc_tir=60;}
  byte numero;
  for(int i=0; i<veloc_tir; i++){
    numero = random(1,lados + 1);
    matriz.dibujar_simbolo(num_mat,numero);
    delay(tiempo);
    }
  return numero;
  }

void perder_turno(Jugador &j, int cantidad){ // Incrementa los turnos perdidos de un jugador
  j.turnos_perdidos++;
  }

  
String leer_archivo(String ruta){ //elije una carta randomica del mazo indicado(ruta)
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
        if(dataLine!=""){
          linea++;}
      }
      dato = dataLine;
      dataFile.close();
    }else{
      Serial.println("Error al abrir el archivo");}//mostrar en lcd o led
  return dato;
}

String leer_archivo(String ruta, int num_linea){ //Lee la carta x del mazo indicado
  File dataFile;
  String dato;
  ruta += ".txt";
  dataFile = SD.open(ruta);
  if (dataFile){
      String dataLine = dataFile.readStringUntil('\n'); //Lee la primera línea del .txt
      //dataLine = dataFile.readStringUntil('\n'); //Lee la primera línea del .txt
      int linea = 1;
      while (dataFile.available() && linea <= num_linea+1){
        dataLine = dataFile.readStringUntil('\n');
        if(dataLine!=""){
          linea++;}
      }
      dato = dataLine;
      dataFile.close();
    }else{
      Serial.println("Error al abrir el archivo");}//mostrar en lcd o led
  return dato;
}

String sacar_carta(String ruta){//Ruta es el nombre del mazo
  String carta=leer_archivo(ruta);
  Serial.println(carta); 
  return carta; 
  //formatear carta
}

String elegir_pregunta(String ruta){// nombre del mazo
  String pregunta=leer_archivo(ruta);
  Serial.println(pregunta);
  return pregunta;

}
