using namespace std;

vectorReal media calcularMedia(matrizReal M) {
  int m = matrizReal.size();
  int n = matrizReal[0].size(0);
  vectorReal medias;
  double media = 0;
  for(int i=0; i<n; i++){
    for(int j=0; j<m; j++){
      media += matrizReal[j][i];
    }
    medias.push_back(media / n);
    media = 0;
  }
  return medias;
}

matrizReal producto(matrizReal &A, matrizReal &B) {
  int n = A.size();
  int m = A[0].size();

  int n1 = B.size();
  int m1 = B[0].size();

  assert(m == n1);

  matrizReal C = matrizReal(n,m1);
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
  matrizReal T = matrizReal(n,m);
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      T[i][j] = T[j][i];
    }
  }
  return T;
}


PCA::PCA(matrizReal &I imagenes) {
  vectorReal media = calcularMedia(imagenes);
  matrizReal X = calcularX(imagenes, media);
  matrizReal X_t = trasponer(X);
  matrizReal M = producto(X, X_t);
  calcularAutovectores(M);
}

matrizReal PCA::calcularX(matrizReal &imagenes, vectorReal &media) {
  int n = imagenes.size();
  int m = imagenes[0].size();
  matrizReal X = matrizReal(n,m);
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      X[i][j] = (imagenes[i][j] - media) / sqrt(m-1)
    }
  }
  return X;
}

vectorReal PCA::transformar(vectorReal &imagen) {
  //TODO: Hacer el producto interno entre la imagen y cada autovector calculdo previamente para cambiarlo de base
}

void PCA::calcularAutovectores(matrizReal &M) {
  //agus completa aca
}

