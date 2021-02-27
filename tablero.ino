#include <SPI.h>
#include <SD.h>
#include "Matriz.h"
#include "Primitivas.h"
#include "mensajes.h"


#define pin_servo 3
#define pin_sd 53
#define fin_yn 5 
#define fin_ys 6
#define fin_xo 44
#define fin_xe 42
#define data 7
#define cs 8
#define clk 9
#define led_red 11
#define led_blue 12
#define led_green 10

#define dir_m1 30
#define step_m1 32
#define ms2_1 34
#define dir_m2 36
#define step_m2 38
#define ms2_2 40

#define pin_j1 22
#define pin_j2 24
#define pin_j3 26
#define pin_j4 28

//bluetooth = 1 blue, 2 green, 3 yellow, 4 red




String ruta_msj="mensajes";


const byte cant_f = 2;//cant_fichas_x_jugador
Ficha fichas[4];
Jugador jugadores[4];
Jugador jugadores_inactivos[4];

//Variable
byte  tablero[10][8] = {{2,2,2,2,2,2,2,1},
                        {0,0,0,1,1,1,2,1},
                        {0,2,2,5,2,5,2,1},
                        {3,5,3,1,0,1,0,1},
                        {3,2,3,5,5,5,4,1},
                        {3,2,5,5,5,1,4,1},
                        {3,0,3,0,3,1,5,1},
                        {3,4,5,4,5,4,4,0},
                        {3,4,3,3,3,0,0,0},
                        {3,4,4,4,4,4,4,4}};

int pos_x_carro=0;
int pos_y_carro=0;
int tiempo = 150;
int cantidad_jugadores_activos = 4;
boolean tipo_tablero; //true= tablero libre   false= tablero delimitado

//Objetos
Servo carro;
Matriz matriz = Matriz(data,cs,clk,4);
//Serial4 = Serial4(47, 49); // RX, TX  define el puerto serial para el jugador 4
//SoftwareSerial Monitor(46, 48); // RX, TX  define el puerto serial para el monitor

//Registros
//Jugador j1 = {1,pin_j1,{23,25,27},0,0,0};  //prueba
//Jugador j2 = {2,pin_j2,{29,31,33}};  //prueba
//Jugador j3 = {3,pin_j3,{35,37,39}};  //prueba
//Jugador j4 = {4,pin_j4,{41,43,45}};  //prueba

//esto es en la nueva placa
Jugador j1 = {1,pin_j1,{41,43,45},0,0,0};  //prueba
Jugador j2 = {2,pin_j2,{23,25,27},0,0,0};  //prueba
Jugador j3 = {3,pin_j3,{29,31,33},0,0,0};  //prueba
Jugador j4 = {4,pin_j4,{35,37,39},0,0,0};  //prueba
char Color[] = {'W','B','G','R','Y','L','O','P'};

Ficha f1 = {0,7,j1,'N'};      //prueba//0,7
Ficha f2 = {0,0,j2,'O'};      //prueba//0,0
Ficha f3 = {9,0,j3,'N'};      //prueba
Ficha f4 = {9,7,j4,'O'};      //prueba


void setup() {
  fichas[0] = f1;
  fichas[1] = f2;
  fichas[2] = f3;
  fichas[3] = f4;
  //jugadores_inactivos[0] = j1;
  //jugadores_inactivos[1] = j2;
  //jugadores_inactivos[2] = j3;
  //jugadores_inactivos[3] = j4;
  jugadores[0]=j1;
  jugadores[1]=j2;
  jugadores[2]=j3;
  jugadores[3]=j4;
 

 

  
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600); 
  Serial2.begin(9600);
  Serial3.begin(9600);
 // Serial4.begin(9600);

  //inicializar pulsadores provisorio
  for(int i=23;i<=45;i++){
    pinMode(i,INPUT);
    i++;
    }

  carro.attach(pin_servo);
  //agarrar_ficha(carro);
//  carro.write(0);
//  delay(500);
  soltar_ficha(carro);
//  carro.write(100);
//  delay(500);
  

  pinMode(dir_m1, OUTPUT);
  pinMode(step_m1, OUTPUT);
  pinMode(ms2_1, OUTPUT);
  digitalWrite(ms2_1, HIGH);
  pinMode(dir_m2, OUTPUT);
  pinMode(step_m2, OUTPUT);
  pinMode(ms2_2, OUTPUT);
  digitalWrite(ms2_2, HIGH);
  pinMode (fin_yn,INPUT);
  pinMode (fin_ys,INPUT);
  pinMode (fin_xe,INPUT);
  pinMode (fin_xo,INPUT);
  pinMode(led_red,OUTPUT);
  pinMode(led_green,OUTPUT);
  pinMode(led_blue,OUTPUT);
  
  tipo_tablero= true;//leer cuando se carga el juego

  Serial.print("Initializing SD card...");
  if (!SD.begin(pin_sd)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("Ok!");
  leer_pos_carro(pos_x_carro,pos_y_carro);
  Serial.println(pos_x_carro);
  Serial.println(pos_y_carro);
}

void loop() {
//  Serial.println(obtener_pulsador(j1));
//  Serial.println(obtener_pulsador(j2));
//  Serial.println(obtener_pulsador(j3));
  //Serial.println(obtener_pulsador(j4));
   //Serial.println(sacar_dados(2,3,5,6,2));   
  //Serial.println(obtener_pulsador(j4));
  //Serial.println(casillero_ocupado(0,1));
  //Serial.println(casillero_ocupado(0,7));
  //calibrar();
      if(Serial.available()>0){
        String comando = Serial.readStringUntil('.');
        if(comando == "consola"){
            menu();
            //consola();
          }
      }

//Serial.println(ganador=es_ganador(fichas[1]));

//if(jugador_disponible(j2)){
//  Serial.println("OK");
//  enviar_mensaje(j2,"Hola");
//  }
  

    //sacar_moneda("cara",3,5,2);
    //tirar_moneda(matriz,2,250);    
//
//  if (Serial.available()){
//    /*String mov = recibir_mensaje(j1);
//        enviar_mensaje(j1,mov);
//    */String mov = Serial.readStringUntil('.');
//    if (mov=="N"){
//      mover('N');
//      delay(1000);  
//      }
//       if (mov=="E"){
//      mover('E');
//      delay(1000);  
//      }
//      if (mov=="J"){
//        mover_ficha(f, 1, 'E');
//      delay(1000);  
//      }
//  }

  //jugador_disponible(j1);
  //jugador_disponible(j2);
 delay(500);
 //mover_ficha(f2,4);
 //Serial.println(f2.pos_x);
 //Serial.println(f2.pos_y);
 //delay(10000);

}


