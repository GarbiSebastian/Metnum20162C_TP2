#include "typedefs.h"
#include <math.h>
#include <assert.h>
#include "matriz.h"

using namespace std;

class Matriz;

double metodoPotencia(Matriz &B, vectorReal &v, int niter, double epsilon);
vectorReal randVector(unsigned int n);
