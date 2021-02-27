#include "Arduino.h"
#include "Servo.h"
#include "Matriz.h"
#include "SoftwareSerial.h"
#ifndef Primitivas_h
#define Primitivas_h



//Tipos de Datos
struct Jugador{
   byte numero_jugador;//numero de jugador en el tablero
   byte pin_jugador;//conectado pin state
   byte pulsadores[3];
   int desafio;
   byte cant_desafio;
   byte turnos_perdidos;
};

struct Ficha{
    int pos_x;
    int pos_y;
    Jugador jugador;
    char pc_avance; //punto cardinal en que debe avanzar
    char pc_anterior; //punto cardinal en que avanzo antes
};



void guardar_pos_carro(byte x, byte y);
void leer_pos_carro(int &x, int &y);
void color_led(char color, int rled, int gled, int bled);
void agarrar_ficha(Servo &servo);
void soltar_ficha(Servo &servo);
boolean jugador_disponible(Jugador j);
void enviar_mensaje(Jugador j, String mensaje);
String recibir_mensaje(Jugador j);
String tirar_moneda(Matriz matriz, byte num_mat,int tiempo);
byte tirar_dado(Matriz matriz, byte lados, byte num_mat,int tiempo);
void perder_turno(Jugador &j, int cantidad);
String leer_archivo(String ruta);
String leer_archivo(String ruta, int num_linea);
String sacar_carta(String ruta);
String elegir_pregunta(String ruta);
String tirar_dado_arcade(Matriz matriz, byte num_mat, int tiempo); //Prueba dado arcade 
void flechas_matriz(Matriz matriz, byte num_mat, char pos);

#endif
