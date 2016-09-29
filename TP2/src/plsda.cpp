#include "plsda.h"
#include "norma.h"
#include "metodoPotencia.h"
#include "imprimir.h"
#include <assert.h>

using namespace std;


vectorReal preprocesarTrain(matrizReal &train,vectorEntero &labels, matrizReal &Y,int gamma_plsda, int cant_categorias){
	vectorReal medias = centrarRespectoALaMedia(train);
	Y = matrizReal(labels.size(),vectorReal(cant_categorias,0));
	for(unsigned int i = 0; i< labels.size();i++){
		Y[i][labels[i]]=1;
	}
	return medias;
}
	
matrizReal A_por_Bt(matrizReal &A,matrizReal &B){//La armamos de esta manera para que queden productos internos entre vectores fila(por performance)
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

matrizReal transponer(matrizReal &A){
	//A en Rm*n B en Ro*p
	unsigned int m = A.size();
	unsigned int n = A[0].size();
	matrizReal At(n,vectorReal(m,0));
	for(unsigned int i=0; i < m ; i++){
		for(unsigned int j=0; j < n ; j++){
			At[j][i]=A[i][j];
		}
	}
	return At;
}

/*	PLS(X ,Y ,γ)
	Para i = 1, . . . , gamma_plda
		definir M i como X'*Y*Y'*X  //////////// (Y'*X)'*Y'*X
		calcular w_i como el autovector asociado al mayor autovalor de M i
		normalizar w_i
		definir t_i como X*w_i
		normalizar t_i
		///////// t_i*t_i'
		actualizar X como X − t_i*t_i'*X
		actualizar Y como Y − t_i*t_i'*Y
	Fin Para
	Devolver w_i para i = 1, . . . , γ*/


vectorReal plsda(matrizReal &X,matrizReal &Y, int gamma_plsda, matrizReal &V, int niter, double epsilon){
	vectorReal autovalores(gamma_plsda,0);
	matrizReal T,S,A,Mi,U;
	vectorReal ti,wi,u;
	V.clear();
	for(int i=0;i< gamma_plsda;i++){
		T = transponer(X);// T = X'
		S = transponer(Y);// S = Y'
		A = A_por_Bt(T,S);// A = X'*Y = T*Y = A_por_Bt(T,S)
		Mi = A_por_Bt(A,A);// Mi = X'*Y*Y'*X = X'*Y*(X'*Y)' = A*A' = A_por_Bt(A,A)
		autovalores[i] = metodoPotencia(Mi, wi,niter,epsilon);//calcular wi como el autovector asociado al mayor autovalor de Mi
		normalizar(wi);//normalizar w_i
		V.push_back(wi);
		ti = A_por_v(X,wi);//definir ti como X*wi
		normalizar(ti);//normalizar ti
		//actualizar X como X − ti*ti'*X = X - ti*(X'*ti)' = X - ti*(T*ti)'
		u = A_por_v(T,ti);
		U = v_por_ut(ti,u);
		restarA(X, U);
		//actualizar Y como Y − t_i*t_i'*Y
		u = A_por_v(S,ti);
		U = v_por_ut(ti,u);
		restarA(Y, U);
		//restarA(Y, v_por_ut(ti,A_por_v(S,ti)));
		T.clear();
		S.clear();
	}
	return autovalores;
}