////// menu ////

void menu(){
 menuprincipal();
 int  i = 0;
 while(i==0){
  i = Serial.parseInt();
  }
 while(i!=11){
     if(i==1){
      Serial.println("-------------");
      Serial.println("1- SUMAR DADOS");
      Serial.println("2- SACAR MONEDAS");
      Serial.println("3- SACAR DADOS");
      Serial.println("4- TIRAR DADO");
      Serial.println("5- HACER PREGUNTA");
      Serial.println("6- LED COLORES RANDOMICO");
      Serial.println("7- LED TODOS LOS COLORES");
      Serial.println("8- CALIBRAR");
      Serial.println("9- MOVER CARRO");
      Serial.println("10- VOLVER AL MENU PRINCIPAL");
      menuconfig();
      }  
     if(i==2){
      Serial.println("-------------");
      Serial.println("1- Jugar Jumanji");
      Serial.println("2- Jugar Otro Juego");
      Serial.println("3- volver menu principal");
      menujugar();   
      } 
     if (i==3){
      menu();
      }
     i = Serial.parseInt();
  }
}                                         
                                                                                                                                                                             
void menuconfig(){
 int  var = 0;
 while(var==0){
 var =Serial.parseInt();
 }
 bool resultado;
 while(var!=11){
   switch(var){
        case  1: {resultado = sumar_dados(10,4,6,4);break;}
        case  2: {resultado = sacar_moneda("cara",2,5,2);break;}
        case  3: {resultado = sacar_dados(5,2,5,6,2);break;}
        case  4: {tirar_dado(matriz,6,2,500);break;}
        case  5: {resultado = pregunta_respuesta(j1);break;}
        case  6: {Colorrandom();break;}
        case  7: {colores();break;}
        case  8: {calibrar();break;}
        case  9: {submenumover();break;}
        case  10:{menu();break;}
        
	      
        }
   if(var>=1 && var<=5){
     if (resultado){
        Serial.println("OK");}
     else{
        Serial.println("NO");}
   }
   var=Serial.parseInt();    
  }
  
}




 /// ACOMODAR ///
void submenumover(){
  Serial.println("1- MOVER AL OESTE");
  Serial.println("2- MOVER AL SUR");
  Serial.println("3- MOVER AL NORTE");
  Serial.println("4- MOVER AL ESTE"); 
  int c;
  while (c!=5){
    c=Serial.parseInt();
     //if (var!='0'){
      // var='0';}
    switch(c){    
      case 1: {mover('O');Serial.println("OESTE");break;}
      case 2: {mover('S');Serial.println("SUR");break;}
      case 3: {mover('N');Serial.println("NORTE");break;}
      case 4: {mover('E');Serial.println("ESTE");break;}
      case 5: {menu();}
    }
  }
  //Serial.println(c);
  

}


void colores(){
    for(int i=0;i<3;i++){
    color_led('R',led_red,led_green,led_blue);  
    delay(150);  
    color_led('G',led_red,led_green,led_blue);  
    delay(150);  
    color_led('B',led_red,led_green,led_blue);  
    delay(150);  
    color_led('Y',led_red,led_green,led_blue);  
    delay(150);  
    color_led('L',led_red,led_green,led_blue);  
    delay(150);  
    color_led('W',led_red,led_green,led_blue);  
    delay(150);  
    color_led('P',led_red,led_green,led_blue);  
    delay(150);
    color_led('O',led_red,led_green,led_blue);   
    }
}

void Colorrandom(){
 int i = random(0,8);
 char x = Color[i];
 color_led(x,led_red,led_green,led_blue);
  
  }


void menujugar(){  
  int  var = 0;
   while(var==0){
    var =Serial.parseInt();
   }
   while(var!=5){
     switch(var){
          case  1: {iniciar_juego();break;}
          case  2: {Serial.println("esta jugando otro juego");break;}
          case  3: {menu();break;}
          }
     var=Serial.parseInt(); 
   }   
}

void menuprincipal(){
  Serial.println("--------------------");
  Serial.println("MENU");
  Serial.println("1- MODO PRUEBA");
  Serial.println("2- JUGAR");
  
  }




/////

char opuesto(char pcardinal){
  char respuesta=' ';
  switch(pcardinal){
    case 'N':{
      respuesta='S';
      break;
      }
    case 'O':{
      respuesta='E';
      break;
      }
    case 'S':{
      respuesta='N';
      break;
      }
    case 'E':{
      respuesta='O';
      break;
      }
    }
  return respuesta;
  }
