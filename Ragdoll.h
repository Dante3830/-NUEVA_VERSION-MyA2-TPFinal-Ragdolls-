#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SFMLRenderer.h"
#include <box2d/box2d.h>
#include "Object.h"
#include <list>
using namespace sf;
using namespace std;

//enum RagdollBodyPart { Head = 0, Torso = 1, LeftArm = 2, RightArm = 3, LeftLeg = 4, RightLeg = 5};

class Ragdoll {
private:
	// Definiendo al ragdoll y sus partes
	RectangleShape* Shape_Rag[6];
	Object* Ragd[6];

	// Definiendo al cuerpo y fixture del ragdoll
	b2Body* Bdy_Rag[6];
	b2BodyDef BdyDef_Rag[6];
	b2Fixture* Fix_Rag[6];
	b2FixtureDef FixDef_Rag[6];

	// Definimos sus resortes
	b2DistanceJoint* JointRag[5];
	b2DistanceJointDef JointRagDef[5];

public:
	// Constructor
	Ragdoll(Vector2f pos, b2World& phyWorld);

	// Dibuja el objeto ragdoll
	void Draw(RenderWindow* wnd);

	// Aplica la fuerza al cañón para disparar
	void ApplyForce(Vector2f _mousePos);

	// Devuelve el Bounding Box del Ragdoll
	FloatRect GetRagdollBounds(int i);

	// Devuelve la posición del ragdoll
	Vector2f GetPosition(int i);

	// Transforma los radianes en grados
	float RadToDeg(float degrees);

};