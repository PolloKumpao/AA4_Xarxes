#pragma once
#include <PlayerInfo.h>
#include <SFML\Network.hpp>
#include<thread>
#include <iostream>
#include <string>
#include <vector>
#include <fcntl.h>
#include "Graphics.h"


enum  comandos { HELLO, WELCOME, CHALLENGE, CHALLENGE_R};

sf::UdpSocket socket;
std::string prueba = "Prueba conexion";
sf::IpAddress ipAddr;
sf::Packet comand;
sf::Packet Welcome;
int intComando;
int challenge_R;
unsigned short port;


void conect()
{
	comand << 0 ;
	//Vincular el socket en función de si somos server o no
	//Rellenar data
	if (socket.send(comand, "localhost", 50000) != sf::Socket::Done);

	std::cout << "Enviado" << std::endl;
	//Error al enviar
//if (socket.receive(Welcome, ipAddr, port) != sf::Socket::Done);
	comand.clear();

	if (socket.receive(comand, ipAddr, port) != sf::Socket::Done)
	{
		std::cout << "No hay conexion" << std::endl;

	}
	else
	{

		comand >> intComando;
		comandos comando = static_cast<comandos>(intComando);
		switch (comando)
		{
		case HELLO:

			break;
		case WELCOME:


			break;
		case CHALLENGE:
			comand >> challenge_R;
			comand.clear();
			comando = CHALLENGE_R;
			intComando = static_cast<int>(comando);
			comand << intComando << challenge_R;
			socket.send(comand, "localhost", 50000) != sf::Socket::Done;
			comand.clear();


			break;
		case CHALLENGE_R:


			break;
		default:
			break;
		}
	}
}






int main()
{
	

	
		conect();
	

	

	
	
}

