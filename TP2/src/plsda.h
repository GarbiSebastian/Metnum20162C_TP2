#include "typedefs.h"
using namespace std;

matrizReal A_por_Bt(matrizReal &A,matrizReal &B);
matrizReal transponer(matrizReal &A);
vectorReal preprocesarTrain(matrizReal &train,vectorEntero &labels, matrizReal &Y,int gamma_plsda, int cant_categorias);
vectorReal plsda(matrizReal &X,matrizReal &Y, int gamma_plsda, matrizReal &V, int niter, double epsilon);
