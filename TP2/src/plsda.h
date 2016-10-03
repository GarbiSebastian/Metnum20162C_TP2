#include "typedefs.h"
using namespace std;

matrizReal A_por_Bt(matrizReal &A,matrizReal &B);
matrizReal transponer(matrizReal &A);
void armarY(vectorEntero &labels, matrizReal &Y, int cant_categorias);
vectorReal plsda(matrizReal &X,matrizReal &Y, int gamma_plsda, matrizReal &V, int niter, double epsilon);
