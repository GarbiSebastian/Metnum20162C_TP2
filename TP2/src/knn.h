#include "typedefs.h"
#include "norma.h"
#include <math.h>

using namespace std;

void buscar(int k_vecinos, matrizReal & train, vectorReal & imagen, vectorEntero & indices , vectorReal &distancias);
int votar(unsigned int cant_categorias, vectorEntero & labels, vectorEntero & indices , vectorReal &distancias);
