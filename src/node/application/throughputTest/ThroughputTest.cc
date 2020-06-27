/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2011                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *
 ****************************************************************************/


#include "ThroughputTest.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <random>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>

Define_Module(ThroughputTest);

using namespace std;

int contador_arquivo[55] = {0};
int tamanho_janela;
vector<double>janela_temperatura[55];
vector<double>janela_umidade[55];
vector<int>janela_leitura[55];
int contadorleitura[55] = {1};
double tempo_no[55];
double tempo_atual;
double timer;
double proximo_tempo[55];





void ThroughputTest::startup()
{
   
	packet_rate = par("packet_rate");
	recipientAddress = par("nextRecipient").stringValue();
	startupDelay = par("startupDelay");
	janela = par("janela");
	cenario = par("cenario").stringValue();
	
	
	
	
	
	
	
	//setTimer(SWITCH_CHANNEL, slot_len); //switch the channel every 10ms
	double startupDelay_rand = 0;//rand()%10;
	
	packet_spacing = packet_rate > 0 ? 1.0 / float (packet_rate) : -1;
	dataSN = 1;
	string addr(SELF_NETWORK_ADDRESS);
	int id = stoi(addr);
	
	
	
	string dados_file = cenario+addr+".txt";
	
	
	
	int leitura, ano, mes, data, hora, minuto, epoca, no, rotulo, leitura_inutil;
	double segundo;
	double temperatura,umidade;
	
	ifstream file_results(dados_file.c_str());
	
	string line;
	
	
	
	if (file_results.is_open()){
		
		while ( getline (file_results,line) ) {
			 
			   sscanf(line.c_str(),"%d,%d-%d-%d,%d:%d:%lf,%d,%lf,%lf,%d", &leitura, &ano, &mes, &data, &hora, &minuto, &segundo, &no, &temperatura, &umidade, &rotulo);
			   
			   if (leitura == 1){
				    if (no != 0 && no != 5 && no != 28 ){// no zero e no 5 nao possuem dados
						
						 tempo_atual = (3600*hora + 60*minuto + segundo);// calcula o tempo atual do no ( aqui se obtém o tempo inicial dos dados)
					  
						 tempo_no[no] = tempo_atual; // atualiza o tempo em que o nó está
						 
							
					}
				}//se quiser converter para double
		
			
				
		 ++contador_arquivo[id];
				
			
        }
		
	}	
	
	
	
	file_results.close();

  

	if (packet_spacing > 0) {
	
		
		setTimer(SEND_PACKET, tempo_atual );
	}
	else
		// Diego
		trace() << "Not sending packets";

	declareOutput("Packets received per node");
	
	
	
}

void ThroughputTest::fromNetworkLayer(DataPacket * rcvPacket,
		const char *source, double rssi, double lqi)
{
	int sequenceNumber = rcvPacket->getSequenceNumber();
	double* data = rcvPacket->getDataPacket();
	

	//SINK NODE
	string addr(SELF_NETWORK_ADDRESS,1);
	if (addr.compare("0") == 0) {
	
		//data packet received
		int id = data[6];
		trace() << "Received packet " << sequenceNumber << " from node " << source << ": " << rssi << " " << lqi << " fromNetworkLayer";
		trace() << "Temperatura: " << data[3] << " "<< "Umidade " << data[4] <<" "<< "Leitura: " << data[5];
		
		

		tamanho_janela = janela;
	
		
		if (janela_temperatura[id].size() < tamanho_janela-1){
			
			janela_temperatura[id].push_back(data[3]);
			janela_umidade[id].push_back(data[4]);
		    janela_leitura[id].push_back(data[5]);
		    
		   
		    }
		    
					
		else if (janela_temperatura[id].size() == tamanho_janela-1){
			
			janela_temperatura[id].push_back(data[3]);
			janela_umidade[id].push_back(data[4]);
		    janela_leitura[id].push_back(data[5]);
		    
			string file_results_name = "arquivo_results3.txt";
		    string results_file = "final_results.txt";
			
			ofstream send_data;
			send_data.open(file_results_name);	
		    
		    for (int i = 0; i<tamanho_janela; i++){
				
				 //trace() << "Janela_Leitura: " << janela_leitura[i];
			    send_data << janela_leitura[id][i] << "," << janela_temperatura[id][i] << "," << janela_umidade[id][i] << "," << id << endl;
									
								
			}
			
			
			send_data.close();
						
			stringstream sstm;
			sstm << tamanho_janela;
									
			string command = "python3 deteccao_anomalia.py < "+file_results_name+" > "+results_file+" "+sstm.str();
			system(command.c_str()); 
			trace() << command << endl;
			
			ifstream file_results(results_file);
			string line;
			
			if (file_results.is_open()){
				//coloca todas as linhas do arquivo na saída
				while ( getline (file_results,line) ) {
					trace() << line << endl;
								 //se quiser converter para double
				}
					
			}
			
			file_results.close();
						
			janela_temperatura[id].clear();
			janela_umidade[id].clear();
			janela_leitura[id].clear();
						
		}
		
		
		 
		 if (contadorleitura[id]-1 == contador_arquivo[id]){
			
			
		    
			string file_results_name = "arquivo_results3.txt";
		    string results_file = "final_results.txt";
			
			ofstream send_data;
			send_data.open(file_results_name);	
		    int size = janela_temperatura[id].size();
		  
		   
		    for (int i = 0; i<size; i++){
				
			//	 trace() << "Janela_Leitura: " << janela_leitura[i];
			    send_data << janela_leitura[id][i] << "," << janela_temperatura[id][i] << "," << janela_umidade[id][i] << "," << id << endl;
									
								
			}
			
			
			send_data.close();
						
			stringstream sstm;
			sstm << janela_temperatura[id].size();
									
			string command = "python3 deteccao_anomalia.py < "+file_results_name+" > "+results_file+" "+sstm.str();
			system(command.c_str()); 
			trace() << command << endl;
			
			ifstream file_results(results_file);
			string line;
			
			if (file_results.is_open()){
				//coloca todas as linhas do arquivo na saída
				while ( getline (file_results,line) ) {
					trace() << line << endl;
								 //se quiser converter para double
				}
					
			}
			
			file_results.close();
						
			janela_temperatura[id].clear();
			janela_umidade[id].clear();
			janela_leitura[id].clear();
						
		}
			
				
		
		packets_received++;
	} 
	
		//END NODE
	else {
		
	}	
			

}

