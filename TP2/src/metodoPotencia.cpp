#include <cstdlib>
#include "metodoPotencia.h"
#include "norma.h"
#include <iostream>

double productoInterno(vectorReal &u, vectorReal &v, unsigned int k1, unsigned int k2) {
	double sum = 0;
	for (unsigned int i = k1; i <= k2; i++) {
		sum += u[i] * v[i];
	}
	//return (sum < cotaCero ? 0.0 : sum);
	return sum;
}

double productoInterno(vectorReal &u, vectorReal &v) {
	//assert(u.size()==v.size());
	unsigned int k1=0,k2=u.size();
	double sum = 0;
	for (unsigned int i = k1; i <= k2; i++) {
		sum += u[i] * v[i];
	}
	//return (sum < cotaCero ? 0.0 : sum);
	return sum;
}


vectorReal A_por_v(matrizReal& A, vectorReal& v) {
	int n = A.size();
	int m = A[0].size();
	//assert(m == v.size());
	vectorReal resultado(n, 0.0);
	for (int i = 0; i < n; i++) {
		double suma = 0.0;
		for (int j = 0; j < m; j++) {
			suma += A[i][j] * v[j];
		}
		//resultado[i] = (suma < cotaCero ? 0.0 : suma);
		resultado[i] = suma;
	}
	return resultado;
}


double metodoPotencia(matrizReal &B, vectorReal &v, int niter, double epsilon) {
	//MetodoPotencia(B,x 0 ,niter)
	//v ← x0
	//Para i = 1, . . . , niter
	//	v ← Bv/||Bv||
	//Fin Para
	//λ ← v'*Bv/(v'*v)
	//Devolver λ, v .
	vectorReal tmp(v.size(),0.0);
	//assert(B.size() == v.size());
	v = randVector(v.size());
	for (int i = 0; i < niter; i++) {
		tmp= A_por_v(B, v);
		normalizar(tmp);
		cout << norma2(tmp);
		if (norma2(resta(v, tmp)) <= epsilon) break; //si no cambio al menos epsilon en norma 2
		v = tmp;
	}
	//double normaCuadrada_v = pow(norma2(v), 2);
	//vectorReal Bv = A_por_v(B,v);
	//return productoEscalar(v,Bv)/normaCuadrada_v;//producto interno entre v y Bv
	
	//v siempre tiene norma uno entonces normaCuadrada_v es 1
	vectorReal Bv = A_por_v(B,v);
	return productoInterno(v,Bv);//producto interno entre v y Bv
}

vectorReal resta(vectorReal &x, vectorReal &y) {
	//assert(x.size()==y.size());
	vectorReal tmp(x.size(),0.0);
	for (unsigned int i = 0; i < x.size(); i++) tmp[i] = (x[i] - y[i]);
	return tmp;
}

vectorReal randVector(unsigned int n) {
	vectorReal r(n, 0.0);
	srand(1);
	for (unsigned int i = 0; i < n; i++) {
		r[i] = (rand() % 100) / 100.0;
	}
	return r;
}
