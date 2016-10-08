#include "pca.h"
#include <iomanip>

using namespace std;

matrizReal subMatriz(matrizReal &A, int k, int l){
  int n = A.size()-k;
  int m = A[0].size()-l;

  matrizReal X = matrizReal(n,vectorReal(m,0));
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      X[i][j] = A[k+i][l+j];
    }
  }
}

matrizReal restaM(matrizReal A, matrizReal B) {
  int n = A.size();
  int m = A[0].size();
  assert(n==B.size() && m==B[0].size());
  matrizReal C = matrizReal(n,vectorReal(m,0));
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      C[i][j] = A[i][j] - B[i][j];
    }
  }
  return C;
}

vectorReal calcularMedia(matrizReal &M) {
  int m = M.size();
  int n = M[0].size();
  vectorReal medias;
  for(int i=0; i<n; i++){
    double media = 0;
    for(int j=0; j<m; j++){
      media += M[j][i];
    }
    medias.push_back(media / m);
  }
  return medias;
}

matrizReal producto_traspuesto(vectorReal &v1, vectorReal &v2) {
  int n1 = v1.size();
  int n2 = v2.size();

  matrizReal C = matrizReal(n1,vectorReal(n2,0));
  for(int i=0;i<n1;i++){
    for(int j=0;j<n2;j++){
      C[i][j]=v1[i]*v2[j];
    }
  }
  return C;
}

matrizReal productoEscalar(matrizReal &A, double B) {
  int n = A.size();
  int m = A[0].size();

  matrizReal C = matrizReal(n,vectorReal(m,0));
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      C[i][j]=A[i][j]*B;
    }
  }
  return C;
}

matrizReal trasponer(matrizReal &A){
	//A en Rm*n
	unsigned int m = A.size();
	unsigned int n = A[0].size();
	matrizReal At(n,vectorReal(m,0));
	for(unsigned int i=0; i < m ; i++){
		for(unsigned int j=0; j < n ; j++){
			At[j][i] = A[i][j];
		}
	}
	return At;
}

PCA::PCA(){}

matrizReal producto(matrizReal &A,matrizReal &B){//La armamos de esta manera para que queden productos internos entre vectores fila(por performance)
	//A en Rm*n B en Ro*p
	//Bt en Rp*o
	unsigned int m = A.size();
	unsigned int n = A[0].size();
	unsigned int o = B.size();
	unsigned int p = B[0].size();
	assert(n == p);
	matrizReal AxBt(m,vectorReal(o,0));
	for(unsigned int i=0; i < m ; i++){
		for(unsigned int j=0; j < o ; j++){
			AxBt[i][j] = productoInterno(A[i],B[j]);
		}
	}
	return AxBt;
}

PCA::PCA(matrizReal &imagenes, vectorEntero &labels, int alfa, int vecinos,int niter, double epsilon) {
  vectorReal media = calcularMedia(imagenes);
  matrizReal X = calcularX(imagenes, media);
  matrizReal X_t = trasponer(X);
  matrizReal M = producto(X_t, X_t);
  autovalores = vectorReal(alfa, 0);
  autovectores = matrizReal(alfa, vectorReal(M[0].size(),0));
  this -> alfa = alfa;
  this -> vecinos = vecinos;
  this -> labels = labels;
  calcularAutovectores(M, niter, epsilon);

  //aplico transformacion caracteristica a cada una de las imagenes de base
  for (int j=0; j<imagenes.size(); j++){
    imagenesTransformadas.push_back(tcpca(imagenes[j]));
  }
}

matrizReal PCA::calcularX(matrizReal &imagenes, vectorReal &media) {
  //cout << "Filas: " << imagenes.size() << "Columnas: " << imagenes[0].size() << endl;
  int n = imagenes.size();
  int m = imagenes[0].size();
  matrizReal X = matrizReal(n,vectorReal(m,0));
  for(int i=0;i<n;i++){
    X[i] = resta(imagenes[i], media);
    for (int h=0; h<X[i].size(); h++){
      X[i][h] = X[i][h] / sqrt(n-1);
    }
  }
  return X;
}

matrizReal deflacion(double lambda, vectorReal &v, matrizReal &A){
	matrizReal B = producto_traspuesto(v, v);
	return restaM(A, productoEscalar(B, lambda));
}


void PCA::calcularAutovectores(matrizReal &M, int niter, double epsilon) {
  matrizReal A = M;
  for (int i = 0; i < alfa; ++i)
  {
    //cout << "Calculando autovector " << i << endl;
    vectorReal v = vectorReal(A[0].size(), 0);
    double lambda = metodoPotencia(A, v, niter, epsilon);
    autovalores[i] = lambda;
    // cout << scientific ;
    // cout << "Autovalor " << i << ": " << lambda <<endl;
    autovectores[i] = v;
    A = deflacion(lambda, v, A);
  }
}

vectorReal PCA::tcpca(vectorReal &X){
  int n = autovectores.size();
  vectorReal Y;
  for (int i = 0; i < n; ++i)
  {
    Y.push_back(productoInterno(autovectores[i], X));
  }
  return Y;
}

int PCA::clasificar(vectorReal &X){
  vectorReal xprima = tcpca(X);
  vectorEntero indices;
  vectorReal distancias;
  buscar(vecinos, imagenesTransformadas, xprima, indices, distancias);
  int ret = votar(10, labels, indices, distancias);
  return ret;
}
