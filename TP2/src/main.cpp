#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include "typedefs.h"
#include "imprimir.h"
#include <sstream>
#include <math.h>
#include "knn.h"
#include "plsda.h"

using namespace std;
extern const string trainDB = "train.csv";
extern const string testDB = "test.csv";
extern const string config = "config.tp";

void leerConfiguracionGlobal(int &niter, int &cantPixeles, int &cantMuestras, int &cantTests){
	ifstream archivoConfig(config.c_str());
	if(archivoConfig.fail()){
		niter = 1000;
		cantPixeles=784;
		cantMuestras=42000;
		cantTests=28000;
	}else{
		archivoConfig >> niter;
		archivoConfig >> cantPixeles;
		archivoConfig >> cantMuestras;
		archivoConfig >> cantTests;
		cout << endl
			<< "----------------------------------------------------------------------------------------------------------------------" << endl
			<< "|Se utilizaran los valores de prueba del archivo \"config.tp\". Elimine dicho archivo para tomar los parámetros totales"<< endl
			<< "----------------------------------------------------------------------------------------------------------------------" << endl
			<< "|niter: " << niter << endl
			<< "|cantPixeles: " << cantPixeles << endl
			<< "|cantMuestras: " << cantMuestras << endl
			<< "|cantTests: " << cantTests << endl
			<< "------------------------" << endl;
	}
}

void leerConfiguracionExperimento(ifstream &archivoEntrada,string &path, int &k_vecinos,int &alfa_pca, int &gamma_plsda, int &K_folds){
	archivoEntrada >> path;
	archivoEntrada >> k_vecinos;
	archivoEntrada >> alfa_pca;
	archivoEntrada >> gamma_plsda;
	archivoEntrada >> K_folds;
	string descarte;
	getline(archivoEntrada,descarte);
}

void inicializar(int &metodo,ifstream &archivoEntrada,ofstream &archivoSalida,ofstream &archivoCosas, int &niter, int &cantPixeles, int &cantMuestras,int &cantTests, string &path, int &k_vecinos, int &alfa_pca, int &gamma_plsda, int &K_folds,int argc, char** argv){
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
	archivoCosas.open((salida+".cosas").c_str());
	if(archivoSalida.fail()){
		cout << "Falla Archivo de cosas" << endl;
		exit(0);
	}
	leerConfiguracionGlobal(niter, cantPixeles, cantMuestras, cantTests);
	leerConfiguracionExperimento(archivoEntrada, path, k_vecinos, alfa_pca, gamma_plsda, K_folds);
	//cout << "entrada: " << entrada << endl << "salida: " << salida << endl << "metodo: "<< metodo << endl;
	//cout << "path: " << path << endl << "k_vecinos: " << k_vecinos << endl <<  "alfa_pca: " << alfa_pca << endl << "gamma_plsda: " << gamma_plsda << endl << "K_folds: " << K_folds << endl;
}

