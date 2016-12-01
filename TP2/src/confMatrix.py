#!/usr/bin/python

from __future__ import division
import sys
import getopt
import platform
import itertools
import os.path
import matplotlib.pyplot as plt
import numpy as np
from numpy import ravel, reshape, swapaxes
import scipy.io
from sklearn import svm
from sklearn.metrics import confusion_matrix, accuracy_score
from sklearn.metrics import precision_recall_fscore_support as score
from random import sample

def plot_confusion_matrix(cm, classes,
                          normalize=False,
                          title='Confusion matrix',
                          cmap=plt.cm.Blues):
    plt.imshow(cm, interpolation='nearest', cmap=cmap)
    plt.title(title)
    plt.colorbar()
    tick_marks = np.arange(len(classes))
    plt.xticks(tick_marks, classes, rotation=45)
    plt.yticks(tick_marks, classes)

    if normalize:
        cm = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]
        #print("Normalized confusion matrix")
    #else:
        #print('Confusion matrix, without normalization')

    #print(cm)

    thresh = cm.max() / 2.
    for i, j in itertools.product(range(cm.shape[0]), range(cm.shape[1])):
        plt.text(j, i, cm[i, j],
                 horizontalalignment="center",
                 color="white" if cm[i, j] > thresh else "black")

    plt.tight_layout()
    plt.ylabel('True label')
    plt.xlabel('Predicted label')

def readFile(filename):
    y_true = []
    y_pred = []
    labels = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    with open(filename) as f:
        i = 0
        for line in f:
            try:
                x, y = [int(n) for n in line.split()]
                y_true.append(x)
                y_pred.append(y)
            except:
                print("error occured in line " + str(i))
                sys.exit(1)
    accuracy = accuracy_score(y_true, y_pred, True)
    confMatrix = confusion_matrix(y_true,y_pred,labels)
    return accuracy, confMatrix

def metrics(filename):
    y_true = []
    y_pred = []
    labels = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    with open(filename) as f:
        i = 0
        for line in f:
            try:
                x, y = [int(n) for n in line.split()]
                y_true.append(x)
                y_pred.append(y)
            except:
                print("error occured in line " + str(i))
                sys.exit(1)
    accuracy = accuracy_score(y_true, y_pred, True)
    precision, recall, fscore, support = score(y_true, y_pred)
    return accuracy, (sum(precision)/len(precision)), (sum(recall)/len(recall)), (sum(fscore)/len(fscore))

def readParams(filename):
    params = ''
    with open(filename) as f:
        params = f.readline().split()
    return params

def promediarTiempos(filename):
    prom = 0
    with open(filename) as f:
        for line in f:
            prom += int(line)
    prom = prom / 10
    return prom

def plotearMetricas(metodo, parametroFijo, valor):
    archivosFiltrados = filtrarArchivos(metodo, parametroFijo, valor)
    labels = [x[0] for x in archivosFiltrados]
    archs = [x[1] for x in archivosFiltrados]
    accuracies = []
    precisions = []
    recalls = []
    fscores = []

    for archivo in archs:
        accuracy, precision, recall, fscore = metrics(archivo)
        accuracies.append(accuracy)
        precisions.append(precision)
        recalls.append(recall)
        fscores.append(fscore)

    print labels
    print accuracy
    print precision
    print recall
    print fscore

    plt.figure()
    plt.gca().set_color_cycle(['red', 'green', 'blue', 'black'])

    if (metodo == 'pca'):
        if (parametroFijo == 'k'):
            plt.xlabel('alfa')
        else:
            plt.xlabel('k')
        plt.ylabel('Percentage')
        plt.title('Metricas para PCA con ' + parametroFijo + '='+ valor)
    else:
        if (parametroFijo == 'k'):
            plt.xlabel('gamma')
        else:
            plt.xlabel('k')
        plt.ylabel('Percentage')
        plt.title('Metricas para PLS-DA con ' + parametroFijo + '='+ valor)

    plt.plot(labels,accuracies) # plotting t,a separately
    plt.plot(labels,precisions) # plotting t,b separately
    plt.plot(labels,recalls) # plotting t,c separately
    plt.plot(labels,fscores)
    plt.legend(['Accuracy', 'Precision', 'Recall', 'FScore'], loc='best')
    plt.savefig('metricas_'+metodo+'_'+parametroFijo+valor+'.jpg')
    plt.close()

def plotearTiempos(metodo, parametroFijo, valor):
    archivosFiltrados = filtrarArchivosDeTiempo(metodo, parametroFijo, valor)
    labels = [x[0] for x in archivosFiltrados]
    archs = [x[1] for x in archivosFiltrados]

    times = []
    for archivo in archs:
        times.append(promediarTiempos(archivo))

    plt.figure()

    if (metodo == 'pca'):
        if (parametroFijo == 'k'):
            plt.xlabel('alfa')
        else:
            plt.xlabel('k')
        plt.ylabel('Ticks')
        plt.title('Metricas para PCA con ' + parametroFijo + '='+ valor)
    else:
        if (parametroFijo == 'k'):
            plt.xlabel('gamma')
        else:
            plt.xlabel('k')
        plt.ylabel('Ticks')
        plt.title('Metricas para PLS-DA con ' + parametroFijo + '='+ valor)

    plt.plot(labels,times)
    plt.savefig('tiempos_'+metodo+'_'+parametroFijo+valor+'.jpg')
    plt.close()

