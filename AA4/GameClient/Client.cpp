#pragma once
#include <PlayerInfo.h>
#include <SFML\Network.hpp>
#include<thread>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include <fcntl.h>

#include "Graphics.h"


enum  comandos { HELLO, WELCOME, CHALLENGE, CHALLENGE_R, DISCONECT, ACK_DISCONECT};

sf::UdpSocket* socket = new sf::UdpSocket;
std::string prueba = "Prueba conexion";
sf::IpAddress ipAddr;
sf::Packet comandSend;
sf::Packet comandRecive;
sf::Packet Welcome;
int id;
int intComandoSend;
int intComandoRecive;
int clientSalt;
int serverSalt;
int saltC;
int saltS;
int challenge_R;
unsigned short port;
double reSendTime = 0.009;
bool conected;
sf::Event event;
int timer = 0;
bool helloSpam = true;

void execComand(int intComandoRecive)
{
	
	switch ((comandos)intComandoRecive)
	{
	case WELCOME:
		comandRecive >> saltC >> saltS;
		if (saltC == clientSalt && saltS == serverSalt)
		{
			comandRecive >> id;
			std::cout << "WELCOME" << std::endl;
			conected = true;
		}
		break;

	case CHALLENGE:
		helloSpam = false;
		//Falta spamear "hello" y una vez aqui se tendrian que dejar de spamear
		comandRecive >> saltC >> serverSalt;
		comandRecive >> challenge_R;
		comandRecive.clear();
		
		comandSend.clear();
		comandSend << saltC << serverSalt;
		if (saltC == clientSalt)
		{
			comandSend << comandos::CHALLENGE_R << challenge_R;
			socket->send(comandSend, "localhost", 50000) != sf::Socket::Done;
			std::cout << "Challenge enviado" << std::endl;
			comandSend.clear();
		}
		else
		{
			std::cout << "salt erroneo" << std::endl;
		}



		break;
	case CHALLENGE_R:


		break;
	case ACK_DISCONECT:
		std::cout << "desconectado" << std::endl;
		break;
	default:
		break;
	}
}
void addSalt(sf::Packet p)
{
	p << clientSalt << serverSalt; 
	std::cout << clientSalt << serverSalt;
}
void sendComand(comandos c)
{

}

void receive()
{
	socket->receive(comandRecive, ipAddr, port);

	comandRecive >> intComandoRecive;
	execComand(intComandoRecive);
	
}

void conect()
{
	//Ponemos el socket en nonBlocking porque si no se queda bloqueado esperando respuesta
	
	//Ponemos el cronometro a 0
	comandRecive >> intComandoRecive;
	auto start = std::chrono::high_resolution_clock::now();
	//Añadimos un clientSalt random al paquete y lo guardamos
	clientSalt = rand() % 9223372036854775807;
	comandSend << clientSalt << 0;

	/*while (intComandoRecive == 0)
	{
		receive();

		//comprobamos si ha pasado el tiempo minimo para enviar un HELLO de nuevo al sevidor
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		if (elapsed.count() > reSendTime)
		{
			//Enviamos un Hello si el servidor es valido y reseteamos el cronometro
			if (socket.send(comandSend, "localhost", 50000) != sf::Socket::Done)
			{
				std::cout << "¿?" << std::endl;
			}
			else
			{
				std::cout << "Hello" << std::endl;
				start = std::chrono::high_resolution_clock::now();
			}
		}
	}*/
}



void update()
{
	std::cout << "Update" << std::endl;
	if (helloSpam) {
		comandSend << comandos::HELLO;
		socket->send(comandSend, "localhost", 50000);
		comandSend.clear();
	}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			comandSend.clear();

			std::cout << "ESC" << std::endl;
			comandos comando = DISCONECT;
			intComandoSend = static_cast<int>(comando);

			comandSend << intComandoSend;
			comandSend << saltC << serverSalt;
			comandSend << id;
			socket->send(comandSend, "localhost", 50000) != sf::Socket::Done;
			std::cout << "Disconect Enviado" << std::endl;

			if (socket->receive(comandRecive, ipAddr, port) != sf::Socket::Done)
			{
				std::cout << "No hay conexion" << std::endl;
			}
			else
			{
				//Si responde almacenamos el comando y dejamos de enviar HELLO
				comandRecive >> intComandoRecive;
				comando = static_cast<comandos>(intComandoRecive);
				if (comando == ACK_DISCONECT)
				{
					std::cout << "Desconectado" << std::endl;
					//conected = false;
				}
				
			}
			timer = 0;

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			std::cout << "pa arriba" << std::endl;
			timer = 0;
		}
		if (timer == 10) 
		{/*
			comandSend.clear();

			std::cout << "Tiempo de espera superado" << std::endl;
			comandos comando = DISCONECT;
			intComandoSend = static_cast<int>(comando);

			comandSend << intComandoSend;
			comandSend << saltC << serverSalt;
			comandSend << id;
			socket.send(comandSend, "localhost", 50000) != sf::Socket::Done;
			std::cout << "Disconect Enviado" << std::endl;

			if (socket.receive(comandRecive, ipAddr, port) != sf::Socket::Done)
			{
				std::cout << "No hay conexion" << std::endl;
			}
			else
			{
				//Si responde almacenamos el comando y dejamos de enviar HELLO
				comandRecive >> intComandoRecive;
				comando = static_cast<comandos>(intComandoRecive);
				if (comando == ACK_DISCONECT)
				{
					std::cout << "Desconectado" << std::endl;
					//conected = false;
				}

			}
			timer = 0;*/
		}
	
}

	

	





int main()
{
	socket->setBlocking(true);
	if (socket->isBlocking())
		std::cout << "BLOCKIAO" << std::endl;
	sf::Packet pack;
	unsigned short s;
	sf::IpAddress ip;
	socket->receive(pack, ip, s);
	//auto start = std::chrono::high_resolution_clock::now();
	//conect();
	std::thread conectThread(receive);
	conectThread.detach();
	while (1)
	{

		update();
		/*std::chrono::duration<int, std::milli> timespan(500);
		std::this_thread::sleep_for(timespan);
		timer+=0.5f;*/
	}
	
	

	

	
	
}

