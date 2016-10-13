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
	print '*-- PCA --*'
	print analizar('pca')
	print '*-- PLSDA --*'
	print analizar('plsda')


if __name__ == "__main__":
	main(sys.argv[1:])

y_true = [2, 0, 2, 2, 0, 1]
y_pred = [0, 0, 2, 2, 0, 2]
confusion_matrix(y_true, y_pred)