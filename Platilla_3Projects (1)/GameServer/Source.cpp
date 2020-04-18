#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <PlayerInfo.h>
#include <map>
#include<thread>
sf::Packet Hello;
std::list<sf::UdpSocket*> jugadores;
sf::IpAddress ipAddr;
unsigned short port;
int nCliente = 0;
int id;
std::map<int, sf::IpAddress> mapClientes; // Adress-> ClientProxy


sf::UdpSocket socket;
void conect()
{
	std::cout << "Inicio Thread" << std::endl;

	if (socket.receive(Hello, ipAddr, port) != sf::Socket::Done)
	{
		std::cout << "No hay conexion" << std::endl;

	}
	else
	{
		
		Hello >> id;
		std::cout << "conex" << id << std::endl;
		mapClientes.insert(std::pair<int, sf::IpAddress>(nCliente, ipAddr));
		nCliente++;
	}



}








//void ControlServidor()
//{
//	bool running = true;
//	bool inicio = false;
//	// Create a socket to listen to new connections
//	sf::TcpListener listener;
//	sf::Socket::Status status = listener.listen(50000);
//	if (status != sf::Socket::Done)
//	{
//		std::cout << "Error al abrir listener\n";
//		exit(0);
//	}
//	// Create a list to store the future clients
//
//	// Create a selector
//	sf::SocketSelector selector;
//	// Add the listener to the selector
//	selector.add(listener);
//	// Endless loop that waits for new connections
//	while (running)
//	{
//		// Make the selector wait for data on any socket
//		if (selector.wait())
//		{
//			// Test the listener
//			if (selector.isReady(listener))
//			{
//				// The listener is ready: there is a pending connection
//				sf::TcpSocket* client = new sf::TcpSocket;
//				if (listener.accept(*client) == sf::Socket::Done)
//				{
//					// Add the new client to the clients list
//				
//					std::cout <<" y con puerto: " << client->getRemotePort()<< std::endl;
//					jugadores.push_back(client);
//					// Add the new client to the selector so that we will
//					// be notified when he sends something
//					selector.add(*client);
//				}
//				else
//				{
//					// Error, we won't get a new connection, delete the socket
//					std::cout << "Error al recoger conexiÃ³n nueva\n";
//					delete client;
//				}
//			}
//			else
//			{
//				// The listener socket is not ready, test all other sockets (the clients)
//				for (std::list<sf::TcpSocket*>::iterator it = jugadores.begin(); it != jugadores.end(); ++it)
//				{
//					sf::TcpSocket& client = **it;
//					if (selector.isReady(client))
//					{
//						// The client has sent some data, we can receive it
//						sf::Packet packet;
//						status = client.receive(packet);
//						if (status == sf::Socket::Done)
//						{
//							std::string strRec;
//							packet >> strRec;
//							std::cout << "He recibido " << strRec << " del puerto " << client.getRemotePort() << std::endl;
//						}
//						else if (status == sf::Socket::Disconnected)
//						{
//							selector.remove(client);
//							std::cout << "Elimino el socket que se ha desconectado\n";
//						}
//						else
//						{
//							std::cout << "Error al recibir de " << client.getRemotePort() << std::endl;
//						}
//					}
//				}
//			}
//		}
//	}
//
//}



int main()
{
	std::thread conectThread = std::thread(conect);
	conectThread.detach();
	
	//Vinculamos este socket al puerto 50000
	//Los que envíen datos deben conocer la ip y el puerto.
	sf::Socket::Status status = socket.bind(50000);
	if (status != sf::Socket::Done)
	{
		//No se puede vincular al puerto 50000
		std::cout << "No puedo vincularme al puerto 50000" << std::endl;
	}
	else
	{
		std::cout << "Puedo vincularme al puerto 50000" << std::endl;

	}
	
	
	
	while (true)
	{
		
		conectThread;
		//conectThread.detach();
		//std::cout << "vuelta" << std::endl;
		
	}
	

	
	return 0;
}

