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
#include "pca.h"

using namespace std;
extern const string trainDB = "train.csv";
extern const string testDB = "test.csv";
extern const string config = "config.tp";

void leerConfiguracionGlobal(int &niter, int &cantPixeles, int &cantMuestras, int &cantTests,bool &ejecutarFold, bool &ejecutarCompleto){
	ifstream archivoConfig(config.c_str());
	if(archivoConfig.fail()){
		niter = 1000;
		cantPixeles=784;
		cantMuestras=42000;
		cantTests=28000;
		ejecutarFold=true;
		ejecutarCompleto=true;
	}else{
		archivoConfig >> niter;
		archivoConfig >> cantPixeles;
		archivoConfig >> cantMuestras;
		archivoConfig >> cantTests;
		archivoConfig >> ejecutarFold;
		archivoConfig >> ejecutarCompleto;
		cout << endl
			<< "----------------------------------------------------------------------------------------------------------------------" << endl
			<< "|Se utilizaran los valores de prueba del archivo \"config.tp\". Elimine dicho archivo para tomar los parámetros totales"<< endl
			<< "----------------------------------------------------------------------------------------------------------------------" << endl
			<< "|niter: " << niter << endl
			<< "|cantPixeles: " << cantPixeles << endl
			<< "|cantMuestras: " << cantMuestras << endl
			<< "|cantTests: " << cantTests << endl
			<< "|ejecutarFold: " << (ejecutarFold?"true":"false")<<endl
			<< "|ejecutarCompleto: " << (ejecutarCompleto?"true":"false")<<endl
			<< "------------------------" << endl;
			if(!ejecutarFold&&!ejecutarCompleto){
				cout << endl << "Error en \"config.tp\" no se ejecutará nada.";
			}
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

void inicializar(int &metodo,ifstream &archivoEntrada,ofstream &archivoSalida,ofstream &archivoCosas, int &niter, int &cantPixeles, int &cantMuestras,int &cantTests,bool &ejecutarFold, bool &ejecutarCompleto, string &path, int &k_vecinos, int &alfa_pca, int &gamma_plsda, int &K_folds,int argc, char** argv){
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
	leerConfiguracionGlobal(niter, cantPixeles, cantMuestras, cantTests, ejecutarFold,ejecutarCompleto);
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

matrizReal copiar(matrizReal &A){
	unsigned int m= A.size();
	unsigned int n= A[0].size();
	matrizReal B(m,vectorReal(n,0));
	for(unsigned int i =0;i<m;i++){
		for(unsigned int j =0;j<n;j++){
			B[i][j]=A[i][j];
		}
	}
	return B;
}

int main(int argc, char** argv){
	int metodo;
	ifstream archivoEntrada;
	ofstream archivoSalida,archivoCosas,archivoKaggle;
	int niter, cantPixeles, cantMuestras, cantTests;
	string path;
	int k_vecinos, alfa_pca, gamma_plsda, K_folds;
	double epsilon = 0.1e-15;
	bool ejecutarFold,ejecutarCompleto;
	
	inicializar(metodo,archivoEntrada,archivoSalida,archivoCosas,niter, cantPixeles, cantMuestras, cantTests, ejecutarFold,ejecutarCompleto, path,k_vecinos, alfa_pca, gamma_plsda, K_folds,argc,argv);
	matrizEntero Muestras(cantMuestras,vectorEntero(cantPixeles,0));//Inicializo una matriz con la cantidad de muestras a tomar y la cantidad de pixeles por muestra
	vectorEntero labels(cantMuestras,0);
	armarMatrizTrain(path,Muestras,labels,cantMuestras,cantPixeles);

	matrizReal train,test,V,W,X,Y,Z;
	vectorEntero trainLabels,testLabels;
	vectorReal medias, autovalores;
	vectorEntero indices;
	vectorReal distancias;
	
	
	if(ejecutarFold){
		for(int i_fold=0;i_fold<K_folds;i_fold++){
			indices.clear();
			distancias.clear();
			//Clasificar todos los que vienen en test, habiendo entrenado con los que vienen en train, y comparar con el label correspondiente
			//para cada fold armar train,test,trainLabels,testLabels
			armarFold(archivoEntrada,Muestras,labels,train,test,trainLabels,testLabels);
			cout << "fold " << (i_fold+1) << " de " << K_folds << endl << "train: " <<  train.size() << endl << "test: " << test.size() << endl;
			
			//preprocesar con PCA -> Escribir los alfa autovalores en salida
			//pca(train,alfa_pca,V,niter,epsilon);
			//autovalores = PCA.autovalores();
			//V = PCA.autovectores();
			//imprimir(autovalores, archivoSalida,true);
			//etiquetar con PCA -> Preparar vector para matriz de confusion
			//.... aca hay que etiquetar
			
			medias = preprocesarTrain(train);
			preprocesarTest(test,medias);
			X=copiar(train);//centrado en la media y dividido raiz de n-1
			armarY(trainLabels, Y, 10);
			autovalores = plsda(train,Y,gamma_plsda,V, niter, epsilon);//¡¡¡OJO!!! ROMPE TRAIN
			imprimir(autovalores, archivoSalida,true);
			W = A_por_Bt(X,V);
			Z = A_por_Bt(test,V);
			for(unsigned int i =0; i < Z.size();i++){
				buscar(k_vecinos,W,Z[i],indices,distancias);
				archivoCosas << testLabels[i] << " "<< votar(10,trainLabels,indices,distancias) << endl;
			}
			indices.clear();
			distancias.clear();
		}
	}
	if(ejecutarCompleto){
		armarTrainTestPosta(path, Muestras,train,test,cantMuestras,cantTests,cantPixeles);
		switch(metodo){
			case 1:{//kNN
			
				archivoKaggle.open("kaggleKnn.out");
				archivoKaggle << "ImageId" << ',' << "Label" << endl;
				for(unsigned int i =0; i < test.size();i++){
					buscar(k_vecinos,train,test[i],indices,distancias);
					archivoKaggle << i +1 << ',' <<  votar(10,labels,indices,distancias)<<endl;
				}
				break;
			}
		case 2://PCA+kNN
			{
				archivoKaggle.open("kagglePCA.out");
				PCA metodoPCA = PCA(train, labels, alfa_pca, k_vecinos, niter, epsilon);
				archivoKaggle << "ImageId" << ',' << "Label" << endl;
				for(unsigned int i =0; i < test.size();i++){
					cout << "clasificando imagen " << i << endl;
					archivoKaggle << i +1 << ',' <<  metodoPCA.clasificar(test[i]) << endl;
				}
				break;
			}
			case 3:{//PLS-DA
				archivoKaggle.open("kaglePLSDA.out");
				indices.clear();
				distancias.clear();
				V.clear();
				W.clear();
				X.clear();
				Y.clear();
				Z.clear();
				medias.clear();
				medias = preprocesarTrain(train);
				preprocesarTest(test,medias);
				X=copiar(train);//centrado en la media y dividido raiz de n-1
				armarY(labels, Y, 10);
				autovalores = plsda(X,Y,gamma_plsda,V, niter, epsilon);//¡¡¡OJO!!! ROMPE TRAIN
				W = A_por_Bt(train,V);
				Z = A_por_Bt(test,V);
				for(unsigned int i =0; i < Z.size();i++){
					buscar(k_vecinos,W,Z[i],indices,distancias);
					archivoKaggle << votar(10,labels,indices,distancias)<<endl;
				}
				break;
			}
		}
	}
	return 0;
}
