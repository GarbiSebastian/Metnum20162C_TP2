#include <cstdlib>
#include <cmath>
#include "metodoPotencia.h"
#include "norma.h"
#include "imprimir.h"
#include <iostream>
#include <iomanip>


double metodoPotencia(matrizReal &B, vectorReal &v, int niter, double epsilon) {
	v.clear();
	vectorReal tmp(B.size(),0);
	//MetodoPotencia(B,x 0 ,niter)
	
	
	
	//Devolver λ, v .
	
	//v ← x0
	v = randVector(B.size());
	cout << fixed << setprecision(17);
	//Para i = 1, . . . , niter
	for (int i = 0; i < niter; i++) {
		tmp= A_por_v(B, v);
		normalizar(tmp);
		//assert(abs(norma2(tmp)-1) <= 0.1e-10);
		if (norma2(resta(v, tmp)) <= epsilon){
			cout << "salio con " << i << " iteraciones" << endl;
			break;
		} //si no cambio al menos epsilon en norma 2
		//v ← Bv/||Bv||
		v = tmp;
	}
	//λ ← v'*Bv/(v'*v) //v siempre tiene norma uno entonces norma Cuadrada de v es 1
	//assert(norma2(v)-1 <= 0.1e-10);
	vectorReal Bv = A_por_v(B,v);
	return productoInterno(v,Bv);//producto interno entre v y Bv
}


vectorReal randVector(unsigned int n) {
	vectorReal r(n, 0.0);
	srand(1);
	for (unsigned int i = 0; i < n; i++) {
		r[i] = (rand() % 100) / 100.0;
	}
	return r;
}
