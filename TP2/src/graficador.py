import sys
from sklearn import svm
from sklearn.metrics import confusion_matrix, accuracy_score
from sklearn.metrics import precision_recall_fscore_support as score
import matplotlib.pyplot as plt
import numpy as np
import itertools


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
    print tiempos_K_param.keys()
    print accuracy_K_param.keys()
    print tiempos_accuracy.keys()
    print tiempos_param.keys()
    print accuracy_param_fold.keys()

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
    plt.title('Confusion matrix')
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

def main(argv):
    #recorrer_archivos()
    plotConfMatrix(3,10,'plsda')


if __name__ == "__main__":
    main(sys.argv[1:])