void preprocesar(Jugador &j, Ficha &f, int cant, String cadena){
  if(cadena.indexOf("volver_anterior")>=0){
    j.desafio=0;
    Serial.println("Debes volver a la posicion anterior"); //ejecutar mover a posicion anterior
    agarrar_ficha(carro);
    f.pc_avance=opuesto(f.pc_avance);
    f.pc_anterior=f.pc_avance;
    mover_ficha(f,cant+1);
    f.pc_avance=opuesto(f.pc_avance);
    f.pc_anterior=f.pc_avance;
    soltar_ficha(carro);
    }
  if (cadena.indexOf("volver_inicio")>=0){
    j.desafio=0;
    Serial.println("Debes volver al inicio"); //ejecutar mover a posicion de inicio
    switch(j.numero_jugador){
      case 1:{
        f.pos_x=0;
        f.pos_y=7;
        f.pc_avance='S';
        agarrar_ficha(carro);
        mover_carro_a(0,7);//falta llamar a mover a()
        soltar_ficha(carro);
        break;
        }
      case 2:{
        f.pos_x=0;
        f.pos_y=0;
        f.pc_avance='E';
        agarrar_ficha(carro);
        mover_carro_a(0,0);//falta llamar a mover a()
        soltar_ficha(carro);
        break;
        }
      case 3:{
        f.pos_x=9;
        f.pos_y=0;
        f.pc_avance='N';
        agarrar_ficha(carro);
        mover_carro_a(9,0);//falta llamar a mover a()
        soltar_ficha(carro);
        break;
        }
      case 4:{
        f.pos_x=9;
        f.pos_y=7;
        f.pc_avance='O';
        agarrar_ficha(carro);
        mover_carro_a(9,7);//falta llamar a mover a()
        soltar_ficha(carro);
        break;
        }
      }
  }
  
  if (cadena.indexOf("perder_turno")>=0){
    j.desafio=0;
    perder_turno(j,1);
    Serial.println("Perderas el siguiente turno");
  }
  }

boolean comando(Jugador j, String cadena){
  boolean resultado=false;
  cadena=cadena.substring(cadena.indexOf(';')+1);
  cadena=cadena.substring(cadena.indexOf(';')+1);
  String desafio=cadena.substring(0,cadena.indexOf(';'));
  cadena=cadena.substring(cadena.indexOf(';')+1);
  if (desafio=="sacar_dado"){
    byte valor=cadena.substring(0,cadena.indexOf(';')).toInt();
    cadena=cadena.substring(cadena.indexOf(';')+1);
    byte cantidad=cadena.substring(0,cadena.indexOf(';')).toInt();
    cadena=cadena.substring(cadena.indexOf(';')+1);
    byte intentos=cadena.toInt();
    resultado = sacar_dados(valor,cantidad,intentos,6,j.numero_jugador);
    }
  if (desafio=="sumar_dado"){
    byte valor=cadena.substring(0,cadena.indexOf(';')).toInt();
    cadena=cadena.substring(cadena.indexOf(';')+1);
    byte cantidad=cadena.substring(0,cadena.indexOf(';')).toInt();
    cadena=cadena.substring(cadena.indexOf(';')+1);
    byte intentos=cadena.toInt();
    resultado= sumar_dados(cantidad,intentos,6,j.numero_jugador);
  }
  if (desafio=="sacar_moneda"){
    String valor=cadena.substring(0,cadena.indexOf(';'));
    cadena=cadena.substring(cadena.indexOf(';')+1);
    byte cantidad=cadena.substring(0,cadena.indexOf(';')).toInt();
    cadena=cadena.substring(cadena.indexOf(';')+1);
    byte intentos=cadena.toInt();
    resultado=sacar_moneda(valor,cantidad,intentos,j.numero_jugador);
  }
  if (desafio.indexOf("pregunta")>=0){
    resultado= pregunta_respuesta(j);
  }
  
    
  return resultado;
  }

boolean buscar_carta(String mazo, String buscado){
  Serial.println("buscar en mazo");
  buscado = ";"+buscado+";";
  return mazo.indexOf(buscado)>=0;
}

void jugar(){
  int anterior_x;
  int anterior_y;
  int cantidad; //valor obtenido en el dado que se mueve la ficha
  boolean ganador=false;
  byte i = 0;
  String usadas;
  int cant_usadas=0;
  Serial.println("Inicio de Juego");
  while (not ganador) {
    Jugador j;
    Ficha f;
    if (jugadores[i].numero_jugador != 0){
      j = jugadores[i];
      f = fichas[i];
    }
    else{
      for (int k = i+1; k <=3; k++){
        if (jugadores[k].numero_jugador != 0){
          j = jugadores[k];
          f = fichas[k];
          break;
        }
      }
    }
    led_jugador(j);
    Serial.println("carro");
    Serial.println(pos_x_carro);
    Serial.println(pos_y_carro);
    Serial.println("ficha");
    Serial.println(f.pos_x);
    Serial.println(f.pos_y);
    Serial.println("Turno del jugador");
    Serial.println(j.numero_jugador);
    if (j.desafio<=0){
      mover_carro_a(f.pos_x,f.pos_y);
      pos_x_carro=f.pos_x;
      pos_y_carro=f.pos_y;  
    }
    Serial.println(pos_x_carro);
    Serial.println(pos_y_carro);
    if (j.turnos_perdidos==0){
     
      //if (j.desafio == 0) || j.desafio==-1){
         if (j.desafio==0){
             Serial.println("Tire un dado para mover");
             char pulsa='$';
             while (pulsa=='$'){
               delay(10);
               pulsa=obtener_pulsador(j);}
               int tiempo;
              if(pulsa=='X'){tiempo=500;}
              if(pulsa=='Y'){tiempo=250;}
              if(pulsa=='Z'){tiempo=100;}
               Serial.print("Obtuviste un ");
               cantidad=tirar_dado(matriz,6,j.numero_jugador,tiempo);
               Serial.println(cantidad);
               anterior_x=f.pos_x;
               anterior_y=f.pos_y;
               int movimientos = 0;
               agarrar_ficha(carro);
               while(movimientos<cantidad && !es_ganador(f)){
                  mover_ficha(f,1);
                  movimientos++;
                  }
               soltar_ficha(carro);
         //}
           ganador=es_ganador(f);
           if(!ganador){
             String desafio=sacar_carta("jumanji");
             j.desafio=desafio.substring(0,desafio.indexOf(';')).toInt();
             Serial.println(desafio.substring(0,desafio.indexOf(';')).toInt());
             String num_desafio = desafio.substring(0,desafio.indexOf(';'));
             while(buscar_carta(usadas, num_desafio)){
                 desafio = leer_archivo("jumanji");
                 num_desafio = desafio.substring(0,desafio.indexOf(';'));
              }
             j.desafio=num_desafio.toInt();
             cant_usadas++;
             Serial.println(j.desafio);
             usadas += ";"+num_desafio+";";
             preprocesar(j,f,cantidad,desafio); 
           }
           //delay(2000);
        }  
        else{
           String desafio=leer_archivo("jumanji",j.desafio);   
           Serial.println(desafio);
           boolean resultado = comando(j,desafio);
           Serial.println("Resultado obtenido");
           Serial.println(resultado);
           if(resultado){
            j.cant_desafio++;
            j.desafio=0;
            }
        }
        if (cant_usadas==28){
          usadas="";
          for(byte j=0;j<4;j++){
            if (jugadores[j].desafio>0){
              cant_usadas++;
              usadas+= ";"+String(jugadores[j].desafio)+";";
              }
            }
          }

        if (ganador){
          Serial.println("El ganador fue");
          Serial.println(j.numero_jugador);
         }
    }
    else{
      j.turnos_perdidos--;
      Serial.println("Se te desconto un turno perdido");
      if (j.turnos_perdidos == 0){
        j.desafio=0;//-1;
        }
      }
    jugadores[i]=j;
    fichas[i]=f;
    matriz.dibujar_simbolo(j.numero_jugador, '0');
    i++;  
    if (i==cantidad_jugadores_activos){
      i=0;
      if(cantidad_jugadores_activos<4){
        incorporar_jugador(5000);}
      }
      else{
        Serial.println("Maximo de jugadores, no incorpora");
      }
    }
  }

