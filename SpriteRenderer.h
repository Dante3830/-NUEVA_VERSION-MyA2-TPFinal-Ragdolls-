#pragma once
#include "SFML/Graphics.hpp"
#include <box2d/box2d.h>
using namespace sf;
using namespace std;

class SpriteRenderer {
private:
	// Define los objetos
	b2Body* BdyObject;
	Sprite* SprObject;

	// Dimensiones
	b2AABB Dims;

	// Posicion
	b2Vec2 SpritePos;

public:
	// Constructor
	SpriteRenderer(b2Body* _body, Sprite* _sprite);

	// Dibuja los objetos que recibe
	void Draw(RenderWindow* _window);

	// Convierte radianes en grados
	float RadToDeg(float _radians);
};