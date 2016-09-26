#include "pca.h"

using namespace std;

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


PCA::PCA(matrizReal &imagenes, int alfa) {
  vectorReal media = calcularMedia(imagenes);
  matrizReal X = calcularX(imagenes, media);
  matrizReal X_t = trasponer(X);
  matrizReal M = producto(X, X_t);
  this -> alfa = alfa;
  calcularAutovectores(M);
  //aplico transformacion caracteristica a cada una de las imagenes de base
  for (int j=0; j<imagenes.size(); j++){
    imagenesTransformadas.push_back(transformar(imagenes[j]));
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

vectorReal PCA::transformar(vectorReal &imagen) {
  vectorReal res;
  for(int i=0; i<autovectores.size(); i++){
    res.push_back(productoInterno(autovectores[i], imagen));
  }
  return res;
}

void PCA::calcularAutovectores(matrizReal &M) {
  //TODO: COMPLETAR ESTO!
  //calcular los alfa autovectores y autovalores y guardarlos como variables de instancia.
}

