#!/usr/bin/python

import sys
import getopt
import subprocess
import time
import platform
import os.path

def create(name):
    try:
        file=open(name,'w+')

        file.close()
    except:
            print("error occured")
            sys.exit(1)



def main(argv):
	for i in xrange(1,12):
		archivoEntrada = 'tests/test' + str(i) +'.in'
		archivoSalida = 'results/test' + str(i) +'.out'
		create(archivoSalida)
		metodo = str(2)
		args = ['./tp', archivoEntrada, archivoSalida, metodo]
		print 'comenzando test' + str(i)
		process = subprocess.call(args)
		print 'terminado test' + str(i)


if __name__ == "__main__":
	main(sys.argv[1:])