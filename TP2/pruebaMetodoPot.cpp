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
#include "src/knn.h"
#include "src/knn.cpp"
#include "src/plsda.h"
#include "src/plsda.cpp"

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

void prueba6(){
	unsigned int cant_muestras = 200;
	unsigned int k_vecinos = 10;
	unsigned int tam_imagen = 50;
	matrizReal train(cant_muestras, vectorReal(tam_imagen, 0));
	vectorReal imagen(tam_imagen, 0);
	vectorReal distancias(k_vecinos, 0);
	vectorEntero indices(k_vecinos, 0);
	
	for (unsigned int i = 0; i < cant_muestras; i++){
		train[i][i%tam_imagen] = i;
	}
	
	imagen[5] = 100;
	buscar(k_vecinos, train, imagen, indices , distancias);
	//imprimir(indices);
	//imprimir(distancias);
	
}

void prueba7(){
	unsigned int cant_muestras = 200;
	unsigned int k_vecinos = 10;
	unsigned int tam_imagen = 50;
	matrizReal train(cant_muestras, vectorReal(tam_imagen, 0));
	vectorReal imagen(tam_imagen, 0);
	vectorReal distancias(k_vecinos, 0);
	vectorEntero indices(k_vecinos, 0);
	vectorEntero labels(cant_muestras, 0);
	unsigned int cant_categorias = 4;
	
	for (unsigned int i = 0; i < cant_muestras; i++){
		train[i][i%tam_imagen] = i;
		labels[i] = i % cant_categorias;
	}
	
	imagen[5] = 100;
	buscar(k_vecinos, train, imagen, indices , distancias);
	
	//cout << votar(cant_categorias, labels, indices, distancias) << endl ;
}

void prueba8(){
	unsigned int m = 5;
	unsigned int n = 3;
	matrizReal A(m,vectorReal(n,0));
	int a= 0;
	for(unsigned int i =0;i<m;i++){
		for(unsigned int j =0;j<n;j++){
			A[i][j] = a++;
		}
	}
	//imprimir(A);
	vectorReal media = centrarRespectoALaMedia(A);
/*	cout << endl;
	imprimir(A);
	cout << endl;
	imprimir(media);*/
}

void prueba9(){
	unsigned int m = 2;
	unsigned int n = 4;
	matrizReal A(m,vectorReal(n,0));
	int k = 0 ;
	for(unsigned int i = 0;i< m;i++){
		for(unsigned int j = 0;j< n;j++){
			A[i][j]= k++;
		}
	}
	matrizReal At = transponer(A);
	matrizReal AxAt = A_por_Bt(A,A);
	matrizReal AtxA = A_por_Bt(At,At);
/*	imprimir(A);
	cout << endl;
	imprimir(At);
	cout << endl;
	imprimir(AxAt);
	cout << endl;
	imprimir(AtxA);*/
	
	vectorReal v(3,0),u(3,0);
	v[0] = 1;
	v[1] = 2;
	v[2] = 3;
	u[0] = 4;
	u[1] = 5;
	u[2] = 6;
	matrizReal B = v_por_ut(v,u);
	imprimir(B);
	restarA(B,B);
	imprimir(B);
}

void prueba10(){
	matrizReal A(3,vectorReal(3,0));
	A[0][0] = 1;
	A[1][1] = 3;
	A[2][2] = 2;
	vectorReal v;
	double lambda = metodoPotencia(A,v,1000,1.0e-10);
	imprimir(v);
	cout << endl << "lambda "<<lambda<< endl;
	A[0][0]=1;
	A[0][1]=2;
	A[0][2]=3;
	A[1][0]=4;
	A[1][1]=5;
	A[1][2]=6;
	A[2][0]=7;
	A[2][1]=8;
	A[2][2]=9;
	lambda = metodoPotencia(A,v,1000,1.0e-10);
	imprimir(v);
	cout << endl << "lambda "<<lambda<< endl;
	
	v = vectorReal(5,0);
	v[0] = 1;
	v[1] = 2;
	v[2] = 3;
	v[3] = 4;
	v[4] = 5;
	vectorReal u(3,0);
	u[0]=6;
	u[1]=7;
	u[2]=8;
	A = v_por_ut(v,u);
	imprimir(A);
}

int main(int argc, char** argv){
	cout << scientific << setprecision(5);
	/*cout << endl << "prueba 1 Inicio" << endl;
	prueba1();
	cout << endl << "prueba 1 OK" << endl;
	cout << endl << "prueba 2 Inicio" << endl;
	prueba2();
	cout << endl << "prueba 2 OK" << endl;
	cout << endl << "prueba 3 Inicio" << endl;
	prueba3();
	cout << endl << "prueba 3 OK" << endl;
	cout << endl << "prueba 4 Inicio" << endl;
	prueba4();	
	cout << endl << "prueba 4 OK" << endl;
	cout << endl << "prueba 5 Inicio" << endl;
	prueba5();	
	cout << endl << "prueba 5 OK" << endl;
	cout << endl << "prueba 6 Inicio" << endl;
	prueba6();
	cout << endl << "prueba 6 OK" << endl;
	cout << endl << "prueba 7 Inicio" << endl;
	prueba7();
	cout << endl << "prueba 7 OK" << endl;
	cout << endl << "prueba 8 Inicio" << endl;
	prueba8();
	cout << endl << "prueba 8 OK" << endl;
	cout << endl << "prueba 9 Inicio" << endl;
	prueba9();
	cout << endl << "prueba 9 OK" << endl;
	*/
	cout << endl << "prueba 10 Inicio" << endl;
	prueba10();
	cout << endl << "prueba 10 OK" << endl;
	return 0;
}
