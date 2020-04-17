#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <PlayerInfo.h>
#include <vector>
sf::TcpSocket incoming;
std::list<sf::TcpSocket*> jugadores;

//Funcion por la que el servidor recibe mensajes

//enum comandos {
//	START, LANZADADOS, ELEGIRPISTA, REVELARPISTA, MOVIMIENTO, NUEVAPOSICION, DEDUCCION, ENVIOSUPOSICION,
//	DESMENTIRCS, DESMENTIRSC, DESMENTIDO, DESMENTIDOGENERAL, RESOLVERCS, RESOLVERSC, RESOLUCIONCORRECTA
//};



int dado1;
int dado2;
std::list<carta> baraja;
std::list<carta> crimen;
//std::list<PlayerInfo> listaPlayers;
std::vector<PlayerInfo> listaPlayers;
std::list<sf::TcpSocket*>::iterator jugadorActual;
int id_jugadorActual;
sf::Packet comando;
int comandServer;
int comandClient;
bool end_turno = false;

int n = 0; // ID jugador en listaPlayer




void crearBaraja()
{
	//6 personajes
	baraja.push_back(carta(PERSONAJE,"Amapola",1)); //1
	baraja.push_back(carta(PERSONAJE,"Rubio",2)); //2
	baraja.push_back(carta(PERSONAJE,"Orquidea",3));//3
	baraja.push_back(carta(PERSONAJE,"Celeste",4));//4
	baraja.push_back(carta(PERSONAJE,"Prado",5));//5
	baraja.push_back(carta(PERSONAJE,"Mora",6));//6
	
	//6 armas
	baraja.push_back(carta(ARMA,"Candelabro",7));//7
	baraja.push_back(carta(ARMA,"Cuerda",8));//8
	baraja.push_back(carta(ARMA,"Punal",9));//9
	baraja.push_back(carta(ARMA,"Pistola",10));//10
	baraja.push_back(carta(ARMA,"Tuberia",11));//11
	baraja.push_back(carta(ARMA,"Herramienta",12));//12
	
	//9 habitaciones
	baraja.push_back(carta(SALA,"Biblioteca",13));//13
	baraja.push_back(carta(SALA,"Cocina",14));//14
	baraja.push_back(carta(SALA,"Billar",15));//15
	baraja.push_back(carta(SALA,"Baile",16));//16
	baraja.push_back(carta(SALA,"Invernadero",17));//17
	baraja.push_back(carta(SALA,"Comedor",18));//18
	baraja.push_back(carta(SALA,"Vestibulo",19));//19
	baraja.push_back(carta(SALA,"Salon",20));//20
	baraja.push_back(carta(SALA,"Estudio",21));//21



}
void repartirCartas()
{
	int r;
/*	for (int i = 0; jugadores.size() > i; i++)
	{
		listaPlayers.push_back(PlayerInfo());
	}*/

	for (; baraja.size() != 0;)
	{
		for (int i = 0; listaPlayers.size() > i; i++)
		{
			std::list<carta>::iterator it =baraja.begin();
			//std::list<PlayerInfo>::iterator itP = listaPlayers.begin();
	
			if (baraja.size() > 0)
			{
				r = rand() % baraja.size();
				std::advance(it, r);
				listaPlayers[i].mano.push_back(*it);
				baraja.erase(it);
			}

		}
	}

	
}
void enviarRecibirPista()
{
	
	comandClient = 2;
	std::string pista;
	//comando.clear();
	//Recibimos la eleccion de pista del cliente
	//(*jugadorActual)->receive(comando);
	comando >> pista;
	std::cout << "PISTA ELEGIDA: " << pista << std::endl;
	int i = 0;

	PlayerInfo playerPista;
	playerPista.name = "null";
	for (std::list<sf::TcpSocket*>::iterator it = jugadores.begin(); jugadores.end() != it; it++)
	{
		for (std::list<carta>::iterator ot = listaPlayers[i].mano.begin(); listaPlayers[i].mano.end() != ot; ot++)
		{
			if (pista == (*ot).nombre) 
			{
				playerPista.name = listaPlayers[i].name;
				exit;
			}
		}

			i++;
	}
	comando.clear();
	std::string pistaFinal;
	if (playerPista.name == "null")
	{
		pistaFinal = "Ningun jugador tiene la carta " + pista;
		comando << comandClient << pistaFinal;
	}
	else
	{
		pistaFinal = "El jugador con el Nick: "+ playerPista.name + " tiene la carta" + pista;
		comando << comandClient << pistaFinal;
	}
	
	for (std::list<sf::TcpSocket*>::iterator it = jugadores.begin(); jugadores.end() != it; it++)
	{
		(*it)->send(comando);
		std::cout << "Envio" << std::endl;
	}
}

