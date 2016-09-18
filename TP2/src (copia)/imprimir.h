#include "typedefs.h"
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

template<typename T>
void imprimir(vector<T> &v,ostream &out){
	out << fixed << setprecision(6);
	out << (double)v[0];
	for(unsigned int i = 1; i < v.size();i++){
		out << " " << (double)v[i]; 
	}
	out << endl;
}

template<typename T> 
void imprimir(vector< vector< T > > &m,ofstream &out){
	for(unsigned int i=0; i< m.size();i++){
		imprimir(m[i],out);
	}
}
