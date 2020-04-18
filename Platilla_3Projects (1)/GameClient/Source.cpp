#pragma once
#include <PlayerInfo.h>
#include <SFML\Network.hpp>
#include<thread>
#include <iostream>
#include <string>
#include <vector>
#include <fcntl.h>
#include "Graphics.h"



sf::UdpSocket socket;
std::string prueba = "Prueba conexion";
sf::IpAddress ipAddr;
sf::Packet Hello;
sf::Packet Welcome;
unsigned short port;


void conect()
{
	Hello << 1;
	//Vincular el socket en función de si somos server o no
	//Rellenar data
	if (socket.send(Hello, "localhost", 50000) != sf::Socket::Done);
	
	std::cout << "Enviado" << std::endl;
		//Error al enviar
	//if (socket.receive(Welcome, ipAddr, port) != sf::Socket::Done);
	Hello.clear();
}






int main()
{
	

	
		conect();
	

	

	
	
}

