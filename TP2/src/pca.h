#include "typedefs.h"
#include "norma.h"
#include <assert.h>
#include <math.h>
#include <vector>

using namespace std;

class PCA {

	public:
		PCA(matrizReal &imagenes, int alfa);
		vectorReal transformar(vectorReal &imagen);

	private:
		vector<vectorReal> autovectores;
		vector<double> autovalores;
		matrizReal imagenesTransformadas;
		int alfa;

		//metodos privados auxiliares
		matrizReal calcularX(matrizReal &imagenes, vectorReal &media);
		void calcularAutovectores(matrizReal &M);


};
