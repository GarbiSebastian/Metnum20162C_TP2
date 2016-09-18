#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include "typedefs.h"
#include "imprimir.h"
#include <sstream>
#include <math.h>

using namespace std;
extern const string trainDB = "train.csv";
extern const string testDB = "test.csv";
extern const string config = "config.tp";

void leerConfiguracionGlobal(int &niter, int &cantPixeles, int &cantMuestras){
	ifstream archivoConfig(config.c_str());
	if(archivoConfig.fail()){
		niter = 1000;
		cantPixeles=784;
		cantMuestras=42000;
	}else{
		cout << "Se utilizaran los valores de prueba del archivo \"config.tp\". Elimine dicho archivo para tomar los parámetros totales"<< endl;
		archivoConfig >> niter;
		archivoConfig >> cantPixeles;
		archivoConfig >> cantMuestras;
		cout << "niter: " << niter << endl <<  "cantPixeles: " << cantPixeles << endl << "cantMuestras: " << cantMuestras << endl;		
	}
}

void leerConfiguracionExperimento(ifstream &archivoEntrada,string &path, int &k_vecinos,int &alfa_pca, int &gamma_plsda, int &K_folds){
	archivoEntrada >> path;
	archivoEntrada >> k_vecinos;
	archivoEntrada >> alfa_pca;
	archivoEntrada >> gamma_plsda;
	archivoEntrada >> K_folds;
}

void inicializar(int &metodo,ifstream &archivoEntrada,ofstream &archivoSalida, int &niter, int &cantPixeles, int &cantMuestras, string &path, int &k_vecinos, int &alfa_pca, int &gamma_plsda, int &K_folds,int argc, char** argv){
	if (argc < 4) {
		cout << "Error de cantidad de parametros" << endl;
		exit(0);
	}
	string entrada = argv[1];
	string salida = argv[2];
	
	metodo = atoi(argv[3]);
	archivoEntrada.open(entrada.c_str());
	if(archivoEntrada.fail()){
		cout << "Falla Archivo de entrada" << endl;
		exit(0);
	}
	archivoSalida.open(salida.c_str());
	if(archivoSalida.fail()){
		cout << "Falla Archivo de salida" << endl;
		exit(0);
	}
	leerConfiguracionGlobal(niter,cantPixeles, cantMuestras);
	leerConfiguracionExperimento(archivoEntrada, path, k_vecinos, alfa_pca, gamma_plsda, K_folds);
	//cout << "entrada: " << entrada << endl << "salida: " << salida << endl << "metodo: "<< metodo << endl;
	//cout << "path: " << path << endl << "k_vecinos: " << k_vecinos << endl <<  "alfa_pca: " << alfa_pca << endl << "gamma_plsda: " << gamma_plsda << endl << "K_folds: " << K_folds << endl;
}

void armarMatrizTrain(string path,matrizEntero &X, vectorEntero &labels, int cantMuestras, int cantPixeles){
	ifstream archivoTrain((path + trainDB).c_str());
	string linea;
	string pixel;
	archivoTrain >> linea;//titulos para descartar
	for(int i = 0;i < cantMuestras;i++){
		archivoTrain >> linea;
		stringstream ss(linea);
		getline(ss,pixel,',');
		labels[i] = atoi(pixel.c_str());
		for(int j=0;j < cantPixeles;j++){
			getline(ss,pixel,',');
			X[i][j] = atoi(pixel.c_str());
		}
	}
}

int main(int argc, char** argv){
	int metodo;
	ifstream archivoEntrada;
	ofstream archivoSalida;
	int niter, cantPixeles, cantMuestras;
	string path;
	int k_vecinos, alfa_pca, gamma_plsda, K_folds;
	inicializar(metodo,archivoEntrada,archivoSalida,niter, cantPixeles, cantMuestras,path,k_vecinos, alfa_pca, gamma_plsda, K_folds,argc,argv);
	int tamTest = cantMuestras/K_folds;
	int tamTrain = tamTest*(K_folds-1);
	matrizEntero X(cantMuestras,vectorEntero(cantPixeles,0));//Inicializo una matriz con la cantidad de muestras a tomar y la cantidad de pixeles por muestra
	vectorEntero labels(cantMuestras,0);
	armarMatrizTrain(path,X,labels,cantMuestras,cantPixeles);

	/*matrizReal train(tamTrain,vectorReal(cantPixeles,0));
	matrizReal test(tamTest,vectorReal(cantPixeles,0));*/
	
	imprimir(labels,cout);
	imprimir(X,cout);
	//para cada fold armar train test labelstrain labelstest
	
	//calcular promedio de cada pixel de X 
	//restar a cada pixel de X el promedio
	
	
	
/*		if (archivoConfig) {
		string path;
		vectorEntero labels(cantMuestras, 0);
		int k, alfa, gamma, K;
		leerConfiguracion(archivoConfig, path, k, alfa, gamma, K);
		matrizEntero M = armarMatriz(path + trainDB, cantMuestras, cantPixeles, labels);
		int test = cantMuestras / K;
		int train = test * (K - 1);
		//			matriz X(train, vectorD());
		//			matriz Test(test, vectorD());
		for (int i = 0; i < K; i++) {
			matrizReal X, Test;
			vectorEntero Y, YTest;
			matrizReal XparaRomper;
			cout << "armando " << i << endl;
			armarTrainTestYLabels(archivoConfig, M, labels, X, Y, Test, YTest, XparaRomper);
			vectorReal promedio(cantPixeles, 0);
			centrarEnMedia(XparaRomper);
			//Ahora X está restado el promedio y dividido por raiz cuadrada de n-1
			matrizReal covs;
			vectorReal YparaPLSDA(Y.begin(), Y.end());
			matrizEntero confusion(10, vectorEntero(10, 0));
			ostringstream misc;
			if (metodo == 0) {
				misc << "KNN_confusion_" << i;
				ofstream f(misc.str().c_str());
				clasificarConKNNPelado(k, X, Y, Test, YTest, confusion);
				imprimir(confusion, f);
			} else if (metodo == 1) {
				misc << "PCA_mas_KNN_confusion_" << i;
				ofstream f(misc.str().c_str());
				covs = armarMatrizDeCovarianzas(X);
				imprimir(confusion, f);
			} else if (metodo == 2) {
				misc << "PLSDA_mas_KNN_confusion_" << i;
				ofstream f(misc.str().c_str());
				matrizReal V = PLS(XparaRomper, YparaPLSDA, gamma, niter);
				matrizReal muestratransformada = A_por_Bt(X, V);
				matrizReal testTransformada = A_por_Bt(Test, V);
				clasificarConKNNPelado(k, muestratransformada, Y, testTransformada, YTest, confusion);
				imprimir(confusion, f);
			} else {
				cout << "wtf " << metodo << endl;
			}
		}
		archivoConfig.close();
	}*/
	return 0;
}
