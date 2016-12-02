import sys
import matplotlib.pyplot as plt
import itertools
import numpy as np
from sklearn.metrics import confusion_matrix, accuracy_score
from sklearn.metrics import precision_recall_fscore_support as score
import matplotlib.pyplot as plt
import numpy as np
import itertools

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
	graficar_tabla_fold([40, 50, 60], folds, accuracy_param_fold)
	plot_grafico_tiempos(3, params, tiempos_K_param, accuracy_K_param)
	plot_grafico_param(3, params, tiempos_K_param, accuracy_K_param)


def plot_grafico_tiempos(k, params, tiempos_K_param, accuracy_K_param):
    pca = []
    plsda = []
    for j in range(len(params)):
        (tiempo_PCA, tiempo_PLSDA) = tiempos_K_param.get((k,params[j]))
        (accuracy_PCA, accuracy_PLSDA) = accuracy_K_param.get((k,params[j]))
        pca.append((tiempo_PCA, accuracy_PCA))
        plsda.append((tiempo_PLSDA, accuracy_PLSDA))
    pca_sorted = sorted(pca, key=lambda tup: tup[0])        
    plsda_sorted = sorted(plsda, key=lambda tup: tup[0])
    listas = [('PCA', pca_sorted), ('PLSDA', plsda_sorted)]        

    for (title, lista) in listas:
	    plt.plot([x[0] for x in lista], [x[1] for x in lista])

	    plt.xlabel('tiempo (s)')
	    plt.ylabel('Accuracy')
	    plt.title(title)
	    #plt.grid(True)
	    #plt.savefig("test.png")
	    plt.show()

def plot_grafico_param(k, params, tiempos_K_param, accuracy_K_param):
    pca = []
    plsda = []
    for j in range(len(params)):
        (tiempo_PCA, tiempo_PLSDA) = tiempos_K_param.get((k,params[j]))
        (accuracy_PCA, accuracy_PLSDA) = accuracy_K_param.get((k,params[j]))
        pca.append((params[j], accuracy_PCA))
        plsda.append((params[j], accuracy_PLSDA))
    pca_sorted = sorted(pca, key=lambda tup: tup[0])        
    plsda_sorted = sorted(plsda, key=lambda tup: tup[0])
    listas = [('PCA', pca_sorted), ('PLSDA', plsda_sorted)]        

    for (title, lista) in listas:
	    plt.plot([x[0] for x in lista], [x[1] for x in lista])

	    plt.xlabel('Param')
	    plt.ylabel('Accuracy')
	    plt.title(title)
	    #plt.grid(True)
	    #plt.savefig("test.png")
	    plt.show()


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
			accuracies_PCA[i][j] = float("{0:.4f}".format(accuracy_PCA))
			accuracies_PLSDA[i][j] = float("{0:.4f}".format(accuracy_PLSDA))
	matrices = [('Tiempos PCA', tiempos_PCA), ('Accuracies PCA', accuracies_PCA), ('Tiempos PLSDA', tiempos_PLSDA), ('Accuracies PLSDA', accuracies_PLSDA)]
	for (title, matriz) in matrices:
		print matriz
		thresh = matriz.max() / 2.
		for i, j in itertools.product(range(matriz.shape[0]), range(matriz.shape[1])):
			plt.text(j, i, matriz[i, j],
					 horizontalalignment="center",
					 color="black" if matriz[i, j] > thresh else "black")

		plt.tight_layout()
		plt.imshow(matriz, interpolation='nearest', cmap=plt.cm.Blues)
		plt.title(title)
		plt.colorbar()
		plt.ylabel('k para Knn')
		plt.xlabel('parametro para ' + title.split(' ')[1])
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

#For confusion matrix
def plotConfMatrix(k, param, metodo):
	resultados = 'results/test_'+str(k)+'_'+str(param)+'_10_42000_0.out.'+metodo+'.resultados'
	y_true = []
	y_pred = []
	labels = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
	with open(resultados) as f:
		i = 0
		for line in f:
			try:
				x, y = [int(n) for n in line.split()]
				y_true.append(x)
				y_pred.append(y)
			except:
				print("error occured in line " + str(i))
				sys.exit(1)
	confMatrix = confusion_matrix(y_true,y_pred,labels)

	plt.imshow(confMatrix, interpolation='nearest', cmap=plt.cm.Blues)
	plt.title('Confusion matrix '+metodo)
	plt.colorbar()
	tick_marks = np.arange(len(labels))
	plt.xticks(tick_marks, labels, rotation=45)
	plt.yticks(tick_marks, labels)

	#if normalize:
	#    cm = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]

	thresh = confMatrix.max() / 2.
	for i, j in itertools.product(range(confMatrix.shape[0]), range(confMatrix.shape[1])):
		plt.text(j, i, confMatrix[i, j],
				 horizontalalignment="center",
				 color="white" if confMatrix[i, j] > thresh else "black")

	plt.tight_layout()
	plt.ylabel('True label')
	plt.xlabel('Predicted label')
	plt.show()

def graficar_tabla_fold(params, folds, accuracy_param_fold):
	tabla = []
	for param in params:
		for fold in folds:
			accuracy = accuracy_param_fold[(param,fold)]
			tabla.append((fold, accuracy))
	print tabla

def main(argv):
	recorrer_archivos()
	#print("{0:.2f}".format(number))
	plotConfMatrix(3, 50, 'pca')
	plotConfMatrix(3, 50, 'plsda')


if __name__ == "__main__":
	main(sys.argv[1:])
