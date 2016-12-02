import sys
import matplotlib.pyplot as plt
import itertools
import numpy as np
from sklearn.metrics import confusion_matrix, accuracy_score
from sklearn.metrics import precision_recall_fscore_support as score
#def obtener_datos():
#
	#tiempos_K_param[(k,param)] = (tiempo_PCA, tiempo_PLSDA)
	#accuracy_K_param[(k,param)] = (accuracy_PCA, accuracy_PLSDA)
	#tiempos_accuracy[accuracy] = (tiempo_PCA, tiempo_PLSDA)
	#tiempos_param[param] = (tiempo_PCA, tiempo_PLSDA)
	#accuracy_param_fold[(param,fold)] = (accuracy_PCA, accuracy_PLSDA)

def recorrer_archivos():
	tiempos_K_param = {}
	accuracy_K_param = {}
	tiempos_accuracy = {}
	tiempos_param = {}
	accuracy_param_fold = {}
	ks = [3, 5, 7, 9]
	params = [10, 20, 30, 40, 50, 60, 70, 80, 90]
	folds = [10]
	for k in ks:
		for param in params:
			for fold in folds:
					#[(tiempo_PCA, tiempo_PLSDA), (accuracy_PCA, accuracy_PLSDA)] = leer(k,param,fold)
					tupla = leer(k,param,fold)
					tiempos_K_param[(k,param)] = tupla[0]
					accuracy_K_param[(k,param)] = tupla[1]
					accuracy_param_fold[(param,fold)] = tupla[1]
					tiempos_accuracy[tupla[1]] = tupla[0]
					tiempos_param[param] = tupla[0]

	graficar_matrices_k_param(ks, params, tiempos_K_param, accuracy_K_param)
	#print tiempos_K_param.keys()
	#print accuracy_K_param.keys()
	#print tiempos_accuracy.keys()
	#print tiempos_param.keys()
	#print accuracy_param_fold.keys()

def graficar_matrices_k_param(ks, params, tiempos_K_param, accuracy_K_param):
	tiempos_PCA = np.zeros((len(ks),len(params)))
	tiempos_PLSDA = np.zeros((len(ks),len(params)))
	accuracies_PCA = np.zeros((len(ks),len(params)))
	accuracies_PLSDA = np.zeros((len(ks),len(params)))
	for i in range(len(ks)):
		for j in range(len(params)):
			(tiempo_PCA, tiempo_PLSDA) = tiempos_K_param.get((ks[i],params[j]))
			(accuracy_PCA, accuracy_PLSDA) = accuracy_K_param.get((ks[i],params[j]))
			tiempos_PCA[i][j] = tiempo_PCA
			tiempos_PLSDA[i][j] = tiempo_PLSDA
			accuracies_PCA[i][j] = accuracy_PCA
			accuracies_PLSDA[i][j] = accuracy_PLSDA
	matrices = [('Tiempos PCA', tiempos_PCA), ('Accuracies PCA', accuracies_PCA), ('Tiempos PLSDA', tiempos_PLSDA), ('Accuracies PLSDA', accuracies_PLSDA)]
	for (title, matriz) in matrices:
		print matriz
		thresh = matriz.max() / 2.
		for i, j in itertools.product(range(matriz.shape[0]), range(matriz.shape[1])):
			plt.text(j, i, matriz[i, j],
	                 horizontalalignment="center",
	                 color="white" if matriz[i, j] > thresh else "black")

		plt.tight_layout()
		plt.imshow(matriz, interpolation='nearest', cmap=plt.cm.Blues)
		plt.title(title)
		plt.colorbar()
		tick_marks = np.arange(len(params))
		plt.xticks(tick_marks, params, rotation=45)
		tick_marks = np.arange(len(ks))
		plt.yticks(tick_marks, ks)
		plt.show()

def leer(k,param,fold):
	tiempo_PCA = 0
	accuracy_PCA = 0
	tiempo_PLSDA = 0
	accuracy_PLSDA = 0
	for metodo in ['pca', 'plsda']:
		y_true = []
		y_pred = []
		fresultados = 'results/test_'+str(k)+'_'+str(param)+'_'+str(fold)+'_42000_0.out.'+metodo+'.resultados'
		ftiempos = 'results/test_'+str(k)+'_'+str(param)+'_'+str(fold)+'_42000_0.out.'+metodo+'.tiempos'
		with open(fresultados) as f:
			i = 1
			for line in f:
				try:
					x, y = [int(n) for n in line.split()]
					y_true.append(x)
					y_pred.append(y)
					i+=1
				except:
					print("error occured in line " + str(i) + ' of '+fresultados)
					sys.exit(1)
		with open(ftiempos) as f:
			i = 1
			t = 0
			for line in f:
				try:
					t+=int(line)
					i+=1
				except:
					print("error occured in line " + str(i) + ' of '+ftiempos)
					sys.exit(1)
		accuracy = accuracy_score(y_true, y_pred, True)
		tiempos = t/fold
		if metodo == 'pca':
			tiempo_PCA = tiempos
			accuracy_PCA = accuracy
		else:
			tiempo_PLSDA = tiempos
			accuracy_PLSDA = accuracy
	return [(tiempo_PCA, tiempo_PLSDA), (accuracy_PCA, accuracy_PLSDA)]


def main(argv):
	recorrer_archivos()


if __name__ == "__main__":
	main(sys.argv[1:])