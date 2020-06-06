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

Define_Module(ThroughputTest);

using namespace std;

void ThroughputTest::startup()
{
//	srand(time(NULL));
	packet_rate = par("packet_rate");
	recipientAddress = par("nextRecipient").stringValue();
	startupDelay = par("startupDelay");
		
	//setTimer(SWITCH_CHANNEL, slot_len); //switch the channel every 10ms
	double startupDelay_rand = 0;//rand()%10;
	
	packet_spacing = packet_rate > 0 ? 1.0 / float (packet_rate) : -1;
	dataSN = 1;
	
	int nodeId = abs(recipientAddress.compare(SELF_NETWORK_ADDRESS));
	string s = to_string(nodeId);
	
	string addr(SELF_NETWORK_ADDRESS);
    string str1 = "/home/miqueas/Dados/No_Final/No_"+ addr +".txt";
	string line;
	
	
	// leitura dos arquivos	
	
	
	int leitura, ano, mes, data, hora, minuto, epoca, no;
	double segundo;
	ifstream myfile;
	myfile.open(str1);	
	
	while (getline (myfile,line))
    {
	  sscanf(line.c_str(),"%d,%d-%d-%d,%d:%d:%lf,%d,%d, %lf, %lf", &leitura, &ano, &mes, &data, &hora, &minuto, &segundo, &epoca, &no);
      if (leitura == 1){
		   if (no != 0 && no != 5 ){// nó zero e nó 5 nao possuem dados
			   tempo_atual = (3600*hora + 60*minuto + segundo);// calcula o tempo atual do no ( aqui se obtém o tempo inicial dos dados)
			   tempo_no[no] = tempo_atual; // atualiza o tempo em que o nó está
								
								   }
															
						}
                         
      }
 	
	myfile.close();
	
				

	
	if (packet_spacing > 0) {
		setTimer(SEND_PACKET, packet_spacing + startupDelay_rand + 1.5);
	}
	else
		// Diego
		trace() << "Not sending packets";

	declareOutput("Packets received per node");
	for(int i = 0; i < 5; i++)new_value_flqe[i]=false;
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
		trace() << "Received packet " << sequenceNumber << " from node " << source << ": " << rssi << " " << lqi << " fromNetworkLayer";
		packets_received++;
	} 
	//END NODE
	else {
		
	}
	//free(data);
}

void ThroughputTest::timerFiredCallback(int index)
{
	
	switch (index){
		case SEND_PACKET: {

			string addr(SELF_NETWORK_ADDRESS);
			
			
			//is an END NODE
			if (addr.compare("0") != 0 && addr.compare("5") != 0) {
	
				string arquivo = "/home/miqueas/Dados/No_Final/No_" + addr + ".txt";
				trace() << "Sending packet (nó"<< " " << addr << ")" << dataSN;
				packets_sent++;
                FILE *file;
                file = fopen(arquivo.c_str(),"r");
                
                double *data = new double[10];
                int leitura, ano, mes, dia, hora, minuto, epoca, no;
                
                double segundo,temperatura, umidade;
                char frase[100];
				
		        while(fgets(frase,100,file) != NULL)	 { 
					
					fscanf(file,"%d,%d-%d-%d,%d:%d:%lf,%d, %d, %lf,%f", &leitura, &ano, &mes, &dia, &hora, &minuto, &segundo, &epoca, &no, &temperatura, &umidade);
                            //lê o dado do arquivo
                             if (leitura == contadorleitura[no]){
								 
								 data[0] = 0;
				                 data[1] = prr_beacon_end;
				                 data[2] = RNP;
                                 data[3] = umidade;
                                 data[4] = temperatura;
                                 data[5] = no;
                                 data[6] = leitura;
								   								
                                 DataPacket* data_packet = createDataPacket(data, dataSN, 80);
                                 toNetworkLayer(data_packet, (const char*)("0"));
                                 dataSN++;
                                 cont_tx++;
                   
																}
										
							// lê o proximo dado do arquivo
							  if (leitura == contadorleitura[no]+1){
								  proximo_tempo[no] = (3600*hora + 60*minuto + segundo); // o proximo dado do arquivo 
	
								
																	}
                             
															}

				
                fclose(file);
                
                timer = proximo_tempo[no] - tempo_no[no]; // calcula o intervalo entre pacotes entre o anterior enviado e proximo a ser enviado
                
                // if para o caso de troca de dia das 23 horas para meia noite
                
                if( timer < 0){
					double troca_dia = (24*3600);
					timer = timer + troca_dia;

					
								}
				tempo_no[no] = proximo_tempo[no]; // o tempo do nó vira o proximo tempo 
				contadorleitura[no]++;
				
				
			    }

			       
			
		 }
	
		setTimer(SEND_PACKET,timer); // o intervalo do pacote é setado
            
           //setTimer(SEND_PACKET, packet_spacing);
                    
			break;
            
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

