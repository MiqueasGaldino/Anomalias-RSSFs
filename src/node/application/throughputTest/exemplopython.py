import sys
import numpy as np
from sklearn.neighbors import LocalOutlierFactor


nsamples = int(sys.argv[1]) #quantiade de valores no arquivo




temperatura = []
umidade = []

soma = 0
for i in range(nsamples):
    v = input()
    dado = v.split(",")
    temperatura.append(float(dado[1]))
    umidade.append(float(dado[2]))
    
    
vetor = np.array((temperatura, umidade)).T.reshape(-1,2)

clf = LocalOutlierFactor(metric='manhattan')
y_pred = clf.fit_predict(vetor)

for i in y_pred:
	print(i)

    



    
    
    
#print(soma/nsamples) #coloca a media na saida
