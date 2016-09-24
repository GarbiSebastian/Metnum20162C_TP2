#include "typedefs.h"
#include "norma.h"
#include <math.h>

using namespace std;

double norma2(vectorReal v){
	double norma=0.0;
	for(unsigned int i = 0; i < v.size();i++){
		norma+= v[i]*v[i];
	}
	return sqrt(norma);
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
	//return (sum < cotaCero ? 0.0 : sum);
	return sum;
}

double productoInterno(vectorReal &u, vectorReal &v) {
	//assert(u.size()==v.size());
	unsigned int k1=0,k2=u.size();
	return productoInterno(u,v,k1,k2);
}
