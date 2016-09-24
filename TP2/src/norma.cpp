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
