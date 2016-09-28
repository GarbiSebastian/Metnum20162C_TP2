#include <cstdlib>
#include <cmath>
#include "metodoPotencia.h"
#include "norma.h"
#include "imprimir.h"
#include <iostream>


double metodoPotencia(matrizReal &B, vectorReal &v, int niter, double epsilon) {
	//MetodoPotencia(B,x 0 ,niter)
	//v ← x0
	//Para i = 1, . . . , niter
	//	v ← Bv/||Bv||
	//Fin Para
	//λ ← v'*Bv/(v'*v)
	//Devolver λ, v .
	v.clear();
	
	vectorReal tmp(B.size(),0);
	v = randVector(B.size());
	for (int i = 0; i < niter; i++) {
		
		tmp= A_por_v(B, v);
		
		normalizar(tmp);
		
		
		assert(abs(norma2(tmp)-1) <= epsilon);
		if (norma2(resta(v, tmp)) <= epsilon){
			//cout << "salio con " << i << " iteraciones" << endl;
			//break;
		} //si no cambio al menos epsilon en norma 2
		v = tmp;
	}
	
	//v siempre tiene norma uno entonces norma Cuadrada de v es 1
	assert(norma2(v)-1 <= epsilon);
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