///////////Led jugador
void led_jugador(Jugador j){
    switch(j.numero_jugador){
    case 1:
      color_led('B',led_red,led_green,led_blue);
      break;
    case 2:
      color_led('G',led_red,led_green,led_blue);
      break;
    case 3:
      color_led('Y',led_red,led_green,led_blue);
      break;
    case 4:
      color_led('R',led_red,led_green,led_blue);
      break;
  }
}
//////////////////////////////

///////////Incorporar jugador 
void incorporar_jugador(int tiempo){
  char respuesta = '$';
  int inicio = millis();
  int actual = millis(); 
  Serial.println(" Desea incorporarse a la partida? - Presione un boton");    
  while(actual - inicio < tiempo){
    for (int i = 0; i<=3; i++){
      if(jugadores_inactivos[i].numero_jugador!=0){      
        respuesta = obtener_pulsador(jugadores_inactivos[i]);  
        if(respuesta!='$' ){
          jugadores[i] = jugadores_inactivos[i];
          cantidad_jugadores_activos++;
          Jugador jnull;
          jugadores_inactivos[i] = jnull;
          led_jugador(jugadores[i]);
          delay(150);
          color_led('O',led_red,led_green,led_blue);
          Serial.print(" Jugador agregado: ");
          Serial.println(i+1);
        }
      }
    }
    actual = millis(); 
  }
}
////////////////////////////

///////////////Iniciar juego
void iniciar_juego(){
  while(cantidad_jugadores_activos<2){
    incorporar_jugador(3000);
  }
  Serial.println("30 segundos para incorporar otros jugadores");
  incorporar_jugador(10000);
  jugar();
}
//////////////////////////////

boolean es_ganador(Ficha f){
  boolean resultado=false;

  if((f.pos_x>=4)&&(f.pos_x<=5)&&(f.pos_y>=3)&&(f.pos_y<=4)){
    resultado=true;
    }
  return resultado;
  }


void consola(){
  Serial.println("Modo Consola");
   String comando = Serial.readStringUntil('.');
   while(comando != "salir"){
      if (comando == "sumar dado"){
          if (sumar_dados(10,4,6,1)){
            Serial.println("OK");}
          else{
            Serial.println("NO");}
        }

      if (comando== "sacar dados"){
        if (sacar_dados(5,2,5,6,2)){
          Serial.println("OK");}
         else{
          Serial.println("NO");}
        }

      if (comando == "sacar moneda"){
        if (sacar_moneda("cara",2,5,2)){
          Serial.println("OK");}
        else{
          Serial.println("NO");}
        } 

       if (comando == "tirar dado"){
        if (tirar_dado(matriz,6,2,500)){
          Serial.println("OK");}
        else{
          Serial.println("NO");}
        }  

        if (comando == "tirar moneda"){
          if (tirar_moneda(matriz,2,500)){
            Serial.println("OK");}
          else{
            Serial.println("NO");}
        }


        if (comando=="hacer pregunta"){
          if(pregunta_respuesta(j1)){
            Serial.println("OK");
            }
           else{Serial.println("NO");} 
          }


        if (comando=="agarrar"){
            agarrar_ficha(carro);
        } 
          

         if (comando=="soltar"){
            soltar_ficha(carro);
            
         }
          if (comando=="jugar"){
           iniciar_juego();
            
         }
          if (comando=="N"){
           mover('N');
            
         }
          if (comando=="S"){
           mover('S');
            
         }
          if (comando=="E"){
           mover('E');
            
         }
          if (comando=="O"){
           mover('O');
            
         }
          if (comando=="calibrar"){
           calibrar();
            
         }
         if (comando == "tirar dado arcade"){ //Comando en consola para probar el dado arcade
          if (tirar_dado_arcade(matriz,4,500)){
            Serial.println("OK");}
          else{
            Serial.println("NO");}
        }
        if (comando=="matriz flechas"){
           flechas_matriz(matriz,4,'R');
            
         }
     comando = Serial.readStringUntil('.');
    }  
  }

//PRIMITIVAS TABLERO







