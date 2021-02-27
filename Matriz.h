#include "Arduino.h"
#ifndef Matriz_h
#define Matriz_h

class Matriz
{
  public:
    Matriz(int dataIn, int load, int clock, byte nmax);
    void dibujar_simbolo(byte matriz, byte numero);
    void dibujar_simbolo(byte matriz, char simbolo);
    void num_matriz(byte cant);
    void maxSingle( byte reg, byte col);
    void maxAll (byte reg, byte col);
    void maxOne(byte maxNr, byte reg, byte col);
  
  private:
    int _dataIn, _load, _clock, _nmax;
    void putByte(byte data);
};


#endif
