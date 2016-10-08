#include "pca.h"
#include <iostream>
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
/*
matrizReal producto(matrizReal &A, matrizReal &B) {
  int n = A.size();
  int m = A[0].size();

  int n1 = B.size();
  int m1 = B[0].size();

  assert(m == n1);

  matrizReal C = matrizReal(n,vectorReal(m1,0));
  for(int i=0;i<n;i++){
    for(int j=0;j<m1;j++){
      C[i][j]=0;
      for(int k=0;k<m;k++){
        C[i][j]=C[i][j]+(A[i][k] * B[k][j]);
      }
    }
  }
  return C;
}*/

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
  cout << "X, " << "Filas: " << X.size() << " Columnas: " << X[0].size() << endl;
  //cout << "X: " << X.size() << " " << X[0].size() <<endl;
  //cout << "X_t: " << X_t.size() << " " << X_t[0].size() <<endl
  //cout << "calcule x" << endl;
  matrizReal M = producto(X_t, X_t);
  autovalores = vectorReal(alfa, 0);
  autovectores = matrizReal(alfa, vectorReal(M[0].size(),0));
  //cout << "pase producto" <<endl;
  this -> alfa = alfa;
  this -> vecinos = vecinos;
  this -> labels = labels;
  //cout << "empiezo con autovectores" << endl;
  calcularAutovectores(M, niter, epsilon);
  //cout << "termino con autovectores" << endl;

  //aplico transformacion caracteristica a cada una de las imagenes de base
  for (int j=0; j<imagenes.size(); j++){
  	//cout << "transforme imagen " << j << endl;
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

/*
vectorReal PCA::transformar(vectorReal &imagen) {
  vectorReal res;
  for(int i=0; i<autovectores.size(); i++){
    res.push_back(productoInterno(autovectores[i], imagen));
  }
  return res;
}

matrizReal PCA::deflacion(vectorReal &v1, matrizReal &A){
  vectorReal e1 = vectorReal(0,v1.size());
  e1[0] = 1;
  vectorReal w = (v1-e1)/norma2(v1-e1);
  matrizReal H = resta(I,productoEscalar(producto(w,trasponer(w)), 2));
  matrizReal HAH = producto(producto(H,A),H);
  return subMatriz(HAH,1,1);
}
*/

matrizReal deflacion(double lambda, vectorReal &v, matrizReal &A){
	//cout << "entre en producto traspuesto" << endl;
	matrizReal B = producto_traspuesto(v, v);
  //double norma = norma2(v);
  //double norma2Cuadrado = norma2 * norma2;
	//cout << "entre en producto escalar" << endl;
	return restaM(A, productoEscalar(B, lambda));
}


void PCA::calcularAutovectores(matrizReal &M, int niter, double epsilon) {
  //data.open("/pca/data" + "_" + niter)
  //if (!data) {
    matrizReal A = M;
    for (int i = 0; i < alfa; ++i)
    {
      cout << "Calculando autovector " << i << endl;
      vectorReal v = vectorReal(A[0].size(), 0);
      double lambda = metodoPotencia(A, v, niter, epsilon);
      autovalores[i] = lambda;
      cout << scientific ;
      cout << "Autovalor " << i << ": " << lambda <<endl;

      // for (int h=0; h<v.size(); h++) {
      //   cout << v[h] << " ";
      // }

      //escribirData(autovalores);
      autovectores[i] = v;
      A = deflacion(lambda, v, A);
    }
  // } else {
  //   cargarData();
  // }
}

// void PCA::cargarData() {
//
// }
//
// void PCA::escribirData(autovalores) {
//
// }

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