void calibrar(){
    digitalWrite(dir_m2, true);
     while (digitalRead(fin_xo)!=HIGH){
        digitalWrite(step_m2, HIGH);         // Aqui generamos un flanco de bajada HIGH - LOW
        delayMicroseconds(tiempo);         // Pequeño retardo para formar el pulso en STEP
        digitalWrite(step_m2, LOW); 
        delayMicroseconds(tiempo);        
     }
    digitalWrite(dir_m1, true);
    while (digitalRead(fin_yn)!=HIGH){    
        digitalWrite(step_m1, HIGH);         // Aqui generamos un flanco de bajada HIGH - LOW
        delayMicroseconds(tiempo);         // Pequeño retardo para formar el pulso en STEP
        digitalWrite(step_m1, LOW); 
        delayMicroseconds(tiempo);  
  }
    digitalWrite(dir_m2, false);
    for (int i = 1; i <= 2500; i++){  //Despues revisar cuando se acomoden los fines de carrera (4500)
    if (digitalRead(fin_xe)!=HIGH){           
        digitalWrite(step_m2, HIGH);         // Aqui generamos un flanco de bajada HIGH - LOW
        delayMicroseconds(tiempo);         // Pequeño retardo para formar el pulso en STEP
        digitalWrite(step_m2, LOW); 
        delayMicroseconds(tiempo); 
      }
    }
  digitalWrite(dir_m1, false);
  for (int i = 1; i <= 1; i++){  //Despues revisar cuando se acomoden los fines de carrera (5000);
    if (digitalRead(fin_ys)!=HIGH){
      digitalWrite(step_m1, HIGH);         // Aqui generamos un flanco de bajada HIGH - LOW
      delayMicroseconds(tiempo);         // Pequeño retardo para formar el pulso en STEP
      digitalWrite(step_m1, LOW); 
      delayMicroseconds(tiempo);
    }
  }
   pos_x_carro=0;
   pos_y_carro=0;
   guardar_pos_carro(pos_x_carro, pos_y_carro);
   //pestaneo led pendiente rgb
  colores();
   
}







void esquivar_casillero(Ficha &f, byte x, byte y, char direccion){
  char dir1=' ';
  char dir2 =' '; 
   if(direccion=='O'){
    if((f.pos_x!=x)&&(f.pos_y!=y)){
      if(x>f.pos_x){mover('S');mover('O');f.pc_avance='S';}
      else{mover('N');mover('O');f.pc_avance='N';}
      
     }
     else{
      if(f.pos_x<=5){dir1='N';dir2='S';}
      else{dir1='S';dir2='N';}
      mover(dir1);
      for(int i=f.pos_y ; i>y ;i--){
        mover(direccion);
        }
      mover(dir2);
      }
   }
    if(direccion=='E'){
      if((f.pos_x!=x)&&(f.pos_y!=y)){
        if(x>f.pos_x){mover('S');mover('E');f.pc_avance='S';}
        else{mover('N');mover('E');f.pc_avance='N';}
      }
      else{
        if(f.pos_x<=5){dir1='N';dir2='S';}
        else{dir1='S';dir2='N';}
        mover(dir1);
        for(int i=f.pos_y ;i<y;i++){
          mover(direccion);
          }
        mover(dir2);
        }
      }
      if(direccion=='N'){
        if((f.pos_x!=x)&&(f.pos_y!=y)){
          if(y>f.pos_y){mover('E');mover('N');mover('N');f.pc_avance='E';}
          else{mover('O');mover('N');mover('N');f.pc_avance='O';}
         }
       else{
        if(f.pos_x<=3){dir1='O';dir2='E';}
        else{dir1='E';dir2='O';}
        mover(dir1);
        for(int i=f.pos_y ;i<y;i++){
          mover(direccion);
          }
          mover(dir2);
      }
      }
     if(direccion=='S'){
      if((f.pos_x!=x)&&(f.pos_y!=y)){
        if(y>f.pos_y){mover('E');mover('S');f.pc_avance='E';}
        else{mover('O');mover('S');f.pc_avance='O';}
       }
     else{
      if(f.pos_x<=3){dir1='O';dir2='E';}
      else{dir1='E';dir2='O';}
      mover(dir1);
      for(int i=f.pos_y ;i<y;i++){
        mover(direccion);
        }
        mover(dir2);
      }      
    }
   }


void mover (char direccion){//mueve el carro en la direccion indicada
    switch (direccion){
      case 'O':{
               digitalWrite(dir_m2, true);
               //Serial.println("O");
               for (int i = 1; i <= 6950; i++){
                  if (digitalRead(fin_xo)!=HIGH){
                    digitalWrite(step_m2, HIGH);         // Aqui generamos un flanco de bajada HIGH - LOW
                    delayMicroseconds(tiempo);         // Pequeño retardo para formar el pulso en STEP
                    digitalWrite(step_m2, LOW); 
                    delayMicroseconds(tiempo);
                  }  
                }
               pos_y_carro--;
               guardar_pos_carro(pos_x_carro, pos_y_carro);
      break;}
      case 'E': {
                digitalWrite(dir_m2, false);
                //Serial.println("E");
                for (int i = 1; i <= 6950; i++){
                  if (digitalRead(fin_xe)!=HIGH){
                    digitalWrite(step_m2, HIGH);         // Aqui generamos un flanco de bajada HIGH - LOW
                    delayMicroseconds(tiempo);         // Pequeño retardo para formar el pulso en STEP
                    digitalWrite(step_m2, LOW); 
                    delayMicroseconds(tiempo);
                    }  
                }
                pos_y_carro++;
                guardar_pos_carro(pos_x_carro, pos_y_carro);
      break;}
      case 'N':{
                digitalWrite(dir_m1, true);
                //Serial.println("N");
                for (int i = 1; i <= 7000; i++){
                  if (digitalRead(fin_yn)!=HIGH){
                    digitalWrite(step_m1, HIGH);         // Aqui generamos un flanco de bajada HIGH - LOW
                    delayMicroseconds(tiempo);         // Pequeño retardo para formar el pulso en STEP
                    digitalWrite(step_m1, LOW); 
                    delayMicroseconds(tiempo);  
                  }
                }
                pos_x_carro--;
                guardar_pos_carro(pos_x_carro, pos_y_carro);
      break;} 
      case 'S': {
                digitalWrite(dir_m1, false);
                //Serial.println("S");
                for (int i = 1; i <= 7000; i++){
                  if (digitalRead(fin_ys)!=HIGH){
                    digitalWrite(step_m1, HIGH);         // Aqui generamos un flanco de bajada HIGH - LOW
                    delayMicroseconds(tiempo);         // Pequeño retardo para formar el pulso en STEP
                    digitalWrite(step_m1, LOW); 
                    delayMicroseconds(tiempo);
                  }  
                }
                pos_x_carro++;
                guardar_pos_carro(pos_x_carro, pos_y_carro);
      break;}
      }         
 }

