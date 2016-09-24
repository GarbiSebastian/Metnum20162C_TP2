#include "src/typedefs.h"
#include "src/metodoPotencia.h"
#include "src/metodoPotencia.cpp"
#include "src/imprimir.h"
#include "src/norma.h"
#include "src/norma.cpp"
#include <iostream>
//#include <math.h>
//#include <cstdlib>

using namespace std;

int main(int argc, char** argv){
	unsigned int n = 3;
	matrizReal A(n,vectorReal(n,0.0));
	//2.44800   0.59990  -0.15984
	A[0][0]=2.44800;
	A[0][1]=0.59990;
	A[0][2]=-0.15984;
	//0.59990   1.68510  -0.67512
	A[1][0]=0.59990;
	A[1][1]=1.68510;
	A[1][2]=-0.67512;
	//-0.15984  -0.67512   1.86690
	A[2][0]=-0.15984;
	A[2][1]=-0.67512;
	A[2][2]=1.86690;
	double epsilon = 1.0e-30;
	int niter = 1000	;
	vectorReal v(n,0);
	
	v= randVector(n);
	cout << norma2(v) << endl;
	normalizar(v);
	cout << norma2(v) << endl;
	double lambda = metodoPotencia(A,v,niter,epsilon);
	cout << lambda <<endl;
	imprimir(v);
	return 0;
}
