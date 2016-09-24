#include "typedefs.h"
#include <math.h>
#include <assert.h>

using namespace std;

double metodoPotencia(matrizReal &B, vectorReal &v, int niter, double epsilon);
vectorReal randVector(unsigned int n);
vectorReal resta(vectorReal &x, vectorReal &y);
double productoInterno(vectorReal &u, vectorReal &v, unsigned int k1, unsigned int k2);
double productoInterno(vectorReal &u, vectorReal &v);