int determinar_posicion(Ficha &ficha, byte cant){
    if(cant>0){
        byte num_jug = ficha.jugador.numero_jugador;
        switch (ficha.pc_avance){
            case 'N':{char opuesto = 'S';
                      if((ficha.pos_x>0) && (tablero[ficha.pos_x-1][ficha.pos_y]== num_jug || tablero[ficha.pos_x-1][ficha.pos_y]== 5) &&
                         (ficha.pc_anterior!=opuesto)){
                            cant--;
                            ficha.pc_anterior=ficha.pc_avance;
                            ficha.pos_x--;
                            if(casillero_ocupado(ficha.pos_x,ficha.pos_y)){
                              Serial.println("casillero ocupado");
                              determinar_posicion(ficha,cant);}
                            else{
                              return cant;
                            }
                      }
                      else{ 
                            ficha.pc_avance='E';
                            determinar_posicion(ficha,cant);}
              break;}
              case 'E':{char opuesto = 'O';
                        if((ficha.pos_y<7) && (tablero[ficha.pos_x][ficha.pos_y+1]== num_jug || tablero[ficha.pos_x][ficha.pos_y+1]== 5) &&
                           (ficha.pc_anterior!=opuesto)){
                              cant--;
                              ficha.pc_anterior=ficha.pc_avance;
                              ficha.pos_y++;
                              if(casillero_ocupado(ficha.pos_x,ficha.pos_y)){
                                determinar_posicion(ficha,cant);}
                              else{
                                return cant;
                              }
                        }
                        else{ 
                              ficha.pc_avance='S';
                              determinar_posicion(ficha,cant);}
              break;}
              case 'S':{char opuesto = 'N';
                        if((ficha.pos_x<9) && (tablero[ficha.pos_x+1][ficha.pos_y]== num_jug || tablero[ficha.pos_x+1][ficha.pos_y]== 5) &&
                           (ficha.pc_anterior!=opuesto)){
                              cant--;
                              ficha.pc_anterior=ficha.pc_avance;
                              ficha.pos_x++;
                              if(casillero_ocupado(ficha.pos_x,ficha.pos_y)){
                                determinar_posicion(ficha,cant);}
                              else{
                                return cant;
                              }
                        }
                        else{ 
                              ficha.pc_avance='O';
                              determinar_posicion(ficha,cant);}
              break;}
              case 'O':{char opuesto = 'E';
                        if((ficha.pos_y>0) && (tablero[ficha.pos_x][ficha.pos_y-1]== num_jug || tablero[ficha.pos_x][ficha.pos_y-1]== 5) &&
                           (ficha.pc_anterior!=opuesto)){
                              cant--;
                              ficha.pc_anterior=ficha.pc_avance;
                              ficha.pos_y--;
                              if(casillero_ocupado(ficha.pos_x,ficha.pos_y)){
                                determinar_posicion(ficha,cant);}
                              else{
                                return cant;
                              }
                        }
                        else{ 
                              ficha.pc_avance='N';
                              determinar_posicion(ficha,cant);}
              break;}
        }
    }
    else{
      //no se encontro lugar disponible donde ir devuelve 0
      return cant;
      }

}


void mover_carro_a(int x, int y){
  Serial.println("MOVER CARRO");
  Serial.println(x);
  Serial.println(y);
  Serial.println(pos_x_carro);
  Serial.println(pos_y_carro);
  Serial.println("MOVER CARRO");
    if(pos_x_carro>x){
      for(int i=pos_x_carro;i>x;i--){
        mover('N');}
      }
      else{
     if(pos_x_carro<x){
      for(int i=pos_x_carro;i<x;i++){
        mover('S');}
     }}
     
     if(pos_y_carro>y){
      for(int i=pos_y_carro;i>y;i--){
        mover('O');}
     }
     else{
     if(pos_y_carro<y){
      for(int i=pos_y_carro;i<y;i++){
        mover('E');}
        }   
      }
      
}



