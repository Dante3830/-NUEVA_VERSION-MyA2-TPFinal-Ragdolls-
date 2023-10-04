// LP = Level Processor (Procesador de niveles)
#pragma once
#include "Box2D/Box2D.h"
#include "SFML/Graphics.hpp"
using namespace sf;
using namespace std;

//Nombre del juego, Play = 1, Exit = 2
#define MENU 2

class LP {
private:
	// Ventana
	RenderWindow* window;

	// Texturas y sprites para los niveles
	Texture* LevelTex[7];
	Sprite* LevelSpr[7];

	// Texto para el menú
	Text* Main[MENU];
	// Texto para cada nivel de juego
	Text* Level;
	// Texto que muestra cantidad de ragdolls
	Text* Ragdolls;
	// Mensaje del juego
	Text* Message;
	// Texto play
	Text* Exit;

	// Fuente de letra a utilizar
	Font* font;

	// Selector del menú
	int SelectedItem;
	// Ancho y alto de la ventana
	int Width;
	int Height;
	// Estado del juego
	int GameState;
	// Elementos
	int Items;

public:

	// Constructor de la clase
	LP(RenderWindow* _window, int _width, int _height);

	// Inicializa las texturas y textos
	void InitTextAndSprites();

	// Mueve el selector hacia arriba
	void MoveUpMenu();

	// Mueve el selector hacia abajo
	void MoveDownMenu();

	// Dibuja las texturas y texto
	void Draw(RenderWindow* _window, int _state, int _level, int _Ragdolls);

};