void tirarDados()
{
	comando.clear();
	comandClient = 1;

	bool pista = false;
	int r = 0;
	dado1 = rand() % 6 + 1;
	dado2 = rand() % 6 + 1;

	if (dado1 == 1 || dado2 == 1)
	{
		pista = true;
		r = rand() % 2;
	}
		

	dado1 += dado2;
	
	comando << comandClient << dado1 << pista << r;

	(*jugadorActual)->send(comando);

	

	/*sf::Packet dadosPack;
	dadosPack.clear();
	
	dado1 = rand() % 6 + 1;
	dado2 = rand() % 6 + 1;

	std::cout << dado1 << "  "<< dado2 << std::endl;
	if (dado1 == 1 || dado2 == 1)
		pista = true;

	dado1 += dado2;
	dadosPack << dado1 << pista;
	(*jugadorActual)->send(dadosPack);

	if (pista)
		enviarRecibirPista();*/
}

void actualizarPos()
{
	comandClient = 3;
	float x;
	float y;
	comando >> x >> y;
	x= listaPlayers[n].x;
	y= listaPlayers[n].y;

	comando.clear();
	std::cout << "Actualizo la pos del jugador" << std::endl;

	comando << comandClient << n << x << y;

	for (std::list<sf::TcpSocket*>::iterator it = jugadores.begin(); jugadores.end() != it; it++)
	{
		(*it)->send(comando);
		std::cout << "Envio" << std::endl;
	}
	comando.clear();
	comando << 4;
	(*jugadorActual)->send(comando);
}

void Enviar()
{
	int i = 0;
	for (std::list<sf::TcpSocket*>::iterator it = jugadores.begin();jugadores.end() != it; it++)
	{
		sf::Packet packMano;
		std::list<carta>::iterator itM = listaPlayers[i].mano.begin();

		int manosize =  listaPlayers[i].mano.size();
		packMano << manosize;
		//std::cout << listaPlayers[i].mano.size();
			for (int j = 0; listaPlayers[i].mano.size() > j; j++)
			{			
				packMano << itM->numero;
				//std::cout << itM->numero << std::endl;
				itM++;
			}	
			//packMano << 1;
			
		(*it)->send(packMano);
		i++;
	}

}

void asignarHabitacion()
{
	int counter1 = 0;
	int counter=0;
	sf::Packet packHabitacion;

	int* r = NULL;
	r = new int[listaPlayers.size()];

	int tamaño = listaPlayers.size();
	//Se envia a clientes cuantos jugadores hay en la partida
	packHabitacion << tamaño;

	for (std::list<sf::TcpSocket*>::iterator it = jugadores.begin(); jugadores.end() != it; it++)
	{
		r[counter1] = rand() % 9 + 1;
		counter1++;
	}
	
	for (std::list<sf::TcpSocket*>::iterator it = jugadores.begin(); jugadores.end() != it; it++)
	{	
		packHabitacion << r[counter]<<listaPlayers[counter].id;	
		counter++;
	}

	for (std::list<sf::TcpSocket*>::iterator it = jugadores.begin(); jugadores.end() != it; it++)
	{		
		(*it)->send(packHabitacion);
	}
}

std::list<carta> crearCrimen(std::list<carta> &b)
{
	
	std::list<carta> c;
	std::list<carta>::iterator it = b.begin();

	int r;
	r = rand() %  6;
	std::advance(it, r);
	c.push_back(*it);
	b.erase(it);

	r = rand() % 5 + 6;
	it = b.begin();
	std::advance(it, r );
	c.push_back(*it);
	b.erase(it);
	

	r = rand() % 7 + 12;
	it = b.begin();
	std::advance(it, r );
	c.push_back(*it);
	b.erase(it);
	return c;
	
}


