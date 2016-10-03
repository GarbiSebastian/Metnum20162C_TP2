#include "pca.h"

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

matrizReal resta(matrizReal A, matrizReal B) {
  int n = A.size();
  int m = A[0].size();
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
    medias.push_back(media / n);
  }
  return medias;
}

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

matrizReal trasponer(matrizReal &A) {
  int n = A.size();
  int m = A[0].size();
  matrizReal T = matrizReal(n,vectorReal(m,0));
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      T[i][j] = T[j][i];
    }
  }
  return T;
}


PCA::PCA(matrizReal &imagenes, vectorEntero &labels, int alfa, int vecinos) {
  vectorReal media = calcularMedia(imagenes);
  matrizReal X = calcularX(imagenes, media);
  matrizReal X_t = trasponer(X);
  matrizReal M = producto(X, X_t);
  this -> alfa = alfa;
  this -> vecinos = vecinos;
  this -> labels = labels;
  calcularAutovectores(M);
  //aplico transformacion caracteristica a cada una de las imagenes de base
  for (int j=0; j<imagenes.size(); j++){
    imagenesTransformadas.push_back(tcpca(imagenes[j]));
  }
}

matrizReal PCA::calcularX(matrizReal &imagenes, vectorReal &media) {
  int n = imagenes.size();
  int m = imagenes[0].size();
  matrizReal X = matrizReal(n,vectorReal(m,0));
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      X[i][j] = (imagenes[i][j] - media[j]) / sqrt(m-1);
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
  matrizReal B = producto_traspuesto(v, v);
  A = resta(A, productoEscalar(B, lambda));
}


void PCA::calcularAutovectores(matrizReal &M) {
  matrizReal A = M;
  for (int i = 0; i < alfa; ++i)
  {
    vectorReal v = vectorReal(A[0].size(), 0);
    double lambda = metodoPotencia(M, v, 10, 0.15);
    autovalores.push_back(lambda);
    autovectores.push_back(v);
    A = deflacion(lambda, v, A);
  }
}

vectorReal PCA::tcpca(vectorReal &X){
  int n = autovectores.size();
  vectorReal Y = vectorReal(X.size(), 0);
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