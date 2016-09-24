#include <cstdlib>
#include "src/typedefs.h"
#include "src/metodoPotencia.h"
#include "src/metodoPotencia.cpp"
#include "src/imprimir.h"
#include "src/norma.h"
#include "src/norma.cpp"
#include <iostream>
#include <cmath> 
#include <iomanip>


using namespace std;

void prueba1(){
	unsigned int n=3;
	matrizReal A(n,vectorReal(n,0));
	A[0][0] = 9;
	A[0][1] = 8;
	A[0][2] = 7;
	A[1][0] = 4;
	A[1][1] = 5;
	A[1][2] = 6;
	A[2][0] = 2;
	A[2][1] = 3;
	A[2][2] = 1;
	vectorReal v(n,0);
	v[0] = 1;
	v[1] = 2;
	v[2] = 3;
	vectorReal u = A_por_v(A,v);
	//imprimir(A);
	//imprimir(v);
	//imprimir(u);
	assert(u[0] == 46 && u[1] == 32 && u[2]==11);
}

void prueba2(){
	unsigned int n = 3;
	double epsilon = 1.0e-15 ;
	vectorReal u(n,0);
	vectorReal v(n,0);
	u[0]=3;
	u[1]=2;
	u[2]=1;
	v[0]=4;
	v[1]=3;
	v[2]=2;
	// 3*4+ 2*3+ 1*2 = 20
	double r = productoInterno(u,v);
	assert( abs(r - 20) <= epsilon);
}

void prueba3(){
	unsigned int n = 3;
	double epsilon = 1.0e-15;
	vectorReal u(n,0);
	u[0]=3;
	u[1]=2;
	u[2]=sqrt(3);
//	cout << norma2(u)<<endl;
	assert(abs(norma2(u) - 4) <=epsilon);
}


void prueba4(){
	unsigned int n = 3;
	matrizReal A(n,vectorReal(n,0.0));
	A[0][0]=3;
	A[0][1]=0;
	A[0][2]=0;
	A[1][0]=0;
	A[1][1]=2;
	A[1][2]=0;
	A[2][0]=0;
	A[2][1]=0;
	A[2][2]=1;
	double epsilon = 1.0e-15 ;
	int niter = 1000;
	vectorReal v(n,0);
	double lambda = metodoPotencia(A,v,niter,epsilon);
//	cout << lambda <<endl;
//	imprimir(v);
	assert(abs(lambda - 3.0) <= epsilon);
//	cout << v[0] << " " << v[1] << " " << v[2] << " " << epsilon << endl;
	assert(abs(v[0]-1) <= epsilon*10);
	assert(abs(v[1]) <= epsilon*10);
	assert(abs(v[2]) <= epsilon*10);
}

void prueba5(){
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
	double epsilon = 1.0e-15;
	int niter = 100000;
	vectorReal v(n,0);
	double lambda = metodoPotencia(A,v,niter,epsilon);
	assert(abs(lambda - 3) <= 1.0e-5);
//	imprimir(v);
}

int main(int argc, char** argv){
	prueba1();
	cout << endl << "prueba 1 OK" << endl;
	prueba2();
	cout << endl << "prueba 2 OK" << endl;
	prueba3();
	cout << endl << "prueba 3 OK" << endl;
	prueba4();	
	cout << endl << "prueba 4 OK" << endl;
	prueba5();	
	cout << endl << "prueba 5 OK" << endl;
	return 0;
}
