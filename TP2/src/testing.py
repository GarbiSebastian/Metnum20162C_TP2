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

def crearArchivosTesting():
	for folds_k in xrange(2, 11, 2):
		for k_knn in xrange(3, 10, 2):
			for param in xrange(1, 10, 3):
				createTestingFile(k_knn, param, folds_k, 42000)
			for param in xrange(10, 100, 10):
				createTestingFile(k_knn, param, folds_k, 42000)

def ejecutarArchivosTesting(n):
	for folds_k in xrange(2, 11, 2):
		for k_knn in xrange(3, 10, 2):
			for param in xrange(1, 10, 3):
				for metodo in xrange(0,2):
					archivoEntrada = 'tests/test_'+str(k_knn)+'_'+str(param)+'_'+str(folds_k)+'_'+str(n)+'.in'
					archivoSalida = 'results/test_'+str(k_knn)+'_'+str(param)+'_'+str(folds_k)+'_'+str(n)+'_'+str(metodo)+'.out'
					args = ['./tp', archivoEntrada, archivoSalida, str(metodo)]
					print 'ejecutando: ', archivoEntrada 
					process = subprocess.call(args)

def crearMiniTest():
	#for perc in xrange(10, 30, 10):
	for k in xrange(3, 6, 1):
		for param in xrange(1, 3):
			createTestingFile(k, param, 10)

def ejecutarMiniTests():
	#for perc in xrange(10, 30, 10):
	for k in xrange(3, 6, 1):
		for param in xrange(1, 3):
			for metodo in xrange(0,2):
				archivoEntrada = 'tests/test_'+str(k)+'_'+str(param)+'_'+str(10)+'.in'
				archivoSalida = 'results/test_'+str(k)+'_'+str(param)+'_'+str(10)+'_'+str(metodo)+'.out'
				args = ['./tp', archivoEntrada, archivoSalida, str(metodo)]
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
	#crearMiniTest()
	#ejecutarMiniTests()
	#crearArchivosTesting()
	ejecutarArchivosTesting(42000)


if __name__ == "__main__":
	main(sys.argv[1:])