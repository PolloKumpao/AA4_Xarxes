#pragma once
#include <PlayerInfo.h>
#include <SFML\Network.hpp>
#include<thread>
#include <iostream>
#include <string>
#include <vector>
#include <fcntl.h>
#include "Graphics.h"

sf::Packet comando;
int comandClient  = 0 ;
int comandServer = 0;

sf::TcpSocket socket;
std::string prueba = "Prueba conexion";
sf::Packet pack;


PlayerInfo playerInfo;
Graphics g;





sf::Socket::Status status = socket.connect("localhost", 50000, sf::milliseconds(15.f));
sf::Packet packetMano;
std::vector<carta> mano;
std::vector<PlayerInfo> players;
int cartaRepartida = 0;
int size = 0;
int habitacionInicial[3];
sf::Packet packHabitacion;
int numPlayers = 0;
int id = 0;
bool pista = false;
int dados = 0;
int tipoPista = 0;
int n = 0;

estados estadoPlayer = WAIT;
sf::Event event;
bool end_game = false;
//enum comandos {
//	START, LANZADADOS, ELEGIRPISTA, REVELARPISTA, MOVIMIENTO, NUEVAPOSICION, DEDUCCION, ENVIOSUPOSICION,
//	DESMENTIRCS, DESMENTIRSC, DESMENTIDO, DESMENTIDOGENERAL, RESOLVERCS, RESOLVERSC, RESOLUCIONCORRECTA
//};




void inicioPartida()
{

	if (status != sf::Socket::Done)
	{
		std::cout << "Error al establecer conexion\n";
		exit(0);
	}
	else
	{
		std::cout << "Introduce tu Nick:" << std::endl;
		std::string name;
		std::cin >> name;
		sf::Packet packNombre;
		packNombre << name;
		socket.send(packNombre);
		sf::Packet packId;
		socket.receive(packId);
		packId >> id;
		std::cout << "Se ha establecido conexion\n";
		std::cout << "Tu id de jugador es: " << id << std::endl;
	}

	std::string str = "hola";

	do
	{
		socket.receive(packetMano);
		packetMano >> size;

		std::cout << size << std::endl;
		for (int i = 0; i < size; i++)
		{
			packetMano >> cartaRepartida;

			switch (cartaRepartida)
			{
			case 1:
				mano.push_back((carta(PERSONAJE, "Amapola", 1)));
				break;
			case 2:
				mano.push_back((carta(PERSONAJE, "Rubio", 2)));
				break;
			case 3:
				mano.push_back((carta(PERSONAJE, "Orquidea", 3)));
				break;
			case 4:
				mano.push_back((carta(PERSONAJE, "Celeste", 4)));
				break;
			case 5:
				mano.push_back((carta(PERSONAJE, "Prado", 5)));
				break;
			case 6:
				mano.push_back((carta(PERSONAJE, "Mora", 6)));
				break;
			case 7:
				mano.push_back((carta(PERSONAJE, "Candelabro", 7)));
				break;
			case 8:
				mano.push_back((carta(PERSONAJE, "Cuerda", 8)));
				break;
			case 9:
				mano.push_back((carta(PERSONAJE, "Puñal", 9)));
				break;
			case 10:
				mano.push_back((carta(PERSONAJE, "Pistola", 10)));
				break;
			case 11:
				mano.push_back((carta(PERSONAJE, "Tuberia de Plomo", 11)));
				break;
			case 12:
				mano.push_back((carta(PERSONAJE, "Herramienta", 12)));
				break;
			case 13:
				mano.push_back((carta(PERSONAJE, "Biblioteca", 13)));
				break;
			case 14:
				mano.push_back((carta(PERSONAJE, "Cocina", 14)));
				break;
			case 15:
				mano.push_back((carta(PERSONAJE, "Billar", 15)));
				break;
			case 16:
				mano.push_back((carta(PERSONAJE, "Baile", 16)));
				break;
			case 17:
				mano.push_back((carta(PERSONAJE, "Invernadero", 17)));
				break;
			case 18:
				mano.push_back((carta(PERSONAJE, "Comedor", 18)));
				break;
			case 19:
				mano.push_back((carta(PERSONAJE, "Vestibulo", 19)));
				break;
			case 20:
				mano.push_back((carta(PERSONAJE, "Salon", 20)));
				break;
			case 21:
				mano.push_back((carta(PERSONAJE, "Estudio", 21)));
				break;
			default:
				break;
			}
		}
		for (int i = 0; i < mano.size(); i++)
		{
			std::cout << mano[i].nombre << std::endl;
		}


		socket.receive(packHabitacion);
		packHabitacion >> numPlayers;

		int hab = 0;
		for (int i = 0; i < numPlayers; ++i)
		{
			PlayerInfo player;
			packHabitacion >> hab;
			switch (hab)
			{
			case 1:
				player.habitacion = BILLAR;
				player.x = 14;
				player.y = 5;
				break;
			case 2:
				player.habitacion = BIBLIOTECA;
				player.x = 24;
				player.y = 5;
				break;
			case 3:
				player.habitacion = COCINA;
				player.x = 5;
				player.y = 25;
				break;
			case 4:
				player.habitacion = BAILE;
				player.x = 4;
				player.y = 14;
				break;
			case 5:
				player.habitacion = INVERNADERO;
				player.x = 4;
				player.y = 5;
				break;
			case 6:
				player.habitacion = COMEDOR;
				player.x = 20;
				player.y = 25;
				break;
			case 7:
				player.habitacion = VESTIBULO;
				player.x = 35;
				player.y = 10;
				break;
			case 8:
				player.habitacion = SALON;
				player.x = 35;
				player.y = 26;
				break;
			case 9:
				player.habitacion = ESTUDIO;
				player.x = 36;
				player.y = 5;
				break;
			}
			packHabitacion >> player.id;
			players.push_back(player);
		}
		for (int i = 0; i < numPlayers; ++i)
		{
			std::cout << "El jugador " << players[i].id << " comienza en la habitacion " << players[i].habitacion << std::endl;
		}
		str = "Fin Inicio";
		/*std::getline(std::cin, str);
		sf::Packet packet;
		packet << str;
		status = socket.send(packet);
		if (status != sf::Socket::Done)
		{
		std::cout << "Error al enviar\n";
		}*/
		std::cout << std::endl;
	} while (str != "Fin Inicio");
}