void inicioPartida()
{
	crearBaraja();
	crimen = crearCrimen(baraja);
	repartirCartas();
	Enviar();
	asignarHabitacion();
	
}


void setUp(sf::TcpSocket* _client)
{
	PlayerInfo player;
	int _id;
	_id = listaPlayers.size();
	sf::Packet packNombre;
	std::string _name;
	_client->receive(packNombre);
	packNombre >> _name;
	player.name = _name;
	player.id = _id;
	sf::Packet packId;
	packId << _id;
	_client->send(packId);
	listaPlayers.push_back(player);



	std::cout << "LLega el cliente con nick:" << _name;
}

void ControlServidor()
{
	bool running = true;
	bool inicio = false;
	// Create a socket to listen to new connections
	sf::TcpListener listener;
	sf::Socket::Status status = listener.listen(50000);
	if (status != sf::Socket::Done)
	{
		std::cout << "Error al abrir listener\n";
		exit(0);
	}
	// Create a list to store the future clients

	// Create a selector
	sf::SocketSelector selector;
	// Add the listener to the selector
	selector.add(listener);
	// Endless loop that waits for new connections
	while (running)
	{
		// Make the selector wait for data on any socket
		if (selector.wait())
		{
			// Test the listener
			if (selector.isReady(listener))
			{
				// The listener is ready: there is a pending connection
				sf::TcpSocket* client = new sf::TcpSocket;
				if (listener.accept(*client) == sf::Socket::Done)
				{
					// Add the new client to the clients list
					setUp(client);
					std::cout <<" y con puerto: " << client->getRemotePort()<< std::endl;
					jugadores.push_back(client);
					// Add the new client to the selector so that we will
					// be notified when he sends something
					selector.add(*client);
				}
				else
				{
					// Error, we won't get a new connection, delete the socket
					std::cout << "Error al recoger conexiÃ³n nueva\n";
					delete client;
				}
			}
			else
			{
				// The listener socket is not ready, test all other sockets (the clients)
				for (std::list<sf::TcpSocket*>::iterator it = jugadores.begin(); it != jugadores.end(); ++it)
				{
					sf::TcpSocket& client = **it;
					if (selector.isReady(client))
					{
						// The client has sent some data, we can receive it
						sf::Packet packet;
						status = client.receive(packet);
						if (status == sf::Socket::Done)
						{
							std::string strRec;
							packet >> strRec;
							std::cout << "He recibido " << strRec << " del puerto " << client.getRemotePort() << std::endl;
						}
						else if (status == sf::Socket::Disconnected)
						{
							selector.remove(client);
							std::cout << "Elimino el socket que se ha desconectado\n";
						}
						else
						{
							std::cout << "Error al recibir de " << client.getRemotePort() << std::endl;
						}
					}
				}
			}
		}
		if (jugadores.size() == 3 && !inicio)
		{
			inicioPartida();
			std::cout << "Empiezo" << std::endl;
			inicio = true;
			running = false;
		}
	}

}

