#!/usr/bin/python

import sys
import getopt
import subprocess
import time
import platform
import os.path
import numpy as np
np.set_printoptions(threshold=np.inf, linewidth=np.inf)

def createTestingFile(k, param, perc):
	fname = 'tests/test_'+str(k)+'_'+str(param)+'_'+str(perc)+'.in'
	with open(fname, 'wr+') as f:
		file = []
		f.write('../data/ '+str(k)+' '+str(param)+' '+str(param)+' 10\n')
		m = perc*42000/100
		n = 42000 - m
		for x in range(1,11):
		 	a = np.array([1]*n + [0]*m)
			np.random.shuffle(a)
			f.write(str(a).strip('[]')+'\n')

def crearArchivosTesting():
	for perc in xrange(0, 50, 10):
		for k in xrange(1, 10, 2):
			for param in xrange(1, 10):
				createTestingFile(k, param, perc)
			for param in xrange(11, 50, 5):
				createTestingFile(k, param, perc)

def ejecutarArchivosTesting():
	for perc in xrange(0, 50, 10):
		for k in xrange(1, 10, 2):
			for param in xrange(1, 10):
				for metodo in xrange(0,2):
				 	archivoEntrada = 'tests/test_'+str(k)+'_'+str(param)+'_'+str(perc)+'.in'
				 	archivoSalida = 'results/test_'+str(k)+'_'+str(param)+'_'+str(perc)+'.out'
				 	args = ['./tp', archivoEntrada, archivoSalida, metodo]
				 	process = subprocess.call(args)
			for param in xrange(11, 50, 5):
				for metodo in xrange(0,2):
					archivoEntrada = 'tests/test_'+str(k)+'_'+str(param)+'_'+str(perc)+'.in'
				 	archivoSalida = 'results/test_'+str(k)+'_'+str(param)+'_'+str(perc)+'.out'
				 	args = ['./tp', archivoEntrada, archivoSalida, metodo]
				 	process = subprocess.call(args)

def crearMiniTest():
	for perc in xrange(0, 20, 10):
		for k in xrange(1, 10, 2):
			for param in xrange(1, 10):
				createTestingFile(k, param, perc)

def ejecutarMiniTests():
	for perc in xrange(0, 20, 10):
		for k in xrange(1, 5, 2):
			for param in xrange(1, 5):
				for metodo in xrange(0,2):
				 	archivoEntrada = 'tests/test_'+str(k)+'_'+str(param)+'_'+str(perc)+'.in'
				 	archivoSalida = 'results/test_'+str(k)+'_'+str(param)+'_'+str(perc)+'.out'
				 	args = ['./tp', archivoEntrada, archivoSalida, metodo]
				 	process = subprocess.call(args)

def main(argv):
	crearMiniTest()
	ejecutarMiniTests()
	#crearArchivosTesting()
	#ejecutarArchivosTesting()

if __name__ == "__main__":
	main(sys.argv[1:])