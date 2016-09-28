#include "typedefs.h"
#include <vector>

using namespace std;

double norma2(vectorReal v);
void normalizar(vectorReal &v);
vectorReal resta(vectorReal &x, vectorReal &y);
double productoInterno(vectorReal &u, vectorReal &v, unsigned int k1, unsigned int k2);
double productoInterno(vectorReal &u, vectorReal &v);
vectorReal centrarRespectoALaMedia(matrizReal &A);
vectorReal A_por_v(matrizReal& A, vectorReal& v);
matrizReal v_por_ut(vectorReal &v,vectorReal &u);
void restarA(matrizReal &A,matrizReal &B);
