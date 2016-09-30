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

matrizReal productoEscalar(matrizReal &A, double B) {
  int n = A.size();
  int m = A[0].size();

  matrizReal C = matrizReal(n,vectorReal(m,0));
  for(int i=0;i<n;i++){
    for(int j=0;j<m1;j++){
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

matrizReal PCA::deflacion(vectorReal &v1, matrizReal &A){
  vectorReal e1 = vectorReal(0,v1.size());
  e1[0] = 1;
  vectorReal w = (v1-e1)/norma2(v1-e1);
  matrizReal H = resta(I,productoEscalar(producto(w,trasponer(w)), 2));
  matrizReal HAH = producto(producto(H,A),H);
  return subMatriz(A,1,1); 
}

matrizReal PCA::subMatriz(matrizReal &A, int k, int l){
  int n = A.size()-k;
  int m = A[0].size()-l;

  matrizReal X = matrizReal(n,vectorReal(m,0));
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      X[i][j] = A[k+i][l+j];
    }
  }

}
/*
matrizReal PCA::deflacion(double lambda1, vectorReal &v1, matrizReal &A){
  A = A - lambda1 * producto(v1, trasponer(v1));
}
*/

void PCA::calcularAutovectores(matrizReal &M, int cantAutovectores, vector<vectorReal> &vs, vectorReal &lambdas) {
  matrizReal A = M;
  for (int i = 0; i < cantAutovectores; ++i)
  {
    vectorReal v = r(n, 0.0);
    lambdas.push_back(metodoPotencia(&M, &v, 10, 0.15));
    vs.push_back(v);
    A = deflacion(v, A);
  }

  vector<vectorReal> PCA::TCPCA(vectorReal &X, vector<vectorReal> &autovectores){
    int n = autovectores.size();
    std::vector<vectorReal> Y = vectorReal(0, X.size());
    for (int i = 0; i < n; ++i)
    {
      Y.push_back(producto(trasponer(autovectores[i]), X));
    }
    return Y;
  }

}

