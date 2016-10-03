#ifndef MATRIZ_H
#define MATRIZ_H
#include <vector>
#include "typedefs.h"
#include "metodoPotencia.h"
#include <iostream>

using namespace std;

class Matriz {

private:
    vector< vectorReal > data;
    int cantFilas;
    int cantColumnas;
    vector< vectorReal > autovectoress;
    vectorReal autovaloress;
    void calcularAtuvolaresYAutovectores();
    Matriz deflacion(double lambda, vectorReal& autovector, Matriz& A);

public:
  
    //construye una matriz de tamaño v * 1
    Matriz(vectorReal& v);
    Matriz(int filas, int columnas);
    int filas();
    int columnas();
    vectorReal& operator[] (int i);
    Matriz operator +(Matriz& m2);
    Matriz operator -(Matriz& m2);
    Matriz operator *(Matriz& m2);
    Matriz operator *(vectorReal& v);
    Matriz operator *(double n);
    Matriz trasponer();
    Matriz subMatriz(int n);
    vector<vectorReal> autovectores();
    vectorReal autovalores();
    vectorReal calcularMedia();

};

ostream& operator<<(ostream& os, Matriz& m);

#endif