void ThroughputTest::timerFiredCallback(int index)
{
	
	switch (index){
		case SEND_PACKET: {
			 
			string addr(SELF_NETWORK_ADDRESS);
			int node_id = stoi(addr);
			
		
			
			//is an END NODE
			if (addr.compare("0") != 0 && addr.compare("5") != 0 && addr.compare("28") != 0 && contadorleitura[node_id] <= contador_arquivo[node_id] ) {
				
				
				int nodeId = stoi(SELF_NETWORK_ADDRESS);
				
				string dados_file =  cenario+addr+".txt";
				trace() << "Sending packet (nó"<< " " << addr << ")" << dataSN;
				
				packets_sent++;
  
				ifstream file_results(dados_file.c_str());
	
				string line;
	
               
				double *data = new double[10];
				int leitura, ano, mes, data1, hora, minuto, epoca, no, rotulo, leitura_inutil;
				double segundo, temperatura, umidade;
				
                if (file_results.is_open()){
					
					while ( getline (file_results,line) ) {
						
						sscanf(line.c_str(),"%d,%d-%d-%d,%d:%d:%lf,%d,%lf,%lf,%d", &leitura, &ano, &mes, &data1, &hora, &minuto, &segundo, &no, &temperatura, &umidade, &rotulo);

		                     if (leitura == contadorleitura[no]){
									
                                //     trace()<< "entrou aqui dentro do leitura";
                                     data[0] = 0;
				                     data[1] = prr_beacon_end;
				                     data[2] = RNP;
                                     data[3] = temperatura;
                                     data[4] = umidade;
									 data[5] = leitura;
									 data[6] = no;
									  
                                    DataPacket* data_packet = createDataPacket(data, dataSN, 80);
                               
									
									
                                    toNetworkLayer(data_packet, (const char*)("0"));
                                    dataSN++;
                                    cont_tx++;
							
								
                                    
								}
							
							if (leitura == contadorleitura[no]+1){
									
								
								  proximo_tempo[no] = (3600*hora + 60*minuto + segundo); // o proximo dado do arquivo 
								  
								   trace() << "Proximo tempo: " <<  proximo_tempo[no] ;
								
							}
                             
	                 }

				 }
					
					
				file_results.close();
					
					
				
					
			    timer = proximo_tempo[no] - tempo_no[no]; // calcula o intervalo entre pacotes entre o anterior enviado e proximo a ser enviado
                
                
                // if para o caso de troca de dia das 23 horas para meia noite
                
                if( timer < 0){
					double troca_dia = (24*3600);
					timer = timer + troca_dia;
	
				}
				
				
				tempo_no[no] = proximo_tempo[no]; // o tempo do nó vira o proximo tempo 
				contadorleitura[no]++;
				
			 }
				
			
		if (contadorleitura[node_id] <= contador_arquivo[node_id] and node_id != 28 and node_id != 5){
				 
				 
				setTimer(SEND_PACKET, timer);
			
				break;
          
            
		 }

		
		 
	}
}
}
void ThroughputTest::sendAck(int packet_id)
{
	//trace() << "Sending ack for packet " << packet_id;
    double *data = new double[10];
	data[0] = packet_id;
	DataPacket* data_packet = createDataPacket(data, ack_seq, 5);  
	toNetworkLayer(data_packet, (const char*)("1"));
	acks_sent++;
	//trace() << "Acks sent: " << acks_sent;
}


// This method processes a received carrier sense interupt. Used only for demo purposes
// in some simulations. Feel free to comment out the trace command.
void ThroughputTest::handleRadioControlMessage(RadioControlMessage *radioMsg)
{
	switch (radioMsg->getRadioControlMessageKind()) {
		case CARRIER_SENSE_INTERRUPT:
			trace() << "CS Interrupt received! current RSSI value is: " << radioModule->readRSSI();
                        break;
	}
}