def filtrarArchivosDeTiempo(metodo, parametroFijo, valor):
    archivos = []
    for i in xrange(1,12):
        archivoParams = 'tests/test'+str(i)+'.in'
        archivoTiempos = 'results/test' + str(i) +'.out.' +metodo+ '.tiempos'
        params = readParams(archivoParams)
        if (parametroFijo == "alfa" and valor == params[2]):
            archivos.append([params[1],archivoTiempos])
        elif (parametroFijo == "gamma" and valor == params[3]):
            archivos.append([params[1],archivoTiempos])
        elif (parametroFijo == "k" and valor == params[1]):
            if (metodo == 'pca'):
                archivos.append([params[2],archivoTiempos])
            else:
                archivos.append([params[3],archivoTiempos])
    return archivos

def filtrarArchivos(metodo, parametroFijo, valor):
    archivos = []
    for i in xrange(1,12):
        archivoParams = 'tests/test'+str(i)+'.in'
        archivoEntrada = 'results/test' + str(i) +'.out.' +metodo+ '.resultados'
        params = readParams(archivoParams)
        if (parametroFijo == "alfa" and valor == params[2]):
            archivos.append([params[1],archivoEntrada])
        elif (parametroFijo == "gamma" and valor == params[3]):
            archivos.append([params[1],archivoEntrada])
        elif (parametroFijo == "k" and valor == params[1]):
            if (metodo == 'pca'):
                archivos.append([params[2],archivoEntrada])
            else:
                archivos.append([params[3],archivoEntrada])
    return archivos



def analizar(metodo):
    bestAccuracy = 0
    bestAccuracyTime = 0
    bestAccuracyParams = ''
    bestTime = 9999999999999999
    bestTimeAccuracy = 0
    bestTimeParams = ''
    bestRelation = 0
    bestRelationAccuracy = 0
    bestRelationTime = 0
    bestRelationParams = ''
    for i in xrange(1,12):
        archivoParams = 'tests/test'+str(i)+'.in'
        archivoEntrada = 'results/test' + str(i) +'.out.' +metodo+ '.resultados'
        archivoTiempos = 'results/test' + str(i) +'.out.' +metodo+ '.tiempos'
        archivoSalida = 'results/test' + str(i) +'.out.' +metodo+ '.confMatrix'
        params = readParams(archivoParams)
        accuracy, confMatrix = readFile(archivoEntrada)
        labels = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
        title = ''
        cantTicks = promediarTiempos(archivoTiempos)
        relation = accuracy / cantTicks
        if metodo == 'pca':
            title = ('k: '+ params[1] + ' alfa: '+ params[2] + '\n accuracy = '+str(accuracy)+' cantidad de ticks promedio: ' + str(cantTicks))
        else:
            title = ('k: '+ params[1] + ' gamma: '+ params[3] + '\n accuracy = '+str(accuracy)+' cantidad de ticks promedio: ' + str(cantTicks))
        plt.figure()
        plot_confusion_matrix(confMatrix, labels, False, title)
        #plt.show()
        plt.savefig(archivoSalida +'.jpg')
        plt.close()
        if metodo == 'pca':
            paramsSTR = 'k: '+ params[1] + ' alfa: '+ params[2]
        else:
            paramsSTR = 'k: '+ params[1] + ' gamma: '+ params[3]
        if accuracy > bestAccuracy :
            bestAccuracy = accuracy
            bestAccuracyTime = cantTicks
            bestAccuracyParams = paramsSTR
        if cantTicks < bestTime :
            bestTime = cantTicks
            bestTimeAccuracy = accuracy
            bestTimeParams = paramsSTR
        if relation > bestRelation :
            bestRelation = relation
            bestRelationAccuracy = accuracy
            bestRelationTime = cantTicks
            bestRelationParams = paramsSTR
    return '*-- Best Params --*' + '\n' + 'Best Accuracy score:' + '\n' + 'Accuracy: ' +str(bestAccuracy) + ' Ticks: ' + str(bestAccuracyTime) + '\nParams: \n' + bestAccuracyParams + '\n' + 'Lowest Ticks score:' + '\n' + 'Accuracy: ' +str(bestTimeAccuracy) + ' Ticks: ' + str(bestTime) + '\nParams: \n' + bestTimeParams + '\n' + 'Best Accuracy / Ticks score:' + '\nBest Relation: ' + str(bestRelation) + ' \nAccuracy: ' +str(bestRelationAccuracy) + ' Ticks: ' + str(bestRelationTime) + '\nParams: \n' + bestRelationParams + '\n'


def main(argv):
    #$\gamma = 5$, $k = 3$
    #$\alpha = 50$, $k = 3$
    plotearMetricas('plsda', 'k', '3')
    # plotearMetricas('plsda', 'gamma', '40')
    #plotearMetricas('pca', 'k', '3')
    # plotearMetricas('pca', 'alfa', '50')
    # plotearMetricas('pca', 'alfa', '100')
    #plotearTiempos('pca', 'alfa', '150')
    #plotearTiempos('plsda', 'gamma', '30')
    #plotearTiempos('pca', 'k', '10')
    #plotearTiempos('plsda', 'k', '10')
    # print '*-- PCA --*'
    # print analizar('pca')
    # print '*-- PLSDA --*'
    #print analizar('plsda')


if __name__ == "__main__":
    main(sys.argv[1:])

# y_true = [2, 0, 2, 2, 0, 1]
# y_pred = [0, 0, 2, 2, 0, 2]
# confusion_matrix(y_true, y_pred)
