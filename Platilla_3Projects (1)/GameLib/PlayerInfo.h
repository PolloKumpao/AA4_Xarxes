#pragma once
#include <list>
#include <SFML\Graphics.hpp>

enum tipo { PERSONAJE, ARMA, SALA };



enum estados {TURNO, WAIT};

enum habitaciones { BILLAR, BIBLIOTECA, COCINA, BAILE, INVERNADERO, COMEDOR, VESTIBULO, SALON, ESTUDIO};

class carta
{
public:
	tipo cartatipo;
	std::string nombre;
	int numero;

	carta(tipo t, std::string s, int n)
	{
		cartatipo = t;
		nombre = s;
		numero = n;
	}
private:

};

class PlayerInfo
{

	
	sf::Vector2i position;
	
	int lives;
public:
	std::string name;
	int id;
	float x;
	float y;
	std::list<carta> mano;
	PlayerInfo();
	~PlayerInfo();
	habitaciones habitacion;
};
