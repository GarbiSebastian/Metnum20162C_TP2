#include "knn.h"


using namespace std;
real dameDistancia(vectorReal & u, vectorReal & v){
	return norma2(resta(u,v));
}

void insertame(unsigned int indice, real distancia, vectorEntero &indices, vectorReal& distancias){
	unsigned int k_vecinos = indices.size();
	int j = k_vecinos-1;
	if ( distancia < distancias[j] || indices[j] == -1){
		indices[j] = indice;
		distancias[j] = distancia;
		while (j > 0 && (distancias[j-1] > distancias[j] || indices[j-1] == -1)){
			distancia = distancias[j];
			indice = indices[j];
			distancias[j] = distancias[j-1];
			indices[j] = indices[j-1];
			distancias[j-1] = distancia;
			indices[j-1] = indice;
			j--;	
		}
		

	}
	
}

void buscar(int k_vecinos, matrizReal & train, vectorReal & imagen, vectorEntero & indices , vectorReal &distancias){
	
	indices = vectorEntero(k_vecinos, -1);
	distancias = vectorReal(k_vecinos, -1);
	
	//busco vecinos mas cercanos
	for (unsigned int i = 0; i < train.size() ; i++){
		real distancia = dameDistancia(imagen , train[i]);
		insertame(i, distancia, indices, distancias);
	}
	
	
	
	
}

int votar(unsigned int cant_categorias, vectorEntero & labels, vectorEntero & indices , vectorReal &distancias){
	vectorEntero bucket(cant_categorias, 0);
	for(unsigned int i = 0; i < indices.size(); i++){
		bucket[labels[indices[i]]] ++;
	}
	unsigned int j = 0;
	int max = 0;
	for(unsigned int i = 0; i < cant_categorias; i++){
		if (bucket[i] > max){
			max = bucket[i];
			j = i;
		}
	}
	imprimir(bucket);
	return j;
}
