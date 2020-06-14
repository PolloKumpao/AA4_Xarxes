#include "Graphics.h"
#include <iostream>




Graphics::Graphics()
{

	Sala invernadero("Invernadero", 0, 0, 8, 10, sf::Color::Yellow);
	Sala salaBillar("Sala de billar", 12, 0, 6, 10, sf::Color::Red);
	Sala biblioteca("Biblioteca", 22, 0, 6, 10, sf::Color::Cyan);
	Sala estudio("Estudio", 32, 0, 8, 10, sf::Color::Magenta);
	Sala salaBaile("Sala de baile", 0, 12, 8, 6, sf::Color::Blue);
	Sala vestibulo("vestibulo", 30, 12, 10, 8, sf::Color::White);
	Sala cocina("cocina", 0, 20, 10, 10, sf::Color::Green);
	Sala comedor("comedor", 13, 20, 13, 10, sf::Color(150, 0, 150));
	Sala salon("salon", 30, 22, 10, 8, sf::Color(0, 150, 150));
	salas[0] = invernadero;
	salas[1] = salaBillar;
	salas[2] = biblioteca;
	salas[3] = estudio;
	salas[4] = salaBaile;
	salas[5] = vestibulo;
	salas[6] = cocina;
	salas[7] = comedor;
	salas[8] = salon;

	centroMensajes.color = sf::Color(150, 150, 150);
	centroMensajes.origen.x = 12;
	centroMensajes.origen.y = 12;
	centroMensajes.longitud.x = 14;
	centroMensajes.longitud.y = 6;
}


/*
Sala invernadero("Invernadero", 0, 0, 8, 10, sf::Color::Yellow);
Sala salaBillar("Sala de billar", 12, 0, 6, 10, sf::Color::Red);
Sala biblioteca("Biblioteca", 22, 0, 6, 10, sf::Color::Cyan);
Sala estudio("Estudio", 32, 0, 8, 10, sf::Color::Magenta);
Sala salaBaile("Sala de baile", 0, 12, 8, 6, sf::Color::Blue);
Sala vestibulo("vestibulo", 30, 12, 10, 8, sf::Color::White);
Sala cocina("cocina", 0, 20, 10, 10, sf::Color::Green);
Sala comedor("comedor", 13, 20, 13, 10, sf::Color(150, 0, 150));
Sala salon("salon", 30, 22, 10, 8, sf::Color(0, 150, 150));*/

bool isInRoom(float plx, float ply)
{
	if ((plx < 8 && ply < 10) || //invernadero
		(plx > 11 && plx < 18 && ply < 10) ||  //salaBillar
		(plx > 21 && plx < 28 && ply < 10) ||   //biblioteca
		(plx > 31 && ply < 10) ||			  //estudio
		(plx < 8 && ply > 11 && ply < 18) ||   //salaBaile
		(plx > 29 && ply > 11 && ply < 20) ||  //vestibulo
		(plx < 10 && ply > 19 && ply < 30) ||  //cocina
		(plx > 12 && plx < 26 && ply >19) ||  //comedor
		(plx > 29 && ply > 21))				  //salon
		return true;

	return false;
}

void Graphics::DrawDungeon(sf::TcpSocket &socket)
{
	sf::Packet packet;
	sf::RenderWindow _window(sf::VideoMode(800, 600), "Ventanita");
	sf::RectangleShape shape(sf::Vector2f(SIZE, SIZE));
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(2.f);


	int c_movements = 0;

	float playerX_init = playerX;
	float playerY_init = playerY;

	//PLAYER
	sf::RectangleShape player(sf::Vector2f(SIZE, SIZE));
	player.setOutlineColor(sf::Color::Black);
	player.setOutlineThickness(1.f);
	while (_window.isOpen())
	{
		sf::Event event;
		bool playerMoved = false;
		while (_window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				_window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
				{
					packet.clear();
					_window.close();
					socket.send(packet);
					std::cout << "desconectado" << std::endl;
				}
				if (event.key.code == sf::Keyboard::Left)
				{
					if (playerX > 0 && c_movements < movements)
					{
						if (!(playerX == 26 && playerY < 18 && playerY>11))
						{
							playerX--;
							if (!isInRoom(playerX, playerY))
								c_movements++;
						}
					}
					std::cout << "MOVIMIENTOS RESTANTES: " << movements - c_movements << std::endl;
				}
				else if (event.key.code == sf::Keyboard::Up)
				{
					if (playerY > 0 && c_movements < movements)
					{
						if (!(playerY == 18 && playerX > 11 && playerX < 26))
						{
							playerY--;
							if (!isInRoom(playerX, playerY))
								c_movements++;

						}
					}
					std::cout << "MOVIMIENTOS RESTANTES: " << movements - c_movements << std::endl;
				}

				else if (event.key.code == sf::Keyboard::Right)
				{
					if (playerX < 39 && c_movements < movements)
					{
						if (!(playerX == 11 && playerY < 18 && playerY>11))
						{
							playerX++;
							if (!isInRoom(playerX, playerY))
								c_movements++;
						}
					}
					std::cout << "MOVIMIENTOS RESTANTES: " << movements - c_movements << std::endl;
				}
				else if (event.key.code == sf::Keyboard::Down)
				{
					if (playerY < 29 && c_movements < movements)
					{
						if (!(playerY == 11 && playerX > 11 && playerX < 26))
						{
							playerY++;
							if (!isInRoom(playerX, playerY))
								c_movements++;
						}
					}
					std::cout << "MOVIMIENTOS RESTANTES: " << movements - c_movements << std::endl;
				}
				else if (event.key.code == sf::Keyboard::BackSpace)
				{
					playerX = playerX_init;
					playerY = playerY_init;
					c_movements = 0;
					std::cout << "Reset";
					std::cout << c_movements;
				}
				else if (event.key.code == sf::Keyboard::Enter)
				{
					
				}
				break;

			}

		}
		_window.clear();
		for (int i = 0; i < W_WINDOW_TITLE; i++)
		{
			for (int j = 0; j < H_WINDOW_TITLE; j++)
			{

				shape.setFillColor(sf::Color(90, 90, 90, 255));
				player.setFillColor(sf::Color(255, 0, 0, 255));

				shape.setPosition(sf::Vector2f(i*SIZE, j*SIZE));
				player.setPosition(sf::Vector2f(playerX*SIZE, playerY*SIZE));

				_window.draw(shape);
			}
		}

		for (size_t i = 0; i < salas.size(); i++)
		{
			salas[i].Draw(_window);
			_window.draw(player);
		}
		centroMensajes.Draw(_window);

		/*sf::Vector2f position;
		position.x = 0; position.y = 0;
		shape.setFillColor(sf::Color::Blue);
		shape.setFillColor(sf::Color(0, 0, 255, 255));
		shape.setPosition(sf::Vector2f(position.x*SIZE, position.y*SIZE));
		_window.draw(shape);

		position.x = W_WINDOW_TITLE - 1; position.y = H_WINDOW_TITLE - 1;
		shape.setFillColor(sf::Color::Green);
		shape.setFillColor(sf::Color(255, 255, 0, 255));
		shape.setPosition(sf::Vector2f(position.x*SIZE, position.y*SIZE));
		_window.draw(shape);*/

		_window.display();
	}

}


Graphics::~Graphics()
{
}