void turno()
{

	comando >> dados >> pista >> tipoPista;
	g.movements = dados;
	std::cout << "Has sacado un " << dados;
	std::string pistaElegir;

	if (!pista)
	{
		std::cout << " y no tienes pista." << std::endl;
	}

	else
	{
		
		
			std::cout << " tienes pista de " << std::endl;
			
			tipoPista = 0;
			switch (tipoPista)
			{
			case 0:
				std::cout << "ARMA. Elige entre Candelabro | Cuerda | Punal | Pistola | Tuberia | Herramienta." << std::endl;
				while (pistaElegir != "Candelabro" && pistaElegir != "Cuerda" && pistaElegir != "Punal" && pistaElegir != "Pistola" && pistaElegir != "Tuberia" && pistaElegir != "Herramienta")
				{
					std::cout << "Introduce el nombre correcto del arma que elijas" << std::endl;
					std::cin >> pistaElegir;
				}
				break;

			case 1:
				std::cout << "SALA. Elige entre Biblioteca | Cocina | Billar | Baile | Invernadero | Comedor | Salon | Estudio." << std::endl;
				while (pistaElegir != "Biblioteca" && pistaElegir != "Cocina" && pistaElegir != "Billar" && pistaElegir != "Baile" && pistaElegir != "Invernadero" && pistaElegir != "Comedor" && pistaElegir != "Salon" && pistaElegir != "Estudio")
				{
					std::cout << "Introduce el nombre correcto de la sala que elijas" << std::endl;
					std::cin >> pistaElegir;
				}
				break;

			case 2:
				std::cout << "PERSONAJE. Elige entre Amapola | Rubio | Orquidea | Celeste | Prado | Mora. " << std::endl;
				while (pistaElegir != "Amapola" && pistaElegir != "Rubio" && pistaElegir != "Orquidea" &&  pistaElegir != "Celeste" && pistaElegir != "Prado" && pistaElegir != "Mora")
				{
					std::cout << "Introduce el nombre correcto del personaje que elijas" << std::endl;
					std::cin >> pistaElegir;
				}
				break;
			}
		}

		comando.clear();
		comandServer = 1;
		comando << comandServer << pistaElegir;    // comando Revelar Pista
		comando << pistaElegir;

		socket.send(comando);
		comando.clear();

		
	

		do {
			//MOVIMIENTO
			//send posicion 
			
			if(event.type == sf::Event::KeyPressed)
				if (event.key.code == sf::Keyboard::K)
				{
					estadoPlayer = WAIT;
				}
			sf::Packet packPos;
			packPos << g.playerX << g.playerY;
			socket.send(packPos);
			estadoPlayer = WAIT;
			//ESTADO = WAIT
		} while (estadoPlayer == TURNO);
}

std::string habitacion()
{
	if (players[n].x < 8 && players[n].y < 10)
		return "Invernadero";
	else if (players[n].x > 11 && players[n].x < 18 && players[n].y < 10)
		return "Billar";
	else if (players[n].x > 21 && players[n].x < 28 && players[n].y < 10)
		return "Biblioteca";
	else if (players[n].x > 31 && players[n].y < 10)
		return "Estudio";
	else if (players[n].x < 8 && players[n].y>11 && players[n].y < 18)
		return "Baile";
	else if (players[n].x > 29 && players[n].y > 11 && players[n].y < 20)
		return "Vestibulo";
	else if (players[n].x < 10 && players[n].x >19 && players[n].y < 30)
		return "Cocina";
	else if (players[n].x > 12 && players[n].x < 26 && players[n].y >19)
		return "Comedor";
	else if (players[n].x > 29 && players[n].y > 21)
		return "Salon";
	else
		return "NONE";
}


