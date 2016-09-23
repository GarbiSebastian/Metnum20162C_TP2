#include "typedefs.h"
#include "norma.h"
#include <math.h>

using namespace std;

double norma2(vectorReal &v){
	double norma2=0;
	for(unsigned int i = 1; i < v.size();i++){
		norma2+= v[i]*v[i];
	}
	return sqrt(norma2);
}

void normalizar(vectorReal &v){
	double norma = norma2(v);
	for(unsigned int i = 1; i < v.size();i++){
		v[i]= v[i]/norma;
	}
}
