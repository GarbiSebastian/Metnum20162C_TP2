#include <cstdlib>
#include "metodoPotencia.h"
#include "norma.h"

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


double metodoPotencia(matrizReal &A, vectorReal &q, int iter, double epsilon) {
	vectorReal tmp(A.size(),0.0);
	//assert(A.size() == q.size());
	q = randVector(A.size());
	for (int i = 0; i < iter; i++) {
		tmp = A_por_v(A, q);
		normalizar(tmp);
		if (norma2(resta(q, tmp)) <= epsilon) break; //si no cambio al menos epsilon en norma 2
		q = tmp;
	}
	double normaCuadrada_q = pow(norma2(q), 2);
	//vectorReal vt_A = vt_por_A(q, A);
	//return productoVectorial(vt_A, q) / normaCuadrada_q;*/
	return 0.0;
}

vectorReal resta(vectorReal &x, vectorReal &y) {
	//assert(x.size()==y.size());
	vectorReal tmp(x.size(),0.0);
	for (int i = 0; i < x.size(); i++) tmp[i] = (x[i] - y[i]);
	return tmp;
}

vectorReal randVector(int n) {
	vectorReal r(n, 0.0);
	srand(1);
	for (int i = 0; i < n; i++) {
		r[i] = (rand() % 100) / 100.0;
	}
	return r;
}

/*#include <cstdlib>
#include "typedefs.h"
#include "metodoPotencia.h"
#include <math.h>
#include <assert.h>
#include "norma.h"

using namespace std;


vectorReal A_por_v(matrizReal& A, vectorReal& v) {
	int n = A.size();
	int m = A[0].size();
	assert(m == v.size());
	vectorReal resultado(n, 0.0); // A es n*m, At es m*n , At*v m*1
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

double metodoPotencia(matrizReal &A, vectorReal &q, int iter, double epsilon) {
	vectorReal tmp;
	//assert(A.size() == q.size());
	//q = randVector(A.size());
	/*for (int i = 0; i < iter; i++) {
		tmp = A_por_v(A, q);
		normalizar(tmp);
		if (norma2(resta(q, tmp)) <= epsilon) break; //si no cambio al menos epsilon en norma 2
		q = tmp;
	}
	double normaCuadrada_q = pow(norma2(q), 2);
	vectorReal vt_A = vt_por_A(q, A);
	return productoVectorial(vt_A, q) / normaCuadrada_q;
	return 0.0;
}*/