void mover_ficha (Ficha &ficha, byte cant){//Tablero delimitado
    if(cant>0){
        byte num_jug = ficha.jugador.numero_jugador;
        switch (ficha.pc_avance){
            case 'N':{char opuesto = 'S';
                      if((ficha.pos_x>0) && (tablero[ficha.pos_x-1][ficha.pos_y]== num_jug || tablero[ficha.pos_x-1][ficha.pos_y]== 5) &&
                         (ficha.pc_anterior!=opuesto)){
                          if(casillero_ocupado(ficha.pos_x-1,ficha.pos_y)){
                                if(cant>1){
                                  Ficha aux = ficha;
                                  int caux = determinar_posicion(aux, cant);
                                  Serial.println("cantidad");
                                  Serial.println(caux);
                                  if (caux>0){
                                    esquivar_casillero(ficha,aux.pos_x,aux.pos_y,'N');
                                    cant = cant-(cant-caux);}
                                  else{cant=0;}
                                } 
                                else{cant--;}
                                }
                              else{
                                mover(ficha.pc_avance);
                                cant--;
                              }
                              ficha.pc_anterior=ficha.pc_avance;
                              ficha.pos_x = pos_x_carro;
                              ficha.pos_y = pos_y_carro;
                              Serial.println("mover norte");
                              mover_ficha(ficha,cant);}
                        else{ 
                              ficha.pc_avance='E';
                              Serial.println("cambiar al este");
                              mover_ficha(ficha,cant);}

              break;}
              case 'E':{char opuesto = 'O';
                        if((ficha.pos_y<7) && (tablero[ficha.pos_x][ficha.pos_y+1]== num_jug || tablero[ficha.pos_x][ficha.pos_y+1]== 5) &&
                           (ficha.pc_anterior!=opuesto)){
                              if(casillero_ocupado(ficha.pos_x,ficha.pos_y+1)){
                                if(cant>1){
                                  Ficha aux = ficha;
                                  int caux = determinar_posicion(aux, cant);
                                  if (caux>0){
                                    esquivar_casillero(ficha,aux.pos_x,aux.pos_y,'E');
                                    cant = cant-(cant-caux);}
                                  else{cant=0;}
                                } 
                                else{cant--;}
                                }
                              else{
                                mover(ficha.pc_avance);
                                cant--;
                              }
                              ficha.pc_anterior=ficha.pc_avance;
                              ficha.pos_x = pos_x_carro;
                              ficha.pos_y = pos_y_carro;
                              Serial.println("mover este");
                              mover_ficha(ficha,cant);}
                        else{ 
                              ficha.pc_avance='S';
                              Serial.println("cambiar al sur");
                              mover_ficha(ficha,cant);}
              break;}
              case 'S':{char opuesto = 'N';
                        if((ficha.pos_x<9) && (tablero[ficha.pos_x+1][ficha.pos_y]== num_jug || tablero[ficha.pos_x+1][ficha.pos_y]== 5) &&
                           (ficha.pc_anterior!=opuesto)){
                            if(casillero_ocupado(ficha.pos_x+1,ficha.pos_y)){
                                if(cant>1){
                                  Ficha aux = ficha;
                                  int caux = determinar_posicion(aux, cant);
                                  if (caux>0){
                                    esquivar_casillero(ficha,aux.pos_x,aux.pos_y,'S');
                                    cant = cant-(cant-caux);}
                                  else{cant=0;}
                                } 
                                else{cant--;}
                                }
                              else{
                                mover(ficha.pc_avance);
                                cant--;
                              }
                              ficha.pc_anterior=ficha.pc_avance;
                              ficha.pos_x = pos_x_carro;
                              ficha.pos_y = pos_y_carro;
                              Serial.println("mover sur");
                              mover_ficha(ficha,cant);}
                        else{ 
                              ficha.pc_avance='O';
                              Serial.println("cambiar al oeste");
                              mover_ficha(ficha,cant);}
              break;}
              case 'O':{char opuesto = 'E';
                        if((ficha.pos_y>0) && (tablero[ficha.pos_x][ficha.pos_y-1]== num_jug || tablero[ficha.pos_x][ficha.pos_y-1]== 5) &&
                           (ficha.pc_anterior!=opuesto)){
                            if(casillero_ocupado(ficha.pos_x,ficha.pos_y-1)){
                                if(cant>1){
                                  Ficha aux = ficha;
                                  int caux = determinar_posicion(aux, cant);
                                  if (caux>0){
                                    esquivar_casillero(ficha,aux.pos_x,aux.pos_y,'O');
                                    cant = cant-(cant-caux);}
                                  else{cant=0;}
                                } 
                                else{cant--;}
                                }
                              else{
                                mover(ficha.pc_avance);
                                cant--;
                              }
                              ficha.pc_anterior=ficha.pc_avance;
                              ficha.pos_x = pos_x_carro;
                              ficha.pos_y = pos_y_carro;
                              Serial.println("mover oeste");
                              mover_ficha(ficha,cant);}
                        else{ 
                              ficha.pc_avance='N';
                              Serial.println("cambiar al norte");
                              mover_ficha(ficha,cant);}
              break;}
        }
    }
    else{
      //preguntar si hay que reubicar fichas
      Serial.println();
      }

}

void mover_ficha (Ficha &ficha, int cant, char direccion){//Tablero libre
    int pos, px, py;
    char opuesto;
    int limite, lim_mov;
    px = ficha.pos_x;
    py = ficha.pos_y;
    switch (direccion){
      case 'N':{
            pos = ficha.pos_x;
            opuesto = 'S';
            limite = 0;
            lim_mov = 10;
      break;}
      case 'S':{
            pos = ficha.pos_x;
            opuesto = 'N';
            limite = 9;
            lim_mov = 10;
      break;}
      case 'E':{
            pos = ficha.pos_y;
            opuesto = 'O';
            limite = 7;
            lim_mov = 8;
      break;}
      case 'O':{
            pos = ficha.pos_y;
            opuesto = 'E';
            limite = 0;
            lim_mov = 8;
      break;}
    }

    if(abs(limite-pos)-cant>=0){
      for(int c= 1; c<= cant; c++){
        switch(direccion){
          case 'N':{px--;break;}
          case 'S':{px++;break;}
          case 'E':{py++;break;}
          case 'O':{py--;break;}
          }
          //if(!casillero_ocupado(px,py)){
              mover(direccion);
          /*}
          else{
            if(c-cant>1){
              //esquivar
              }
            }*/   
       }
    }else{
      for(int c= 1; c<= abs(limite-pos); c++){
                mover(direccion);}
      if(cant < lim_mov){
        for(int c= 1; c<= (cant - abs(limite-pos)); c++){
                  mover(opuesto);}
      }else{
        for(int c= 1; c< lim_mov; c++){
                  mover(opuesto);}
        ficha.pos_x = pos_x_carro;
        ficha.pos_y = pos_y_carro;
        mover_ficha(ficha, (cant-lim_mov)+1, opuesto);
        }
    }
    ficha.pos_x = pos_x_carro;
    ficha.pos_y = pos_y_carro;
}

