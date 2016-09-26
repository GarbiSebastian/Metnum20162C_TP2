#include "typedefs.h"
#include <vector>

using namespace std;

class PCA {

	public:
		PCA(matrizReal &I imagenes);
		vectorReal transformar(vectorReal &imagen);

	private:
		vector<vectorReal> autovectores;
		matrizReal calcularX(matrizReal &imagenes, vectorReal &media);
		void calcularAutovectores(matrizReal &M);


};
