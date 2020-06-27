
from sklearn.metrics import precision_score
from sklearn.metrics import recall_score

Rotulos_geral = []
Detectados_geral = []

for i in range(1,55):
	
	if (i != 5 and i != 28):
		
		file1 = open("/home/miqueas/Cenario-1/Castalia-Trace50.txt","r") 

		leitura = file1.readlines()

		anomalias = []


		for linha in leitura:
			
			separado = linha.split(" ")
			
			x = len(separado)
			
		   
			if (x > 4):
				
				
				if (separado[x-1] == str(i)+"\n" and separado[x-4] == 'Anomalia:'):
					
					leitura = separado[x-3].split(",")
				
					anomalias.append(int(leitura[0]))
					
					
					

		file1.close()

		
		file2 = open("/home/miqueas/Cenario-1/No_"+ str(i) +".txt","r") 

		leitura2 = file2.readlines()

		rotulos = []


		for linha2 in leitura2:
			
			separado2 = linha2.split(",")
			
			x = len(separado2)
			
			label = separado2[x-1].split("\n")
			rotulos.append(int(label[0]))

		detectado = [1]*len(rotulos)


		for j in anomalias:
			detectado[j-1] = -1
			
		for dado in rotulos:
			Rotulos_geral.append(dado)
		
		for outlier in detectado:
			Detectados_geral.append(outlier)
			

print(len(Rotulos_geral))
print(len(Detectados_geral))

precision_score(Rotulos_geral,Detectados_geral)
recall_score(Rotulos_geral,Detectados_geral)
			
		
		