void armarMatrizTrain(string path,matrizEntero &X, vectorEntero &labels, int cantMuestras, int cantPixeles){
	ifstream archivoTrain((path + trainDB).c_str());
	if(archivoTrain.fail()){
		cout << "No se encontró el archivo " << path+trainDB << endl;
		exit(0);
	}
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

void armarFold(ifstream &archivoEntrada, matrizEntero &X, vectorEntero &labels, matrizReal &train, matrizReal &test, vectorEntero &trainLabels, vectorEntero &testLabels){
	train.clear();
	test.clear();
	trainLabels.clear();
	testLabels.clear();
	string linea;
	getline(archivoEntrada,linea,'\n');
	stringstream ss(linea);
	unsigned int cantMuestras = X.size();
	int traintest;
	for(unsigned int i = 0;i< cantMuestras;i++){
		ss >> traintest;
		vectorReal digito(X[i].begin(), X[i].end());
		if(traintest==0){
			testLabels.push_back(labels[i]);
			test.push_back(digito);
		}else{
			trainLabels.push_back(labels[i]);
			train.push_back(digito);
		}
	}
}

void armarMatrizTest(string path,matrizReal &X,int cantTests,int cantPixeles){
	X = matrizReal(cantTests,vectorReal(cantPixeles,0));
	ifstream archivoTest((path + testDB).c_str());
	if(archivoTest.fail()){
		cout << "No se encontró el archivo " << path+testDB << endl;
		exit(0);
	}
	string linea;
	string pixel;
	archivoTest >> linea;//titulos para descartar
	for(int i = 0;i < cantTests;i++){
		archivoTest >> linea;
		stringstream ss(linea);
		for(int j=0;j < cantPixeles;j++){
			getline(ss,pixel,',');
			X[i][j] = atoi(pixel.c_str());
		}
	}
}

void armarTrainTestPosta(string path,matrizEntero &X, matrizReal &train, matrizReal &test, int cantMuestras,int cantTests,int cantPixeles){
	train.clear();
	test.clear();
	for(int i = 0;i< cantMuestras;i++){
		vectorReal digito(X[i].begin(), X[i].end());
		train.push_back(digito);
	}
	armarMatrizTest(path,test,cantTests,cantPixeles);
}

vectorReal preprocesarTrain(matrizReal &train){
	vectorReal medias = centrarRespectoALaMedia(train);
	unsigned int m= train.size();
	unsigned int n= train[0].size();
	double raiz = sqrt(m-1);
	for(unsigned int i = 0; i < m;i++){
		for(unsigned int j = 0; j < n;j++){
			train[i][j] = train[i][j]/raiz;
		}
	}
	return medias;
}

void preprocesarTest(matrizReal &test,vectorReal &medias){
	unsigned int m= test.size();
	unsigned int n= test[0].size();
	double raiz = sqrt(m-1);
	for(unsigned int i = 0; i < m;i++){
		for(unsigned int j = 0; j < n;j++){
			test[i][j] = (test[i][j]-medias[j])/raiz;
		}
	}
}

int main(int argc, char** argv){
	int metodo;
	ifstream archivoEntrada;
	ofstream archivoSalida,archivoCosas,archivoKaggle;
	int niter, cantPixeles, cantMuestras, cantTests;
	string path;
	int k_vecinos, alfa_pca, gamma_plsda, K_folds;
	double epsilon = 0.1e-15;
	
	inicializar(metodo,archivoEntrada,archivoSalida,archivoCosas,niter, cantPixeles, cantMuestras, cantTests, path,k_vecinos, alfa_pca, gamma_plsda, K_folds,argc,argv);
	matrizEntero X(cantMuestras,vectorEntero(cantPixeles,0));//Inicializo una matriz con la cantidad de muestras a tomar y la cantidad de pixeles por muestra
	vectorEntero labels(cantMuestras,0);
	armarMatrizTrain(path,X,labels,cantMuestras,cantPixeles);

	matrizReal train,test,Y,V;
	vectorEntero trainLabels,testLabels;
	vectorReal medias, autovalores;
	
	
	for(int i_fold=0;i_fold<K_folds;i_fold++){
		//para cada fold armar train,test,trainLabels,testLabels
		armarFold(archivoEntrada,X,labels,train,test,trainLabels,testLabels);
		cout << "fold " << (i_fold+1) << " de " << K_folds << endl << "train: " <<  train.size() << endl << "test: " << test.size() << endl;
		medias = preprocesarTrain(train);//sirve para PCA Y PLSDA
		preprocesarTest(test,medias);
		
		//preprocesar con PCA -> Escribir los alfa autovalores en salida
		//autovalores = pca(train,alfa_pca,V,niter,epsilon);
		//imprimir(autovalores, archivoSalida,true);
		//etiquetar con PCA -> Preparar vector para matriz de confusion
		//.... aca hay que etiquetar
		
		//preprocesar con PLS-DA -> Escribir los gamma autovalores en salida
		armarY(trainLabels, Y, 10);
		autovalores = plsda(train,Y,gamma_plsda,V, niter, epsilon);
        imprimir(autovalores, archivoSalida,true);
		//etiquetar con PCA -> Preparar vector para matriz de confusion
		//.... aca hay que etiquetar
	}
	
	armarTrainTestPosta(path, X,train,test,cantMuestras,cantTests,cantPixeles);
	medias = preprocesarTrain(train);
	preprocesarTest(test,medias);
	
	exit(0);
	vectorEntero indices;
	vectorReal distancias;
	switch(metodo){
		case 1://kNN
			archivoKaggle.open("kaggleKnn.out");
			for(unsigned int i =0; i < test.size();i++){
				buscar(k_vecinos,train,test[i],indices,distancias);
				archivoKaggle << votar(10,labels,indices,distancias)<<endl;
			}
			break;
		case 2://PCA+kNN
			archivoKaggle.open("kagglePCA.out");
			for(unsigned int i =0; i < test.size();i++){
				buscar(k_vecinos,train,test[i],indices,distancias);
				archivoKaggle << votar(10,labels,indices,distancias)<<endl;
			}
			break;
		case 3://PLS-DA
			archivoKaggle.open("kaglePLSDA.out");
			armarY(labels, Y, 10);
			autovalores = plsda(train,Y,gamma_plsda,V, niter, epsilon);
			//armar la matriz convertida en train
			//convertir test de la misma manera que train
			for(unsigned int i =0; i < test.size();i++){
				buscar(k_vecinos,train,test[i],indices,distancias);
				archivoKaggle << votar(10,labels,indices,distancias)<<endl;
			}
			break;
	}
	
	return 0;
}
