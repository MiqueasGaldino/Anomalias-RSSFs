#!/bin/bash

for i in "10" "50" "100"; do

	foo="SN.node[*].Application.janela ="
	foo="${foo} ${i}"
	sed -i '29s/.*/'"${foo}"'/g' omnetpp.ini
	echo "Janela $i - Cenario 1..."
	../../bin/Castalia -c General,debugALL,csmaca -i omnetpp.ini -o analisar.txt
	
	mv Castalia-Trace.txt Janela_${i}_EE_Cenario1.txt
done

sed -i '30s/Cenario-1/Cenario-2/g' omnetpp.ini

for i in "10" "50" "100"; do

	foo="SN.node[*].Application.janela ="
	foo="${foo} ${i}"
	sed -i '29s/.*/'"${foo}"'/g' omnetpp.ini
	echo "Janela $i - Cenario 2..."
	../../bin/Castalia -c General,debugALL,csmaca -i omnetpp.ini -o analisar.txt
	
	mv Castalia-Trace.txt Janela_${i}_EE_Cenario2.txt
done


sed -i '30s/Cenario-2/Cenario-3/g' omnetpp.ini

for i in "10" "50" "100"; do

	foo="SN.node[*].Application.janela ="
	foo="${foo} ${i}"
	sed -i '29s/.*/'"${foo}"'/g' omnetpp.ini
	echo "Janela $i - Cenario 3..."
	../../bin/Castalia -c General,debugALL,csmaca -i omnetpp.ini -o analisar.txt
	
	mv Castalia-Trace.txt Janela_${i}_EE_Cenario3.txt
done


sed -i '30s/Cenario-3/Cenario-4/g' omnetpp.ini

for i in "10" "50" "100"; do

	foo="SN.node[*].Application.janela ="
	foo="${foo} ${i}"
	sed -i '29s/.*/'"${foo}"'/g' omnetpp.ini
	echo "Janela $i - Cenario 4..."
	../../bin/Castalia -c General,debugALL,csmaca -i omnetpp.ini -o analisar.txt
	
	mv Castalia-Trace.txt Janela_${i}_EE_Cenario4.txt
done
