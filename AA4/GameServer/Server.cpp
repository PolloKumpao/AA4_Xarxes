#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <PlayerInfo.h>
#include <map>
#include<SFML\System.hpp>
#include <thread>

enum  comandos { HELLO, WELCOME, CHALLENGE, CHALLENGE_R, DISCONECT, ACK_DISCONECT};

sf::Packet comandSend;
sf::Packet comandReceive;
sf::IpAddress ipAddr;
unsigned short port;
bool conected = true;
int nCliente = 0;
int intComandoSend;
int intComandoReceive;

int challenge;
int challenge_R;
int id;
std::map<int, sf::IpAddress> mapClientes; // Adress-> ClientProxy
std::map<int, sf::IpAddress>::iterator it;

struct ClientInfo
{
	unsigned short port;
	sf::IpAddress ipAddres;
	int clientSalt;
	int serverSalt;

	ClientInfo()
	{

	}

	ClientInfo(unsigned short _port,  sf::IpAddress _ipAddres, int _clientSalt, int _serverSalt)
	{
		port = _port;
		ipAddres = _ipAddres;
		clientSalt = _clientSalt;
		serverSalt = _serverSalt;
	}
};

std::vector<ClientInfo*> clientesNoValidados;

sf::UdpSocket* socket = new sf::UdpSocket();

bool isSaved()
{
	for (it = mapClientes.begin(); it != mapClientes.end(); ++it)
	{
		if (it->second == ipAddr)
		{
			return true;
		}
	}
	return false;
}

