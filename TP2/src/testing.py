#!/usr/bin/python

import sys
import getopt
import subprocess
import time
import platform
import os.path
import numpy as np

np.set_printoptions(threshold=np.inf, linewidth=np.inf)

def createTestingFile(k_knn, param, folds_k, n):
	fname = 'tests/test_'+str(k_knn)+'_'+str(param)+'_'+str(folds_k)+'_'+str(n)+'.in'
	print 'escribiendo: ', fname
	with open(fname, 'wr+') as f:
		f.write('../data/ '+str(k_knn)+' '+str(param)+' '+str(param)+' '+str(folds_k)+'\n')
		folds = kfolds(folds_k, n)
		for fold in folds:
			f.write(str(fold).strip('[]')+'\n')

def crearArchivosTesting(ks, params, folds, n):
	for k_knn in ks:
		for param in params:
			for folds_k in folds:
				createTestingFile(k_knn, param, folds_k, n)

def ejecutarArchivosTesting(ks, params, folds, n):
	for k_knn in ks:
		for param in params:
			for folds_k in folds:
				for metodo in xrange(0,1):
					archivoEntrada = 'tests/test_'+str(k_knn)+'_'+str(param)+'_'+str(folds_k)+'_'+str(n)+'.in'
					archivoSalida = 'results/test_'+str(k_knn)+'_'+str(param)+'_'+str(folds_k)+'_'+str(n)+'_'+str(metodo)+'.out'
					args = ['./tp', archivoEntrada, archivoSalida, str(metodo)]
					print 'ejecutando: ', archivoEntrada 
					process = subprocess.call(args)

def kfolds(k, n):
	num_folds = k
	subset_size = n/num_folds
	trainings = []
	for i in range(0, k):
		training = np.array([1]*n)
		training[i*subset_size:][:subset_size] = 0
		trainings.append(training)
	return trainings

def main(argv):
	ks = [3]
	params = [40, 50, 60]
	folds = [2, 5, 7]
	n = 42000
	crearArchivosTesting(ks, params, folds, n)
	ejecutarArchivosTesting(ks, params, folds, n)


if __name__ == "__main__":
	main(sys.argv[1:])