void desmentir()
{
	std::string arma, sala, personaje;
	int jugadorIzquierda = n+1;
	comando >> arma >> sala >> personaje;

	std::string deduccion = "El jugador " + listaPlayers[n].name + " ha hecho la siguiente deduccion: " + personaje + " " + sala + " " + arma;
	comando.clear();
	comandClient = 5;
	comando << comandClient << deduccion;
	for (std::list<sf::TcpSocket*>::iterator it = jugadores.begin(); jugadores.end() != it; it++)
	{
		(*it)->send(comando);
		//std::cout << "Envio deduccion" << std::endl;
	}
	comando.clear();
	comandClient = 6;
	comando << comandClient;
	int j = 0;
	bool tieneAlguna = false;
	
	for (std::list<sf::TcpSocket*>::iterator it = jugadores.begin(); jugadores.end() != it; it++)
	{
		if (jugadorActual != (it))
		{

		
		
		for (std::list<carta>::iterator ot = listaPlayers[j].mano.begin(); listaPlayers[j].mano.end() != ot; ot++)
		{
			if ((ot->nombre == arma))
			{
				comando << arma;
				tieneAlguna = true;
			}
				
			if((ot->nombre == sala))
			{
				comando << sala;
				tieneAlguna = true;
			}
			if ((ot->nombre == personaje))
			{
				comando << personaje;
				tieneAlguna = true;
			}
		}

		if (tieneAlguna)
		{
			(*it)->send(comando);
			std::cout << "Envio al que toca" << std::endl;
			break;
		}
		}
		std::cout << "vuelta";
		j++;
	}
	if (!tieneAlguna)
	{
		std::string nadie = "Ningun jugador tiene ninguna de las 3 cartas";
		for (std::list<sf::TcpSocket*>::iterator it = jugadores.begin(); jugadores.end() != it; it++)
		{
			comando.clear();
			comandClient = 7;
			comando << comandClient << nadie;
			(*it)->send(comando);

		}
	}
	
}

void enseñarCarta()
{
	std::string carta;
	comando >> carta;
	comando.clear();
	comandClient = 8;
	comando << comandClient << carta;
	for (std::list<sf::TcpSocket*>::iterator it = jugadores.begin(); jugadores.end() != it; it++)
	{
		(*it)->send(comando);
	std::cout << "enviando la carta" << std::endl;

	}
}
void recibirPacket()
{
	std::cout << "esperando" << std::endl;
	(*jugadorActual)->receive(comando);
	comando >> comandServer;
	std::cout << "He recibido " << comandServer;
	switch (comandServer)
	{
	case 1:    //Recibir Pista
		enviarRecibirPista(); 		
		break;
	case 2:		//Recibir pos
		actualizarPos();
		break;
	case 3:     //Recibir deduccion
		desmentir();
		break;
	case 4:
		enseñarCarta();
		break;
	}
}
void turno()
{

	
	//(*jugadorActual)->send(comando);
	tirarDados();
	while (!end_turno)
	{
		recibirPacket();

	}
	

}
void partida()
{
	std::cout << "entro en la partida" << std::endl;
	std::list<sf::TcpSocket*>::iterator j = jugadores.begin();
	jugadorActual = j;
	
	
	while (1)
	{
		turno();
		if (j == jugadores.end())
		{
			j = jugadores.begin();
		}
		else
		{
			j++;
			jugadorActual = j;
		}
		if (listaPlayers.size() == n)
		{
			n = 0;
		}
		else
		{
			n++;
		}

		

	}
}

int main()
{

	srand(time(NULL));
	std::cout << "Esperando jugadores..." << std::endl;
	ControlServidor();
	partida();
	
	//tirarDados();

	std::cout << "Error" << std::endl;

	
	return 0;
}

//PlayerInfo playerInfo;
//
//sf::TcpSocket socket;
//sf::TcpListener dispatcher;
//sf::Packet pack;
//std::string prueba;
//
//
////5000 es el puerto por el que escucha el servidor.
////El cliente debe conocer la ip del servidor y el puerto por el que escucha.
//sf::Socket::Status status = dispatcher.listen(5000);
//
//std::list<sf::TcpSocket> clientes;
//
//std::cout << "Buscando" << std::endl;
//if (status != sf::Socket::Done)
//{
//	std::cout << "No se puede vincular" << std::endl;
//}
//
////Al conectarse un cliente, el socket incoming pasa a ser el que utilizan este
////cliente y el servidor para comunicarse en exclusiva
//if (dispatcher.accept(incoming) != sf::Socket::Done)
//{
//	std::cout << "Error" << std::endl;
//}
//else
//{
//	std::cout << "Conectado el cliente con puerto " << incoming.getRemotePort() << std::endl;
//	//clientes.push_back(incoming);
//}
//
//
//incoming.receive(pack);
//pack >> prueba;
//std::cout << prueba;
//
////for(int i = 0; i<clientes.size(); i++)
//
//
//
//while (1)
//{
//
//}
//