void update()
{

	while (1)
	{
		std::cout << "update" << std::endl;
	
		socket->receive(comandReceive, ipAddr, port);
	

	comandReceive >> intComandoReceive;
	
	ClientInfo* currentClient = new ClientInfo();

	if (intComandoReceive != comandos::HELLO)
	{
		int saltC;
		int saltS;

		bool exist = false;

		for (int i = 0; i < clientesNoValidados.size(); ++i)
		{
			if (clientesNoValidados[i]->port == port)
			{
				exist = true;
				comandReceive >> saltC >> saltS;
				if (clientesNoValidados[i]->clientSalt == saltC && clientesNoValidados[i]->serverSalt == saltS)
				{
					currentClient = clientesNoValidados[i];
				}
				else
				{
					std::cout << "Salt incorrecta" << std::endl;
					return;
				}
			}
		}
		//Falta comprobar la lista de clientes validados
		if (!exist)
			return;
		
	}


		comandos comando = static_cast<comandos>(intComandoReceive);
		switch (intComandoReceive)
		{
		case HELLO:
		{
			ClientInfo* aux = new ClientInfo();

			bool exist = false;
			for (int i = 0; i < clientesNoValidados.size(); ++i)
			{
				if (clientesNoValidados[i]->port == port)
				{
					exist = true;
					aux = clientesNoValidados[i];
				}
			}
			

			if (!exist) 
			{
				int clientSalt;
				comandSend >> clientSalt;
				int serverSalt = rand() % 9223372036854775807;
				int saltC = clientSalt;
				int saltS = serverSalt;
				std::cout << "Client salt : " << saltC << std::endl;
				std::cout << "Server salt : " << saltS << std::endl;

				ClientInfo* c = new ClientInfo(port, ipAddr, saltC, saltS);
				clientesNoValidados.push_back(c);
				comandSend << (int)comandos::CHALLENGE;
				comandSend << clientSalt << serverSalt;
				challenge = rand() % 10;
				comandSend << challenge;
				socket->send(comandSend, ipAddr, port);
				comandSend.clear();
				std::cout << "puerto" << port << std::endl;
			}
			else {

				comandSend << (int)comandos::CHALLENGE;
				comandSend << aux->clientSalt << aux->serverSalt;
				challenge = rand() % 10;
				comandSend << challenge;
				socket->send(comandSend, ipAddr, port);
			}
			

			break;
		}

		case CHALLENGE_R:
		{
			comandSend >> challenge_R;
			std::cout << "Challenge recibido" << std::endl;
			if (challenge == challenge_R)
			{
				std::cout << "Challenge correcto" << std::endl;
				comandSend.clear();
				mapClientes.insert(std::pair<int, sf::IpAddress>(nCliente, ipAddr));


				comandSend << comandos::WELCOME;
				comandSend << currentClient->clientSalt << currentClient->serverSalt;
				comandSend << nCliente;
				socket->send(comandSend, ipAddr, port);
				std::cout << "Welcome enviado" << std::endl;
				conected = true;
				update();
				nCliente++;

			}
			break;
		}
		case DISCONECT:
		{
			comandReceive >> id;
			comandSend.clear();
			comando = ACK_DISCONECT;
			intComandoSend = static_cast<int>(comando);

			comandSend << intComandoSend;
			comandSend << currentClient->clientSalt << currentClient->serverSalt;
			socket->send(comandSend, ipAddr, port);

			mapClientes.erase(id);
			std::cout << "Cliente borrado" << std::endl;
			conected = false;

			break;
		}
		}
	}
}
/*void conect()
{
	
	std::cout << "Inicio Thread" << std::endl;

	if (socket.receive(comandSend, ipAddr, port) != sf::Socket::Done)
	{
		std::cout << "No hay conexion" << std::endl;

	}
	else
	{
		if (!isSaved())
		{
			if (clientSalt == 0)
			{
				comandSend >> clientSalt;
				serverSalt = rand() % 9223372036854775807;
				saltC = clientSalt;
				saltS = serverSalt;
				std::cout << "Client salt : " << saltC << std::endl;
				std::cout << "Server salt : " << saltS << std::endl;
			
			}
		else
		{
			comandSend >> saltC >> saltS;
		}
		if (clientSalt == saltC && serverSalt == saltS)
		{
			comandSend >> intComandoSend;

			comandos comando = static_cast<comandos>(intComandoSend);
			switch (comando)
			{
			case HELLO:
				
				comandSend.clear();
				comando = CHALLENGE;
				intComandoSend = static_cast<int>(comando);
				comandSend << intComandoSend;
				comandSend << clientSalt << serverSalt;
				challenge = rand() % 10;
				comandSend << challenge;
				socket.send(comandSend, ipAddr, port);
				comandSend.clear();
				std::cout << "puerto"<<port << std::endl;

				break;
			case WELCOME:


				break;
			case CHALLENGE:


				break;
			case CHALLENGE_R:

				comandSend >> challenge_R;
				std::cout << "Challenge recibido" << std::endl;
				if (challenge == challenge_R)
				{
					std::cout << "Challenge correcto" << std::endl;
					comandSend.clear();
					mapClientes.insert(std::pair<int, sf::IpAddress>(nCliente, ipAddr));

					comando = WELCOME;
					intComandoSend = static_cast<int>(comando);
					comandSend << intComandoSend;
					comandSend << clientSalt << serverSalt;
					comandSend << nCliente;
					socket.send(comandSend, ipAddr, port);
					std::cout << "Welcome enviado" << std::endl;
					conected = true;
					update();
					nCliente++;

				}


				break;
			default:
				break;
			}


		}
		else
		{
			std::cout << "salt erroneo" << std::endl;
		}
		}
		else
		{
			std::cout << "Ya esta conectado" << std::endl;
		}
	}




}

sf::Thread conectThread(conect);

*/


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
	bool run = true;
//	std::thread conectThread(conect);


	
	
	
	//Vinculamos este socket al puerto 50000
	//Los que envíen datos deben conocer la ip y el puerto.
	sf::Socket::Status status = socket->bind(50000);
	if (status != sf::Socket::Done)
	{
		//No se puede vincular al puerto 50000
		std::cout << "No puedo vincularme al puerto 50000" << std::endl;
	}
	else
	{
		std::cout << "Puedo vincularme al puerto 50000" << std::endl;

	}
	
	
	
	while (run)
	{
		//conectThread.detach();
	
	//	conectThread.launch();

		update();
		
	}
	

	
	return 0;
}

