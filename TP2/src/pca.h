#include "typedefs.h"
#include "norma.h"
#include "metodoPotencia.h"
#include "knn.h"
#include <assert.h>
#include <math.h>
#include <vector>

using namespace std;

class PCA {

	public:
		PCA(matrizReal &imagenes, vectorEntero &labels, int alfa, int vecinos);
		vectorReal tcpca(vectorReal &v);
		int clasificar(vectorReal &v);

	private:
		vector<vectorReal> autovectores;
		vector<double> autovalores;
		matrizReal imagenesTransformadas;
		int alfa;
		int vecinos;
		vectorEntero labels;

		//metodos privados auxiliares
		matrizReal calcularX(matrizReal &imagenes, vectorReal &media);
		//void calcularAutovectores(matrizReal &M);


};
