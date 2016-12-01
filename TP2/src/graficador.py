#def obtener_datos():
#
	#tiempos_K_param[(k,param)] = (tiempo_PCA, tiempo_PLSDA)
	#accuracy_K_param[(k,param)] = (accuracy_PCA, accuracy_PLSDA)
	#tiempos_accuracy[accuracy] = (tiempo_PCA, tiempo_PLSDA)
	#tiempos_param[param] = (tiempo_PCA, tiempo_PLSDA)
	#accuracy_param_fold[(param,fold)] = (accuracy_PCA, accuracy_PLSDA)

def recorrer_archivos():
	for k in ks:
		for param in params:
			for fold in folds:
					#[(tiempo_PCA, tiempo_PLSDA), (accuracy_PCA, accuracy_PLSDA)] = leer(k,param,fold)
					tupla = leer(k,param,fold)
					tiempos_K_param[(k,param)] = tupla[0]
					accuracy_K_param[(k,param)] = tupla[1]
					tiempos_accuracy[accuracy] = tupla[0]
					tiempos_param[param] = tupla[0]
					accuracy_param_fold[(param,fold)] = tupla[1]

def leer(k,param,fold):
	for metodo in ['pca', 'plsda']
		y_true = []
		y_pred = []
		fresultados = 'test_'+str(k)+'_'+str(param)+'_'+str(fold)+'_42000_0.out.'+metodo+'.resultados'
		ftiempos = 'test_'+str(k)+'_'+str(param)+'_'+str(fold)+'_42000_0.out.'+metodo+'.tiempos'
		with open(fresultados) as f:
			i = 0
			for line in f:
				try:
					x, y = [int(n) for n in line.split()]
					y_true.append(x)
					y_pred.append(y)
				except:
					print("error occured in line " + str(i) + ' of '+fresultados)
					sys.exit(1)
		with open(ftiempos) as f:
			i = 0
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