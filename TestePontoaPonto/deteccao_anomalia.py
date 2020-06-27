import sys
import numpy as np
import pandas as pd
from sklearn.neighbors import LocalOutlierFactor
from sklearn import svm
from sklearn.covariance import EllipticEnvelope

nsamples = int(sys.argv[1]) #quantiade de valores no arquivo




temperatura = []
umidade = []
leitura = [] 
no = []

for i in range(nsamples):
    v = input()
    dado = v.split(",")
    
    leitura.append(int(dado[0]))
    temperatura.append(float(dado[1]))
    umidade.append(float(dado[2]))
    no.append(int(dado[3]))
    
    
vetor = np.array((temperatura, umidade)).T.reshape(-1,2)



#clf=svm.OneClassSVM(nu=.2,kernel="rbf",gamma=.001)
#clf.fit(vetor)
#y_pred=clf.predict(vetor)

clf = EllipticEnvelope(contamination=.1,random_state=0)
clf.fit(x)
ee_scores = pd.Series(clf.decision_function(x)) 
y_pred = clf.predict(x)

for i in range(nsamples):
	if (y_pred[i] !=1):
		print("Anomalia: " + str(leitura[i]) + ", Nó: " + str(no[i]) )
	


    



    
    
    
#print(soma/nsamples) #coloca a media na saida
