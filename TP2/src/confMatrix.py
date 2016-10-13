#!/usr/bin/python

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
from sklearn.metrics import confusion_matrix
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
        print("Normalized confusion matrix")
    else:
        print('Confusion matrix, without normalization')

    print(cm)

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
	return confusion_matrix(y_true,y_pred,labels)

def readParams(filename):
	params = ''
	with open(filename) as f:
		params = f.readline().split()
	return params

def graficar(metodo):
	for i in xrange(1,12):
		archivoParams = 'tests/test'+str(i)+'.in'
		archivoEntrada = 'results/test' + str(i) +'.out.' +metodo+ '.resultados'
		archivoSalida = 'results/test' + str(i) +'.out.' +metodo+ '.confMatrix'
		params = readParams(archivoParams)
		confMatrix = readFile(archivoEntrada)
		labels = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
		title = ''
		if metodo == 'pca':
			title = ('k: '+ params[1] + ' alfa: '+ params[2])
		else:
			title = ('k: '+ params[1] + ' gamma: '+ params[3])
		plt.figure()
		plot_confusion_matrix(confMatrix, labels, False, title)
		#plt.show()
		plt.savefig(archivoSalida +'.jpg')
		plt.close()

def main(argv):
	graficar('pca')
	graficar('plsda')


if __name__ == "__main__":
	main(sys.argv[1:])

y_true = [2, 0, 2, 2, 0, 1]
y_pred = [0, 0, 2, 2, 0, 2]
confusion_matrix(y_true, y_pred)