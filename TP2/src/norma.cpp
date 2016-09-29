#include "typedefs.h"
#include "norma.h"
#include <cmath>

using namespace std;

double normaInfinito(vectorReal &v){
    double norma= 0;
    for(unsigned int i;i<v.size();i++){
        norma = max(norma,abs(v[i]));
    }
    return norma;
}

double norma2Comun(vectorReal v){
	double norma=0.0;
	for(unsigned int i = 0; i < v.size();i++){
		norma+= pow(v[i],2);
	}
	return sqrt(norma);
}

double norma2(vectorReal v){
    double norma,normaI;
    normaI=normaInfinito(v);
    vectorReal u(v.size(),0);
    for(unsigned int i = 0; i < v.size();i++){
        u[i]=v[i]/normaI;
	}
    norma = norma2Comun(u);
    return norma*normaI;    
}

void normalizar(vectorReal &v){
	double norma = norma2(v);
	for(unsigned int i = 0; i < v.size();i++){
		v[i]= v[i]/norma;
	}
}

vectorReal resta(vectorReal &x, vectorReal &y) {
	//assert(x.size()==y.size());
	vectorReal tmp(x.size(),0.0);
	for (unsigned int i = 0; i < x.size(); i++) tmp[i] = (x[i] - y[i]);
	return tmp;
}


double productoInterno(vectorReal &u, vectorReal &v, unsigned int k1, unsigned int k2) {
	double sum = 0;
	for (unsigned int i = k1; i < k2; i++) {
		sum += u[i] * v[i];
	}
	return sum;
}

double productoInterno(vectorReal &u, vectorReal &v) {
	//assert(u.size()==v.size());
	unsigned int k1=0,k2=u.size();
	return productoInterno(u,v,k1,k2);
}

vectorReal centrarRespectoALaMedia(matrizReal &A){
	//A es de m*n
	unsigned int m = A.size();
	unsigned int n = A[0].size();
	vectorReal media(n,0);
	for(unsigned int i = 0; i < m;i++){
		for(unsigned int j = 0; j < n;j++){
			media[j]+=A[i][j]/m;
		}
	}
	for(unsigned int i = 0; i < m;i++){
		for(unsigned int j = 0; j < n;j++){
			A[i][j] = A[i][j] - media[j];
		}
	}
	return media;
}

vectorReal A_por_v(matrizReal& A, vectorReal& v) {
	int n = A.size();
	int m = A[0].size();
	//assert(m == v.size());
	vectorReal resultado(n, 0.0);
	for (int i = 0; i < n; i++) {
		/*double suma = 0.0;
		for (int j = 0; j < m; j++) {
			suma += A[i][j] * v[j];
		}
		//resultado[i] = (suma < cotaCero ? 0.0 : suma);
		resultado[i] = suma;*/
		resultado[i] = productoInterno(A[i],v);
	}
	return resultado;
}

matrizReal v_por_ut(vectorReal &v,vectorReal &u){
	unsigned int m = v.size();
	unsigned int n = u.size();
	matrizReal vXut(m,vectorReal(n,0));
	for(unsigned int i = 0; i < n;i++){
		for(unsigned int j = 0; j < n;j++){
			vXut[i][j] = v[i]*u[j];
		}
	}
	return vXut;
}

void restarA(matrizReal &A,matrizReal &B){
	unsigned int mA=A.size(),nA=A[0].size(),mB=B.size(),nB=B[0].size();
	assert(mA==mB && nA==nB);
	for(unsigned int i = 0; i < mA;i++){
		for(unsigned int j = 0; j < nA;j++){
			A[i][j] -= B[i][j];
		}
	}
}