void revelarPista()
{
	std::string pistaRevelada;
	comando >> pistaRevelada;
	std::cout << pistaRevelada;
	comando.clear();

}
void enviarMov()
{
	comando.clear();
	comandServer = 2;
	while (event.key.code != sf::Keyboard::Enter)
	{
		/*if (event.key.code != sf::Keyboard::Enter)
			break;*/
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			break;
	}
	std::cout << "La pos es: "<< g.playerX << g.playerY << std::endl;
	comando << comandServer << g.playerX << g.playerY;
	socket.send(comando);
	comando.clear();

}
void deduccion()
{
	comandServer = 3;
	comando.clear();
	std::string arma, sala, personaje;
	sala = habitacion();
	if (sala == "NONE")
	{
		std::cout << "Necesitas estar en una sala para hacer una deducción"  << std::endl;
	}
	else
	{
		std::cout << "Haz tu deduccion: " << std::endl;
		std::cout << "Sala: " << sala << std::endl;
		std::cout << "ARMA.Elige entre Candelabro | Cuerda | Punal | Pistola | Tuberia | Herramienta." << std::endl;
		do
		{
			std::cout << "Introduce el nombre correcto" << std::endl;
		  std::cin >> arma;
		  
		} while (arma != "Candelabro" && arma != "Cuerda" && arma != "Punal" && arma != "Pistola" && arma != "Tuberia" && arma != "Herramienta");
		
		
		std::cout << "PERSONAJE. Elige entre Amapola | Rubio | Orquidea | Celeste | Prado | Mora. " << std::endl;
		while (personaje != "Amapola" && personaje != "Rubio" && personaje != "Orquidea" &&  personaje != "Celeste" && personaje != "Prado" && personaje != "Mora")
		{
			std::cout << "Introduce el nombre correcto" << std::endl;
			std::cin >> personaje;
		}

		comando << comandServer << arma << sala << personaje;
		socket.send(comando);
		comando.clear();
		
	}
	
}
void actualizarPos()
{
	
	float x, y;
	x = g.playerX;
	y = g.playerY;
	comando >> n >> x >> y;
	players[n].x = x;
	players[n].y = y;
	comando.clear();
	std::cout << "posicion actualizada" << std::endl;
	
}
void printDeduccion()
{
	std::string deduccionPlayer;
	comando >> deduccionPlayer;
	std::cout << deduccionPlayer << std::endl;
	comando.clear();
}
void opcionesDesmentir()
{
	std::string opcion1, opcion2, opcion3;
	std::string eleccion;
	comando >> opcion1 >> opcion2 >> opcion3;
	comando.clear();
	std::cout << "Escoge la carta que quieras mostrar: " << opcion1 << " | " << opcion2 << " | "<< opcion3 << std::endl;
	do 
	{
		std::cout << "Introduce una de las opciones" << std::endl;
		std::cin >> eleccion;
	} while (eleccion != opcion1 && eleccion != opcion2 && eleccion != opcion3);
	comando.clear();
	comandServer = 4 ;
	comando << comandServer << eleccion;
	socket.send(comando);
	std::cout << "Enviado un " << comandServer << std::endl;

	
}
void nadie()
{
	std::string mensaje;
	comando >> mensaje;
	std::cout << mensaje << std::endl;
	comando.clear();
}
void printCarta()
{
	std::string carta;
	comando >> carta;
	std::cout << "Se muestra la carta: " << carta;
	comando.clear();
}
void recibirComando()
{
	while (!end_game)
	{

		socket.receive(comando);
		comando >> comandClient;
		switch (comandClient)
		{
		case 1: //Recibes dados
			turno();

			break;
		case 2: //Revelar pista
			revelarPista();
			enviarMov();
			break;
		case 3: //Actualizar pos
			actualizarPos();
			break;
		case 4: //Deduccion
			deduccion();
			break;
		case 5: //Printar deduccion
			printDeduccion();
			break;
		case 6 : //Opciones a elegir
			opcionesDesmentir();
			
			break;
		case 7: //Nadie tiene ninguna
			nadie();
			break;
		case 8: //Printa la carta escogida por la pantalla de todos los jugadores
			printCarta();
		}
	}
	
}





int main()
{
	inicioPartida();

	//recieve;
	std::thread recieve(recibirComando);

	g.DrawDungeon(socket);

	
	

	

	
	
	/*wait();
	turno();*/

	
	//socket.disconnect();

	while (1)
	{

	}


	//sf::Socket::Status status = socket.connect("localhost", 5000, sf::seconds(5.f));
	//if (status != sf::Socket::Done)
	//{
	//	std::cout << "No se ha podido conectar";
	//}
	//else {
	//	std::cout << "Conectado" << std::endl;
	//}

	//pack << prueba;
	//socket.send(pack);

	//while (1)
	//{

	//}

	return 0;
}