boolean casillero_ocupado(int x, int y){//provisoria
    boolean control = false;
    for(Ficha ficha : fichas){
        if (ficha.pos_x==x && ficha.pos_y==y){
            control = true;
            break;
          }
      }
      return control;
  }  





char obtener_pulsador(Jugador jug){
  char respuesta='$';
  if (jugador_disponible(jug)){
    char aux=recibir_mensaje(jug).charAt(0);
    if (aux=='X' || aux=='Y' || aux=='Z'){
      respuesta=aux;}
    }
  else{  
    if (digitalRead(jug.pulsadores[0])==HIGH){respuesta= 'X';}
    else{
        if(digitalRead(jug.pulsadores[1])==HIGH){respuesta='Y';}
        else{
          if (digitalRead(jug.pulsadores[2])==HIGH){respuesta='Z';}
        }
    }
  }
  return respuesta;
  }

 char hacer_pregunta(int tiempo, Jugador jug){//milisegundos
  char respuesta='$';
  boolean control = true;
  int inicio = millis();
  int actual = millis();
  while(control && actual - inicio < tiempo){
    respuesta = obtener_pulsador(jug);
    actual=millis();
    if(respuesta!='$' ){
      control = false;}
    }
  return respuesta;
  }


  
//PRIMITIVAS DESAFIO

boolean sacar_moneda(String valor, byte cantidad, byte intentos, byte num_jug){
  boolean resultado = false;
  byte cantidad_obtenida = 0;
  int inten=0;
  for(byte i = 0; i<intentos; i++){
    Serial.println("Tire una moneda para el desafio");
    char pulsa='$';
     while (pulsa=='$'){
       delay(10);
       pulsa=obtener_pulsador(jugadores[num_jug-1]);}
     int tiempo;
     if(pulsa=='X'){tiempo=500;}
     if(pulsa=='Y'){tiempo=250;}
     if(pulsa=='Z'){tiempo=100;}
    inten=intentos-(i+1);
    String res = tirar_moneda(matriz,num_jug,tiempo);
    delay(1500);
    if(res==valor){cantidad_obtenida++;}
    if(cantidad_obtenida+inten < cantidad){Serial.println("corte desafio");break;}
    if(cantidad_obtenida>=cantidad){
      resultado = true;
      break;}
  } 
  return resultado;
  }



boolean sumar_dados(byte cant_sumar, byte intentos, byte num_lados, byte num_jug){
  boolean resultado = false;
  byte cant_obt=0;
  int in;
  for(byte i=0; i<intentos; i++){
    Serial.println("Tire un dado para el desafio");
     char pulsa='$';
     while (pulsa=='$'){
       delay(10);
       pulsa=obtener_pulsador(jugadores[num_jug-1]);}
      int tiempo;
      if(pulsa=='X'){tiempo=500;}
      if(pulsa=='Y'){tiempo=250;}
      if(pulsa=='Z'){tiempo=100;}
    in=intentos-(i+1);
    cant_obt = cant_obt + tirar_dado(matriz,num_lados,num_jug,tiempo); 
    delay(1500);
    if(cant_obt+(in*num_lados)< cant_sumar){Serial.println("corte del desafio");break;}    
    if (cant_obt >=cant_sumar){
      resultado=true;
      break;}
  }
  return resultado;
  }
  


 boolean sacar_dados(byte valor,byte cantidad, byte intentos, byte num_lados, byte num_jug){
  boolean resultado = false;
  byte cantidad_obtenida=0;
  int in=0;
  for(byte i=0; i<intentos; i++){
    Serial.println("Tire un dado para el desafio");
    char pulsa='$';
    while (pulsa=='$'){
      delay(10);
      pulsa=obtener_pulsador(jugadores[num_jug-1]);}
    int tiempo;
    if(pulsa=='X'){tiempo=500;}
    if(pulsa=='Y'){tiempo=250;}
    if(pulsa=='Z'){tiempo=100;}
    in=intentos-(i+1);
    byte res = tirar_dado(matriz,num_lados,num_jug,tiempo);
    delay(1500); 
    if(res==valor){cantidad_obtenida++;}
    if(cantidad_obtenida+in < cantidad){Serial.println("corte del desafio");break;}
    if (cantidad_obtenida >=cantidad){
      resultado=true;
      break;}
  }
  return resultado;
  }



String ruta_respuesta="res";
String ruta_pregunta="pre";

boolean pregunta_respuesta(Jugador jug){
  boolean resultado=false;
  String pregunta=elegir_pregunta(ruta_pregunta);
  int num_pregunta = pregunta.substring(0,pregunta.indexOf(';')).toInt();
  String cr = leer_archivo(ruta_respuesta,0);
  int cant_resp = cr.substring(0, cr.indexOf('\n')).toInt();//borrar
  String respuestas [cant_resp];
  int indice=cant_resp*(num_pregunta-1)+1;
  for(int i=0; i<cant_resp; i++){
    respuestas[i] = leer_archivo(ruta_respuesta,indice+i);
    //Serial.println(respuestas[i]);
   }
  
  String res_random [3];

  res_random[random(0,3)] = respuestas[0];
  byte i = 0;
  while(i<2){
    byte indice = random(0,3);
    if(res_random[indice] == ""){
      i++;
      res_random[indice] = respuestas[i];  
    }
  }

  for(int i=0; i<3; i++){
    Serial.println(res_random[i]);
   }

  char res_jug = hacer_pregunta(7000, jug);
  byte opc_elegida;

  if(res_jug=='X'){opc_elegida=0;}
  else{
    if(res_jug=='Y'){opc_elegida=1;}
    else{
      if(res_jug=='Z'){opc_elegida=2;}
    }
  }
  
  Serial.println(res_jug);
  if(res_jug != '$' and respuestas[0] == res_random[opc_elegida]){
    resultado = true;}
   else{
    resultado = false;
   }

  return resultado;
  }


  
