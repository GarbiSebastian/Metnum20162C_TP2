#include "typedefs.h"
#include "norma.h"
#include "metodoPotencia.h"
#include "knn.h"
#include <assert.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class PCA {

	public:
		PCA();
		PCA(matrizReal &imagenes, vectorEntero &labels, int alfa, int vecinos, int niter, double epsilon);
		vectorReal tcpca(vectorReal &v);
		int clasificar(vectorReal &v);
		vectorReal autovalores;

	private:
		vector<vectorReal> autovectores;
		matrizReal imagenesTransformadas;
		int alfa;
		int vecinos;
		vectorEntero labels;

		//metodos privados auxiliares
		matrizReal calcularX(matrizReal &imagenes, vectorReal &media);
		void calcularAutovectores(matrizReal &M, int niter, double epsilon);


};
