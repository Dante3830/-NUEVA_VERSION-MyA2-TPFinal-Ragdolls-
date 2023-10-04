#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <box2d/box2d.h>
#include <list>
#include "Ragdoll.h"
#include "LP.h"
#include "SpriteRenderer.h"
using namespace sf;
using namespace std;

// Posición en X e Y de la caja fija
const float StoppedBox_X = 50.5f;
const float StoppedBox_Y = 99.f;

class Game {
private:
	// Propiedades de la ventana
	RenderWindow* wnd;
	Color clearColor;
	int width;
	int height;

	float offsetX_cajas;		//offset de las cajas, se alejan en x según incrementan el nivel
	float offsetY_cajas;		//offset de las cajas, se alejan en Y según incrementan el nivel
	float tiempoEsperaNivel;

	// Mundo del juego
	b2World* World;

	// Procesan los eventos, así como la cámara
	Event* evt;
	View* camera;
	Clock* clock;
	Time* time1;

	// Tiempo de frame
	float frameTime;
	float time2;
	float wait;
	int fps;

	int level;
	int state;
	LP* levelProcessor;
	int selectedObject;

	float offSetX;
	float offSetY;

	// Creando los ragdolls
	Ragdoll* ragdolls[6];

	int RagdollCount;

	// Suelo
	// Creando el suelo
	b2Body* Bdy_Ground;
	b2BodyDef BdyGrd_Def;
	b2Fixture* Fix_Ground;
	b2FixtureDef FixGrd_Def;
	// Forma del suelo
	RectangleShape* Fig_Ground;
	// Estableciendo suelo
	Ragdoll* Ground;
	// Textura y sprite
	Texture* Ground_Tex;
	Sprite* Ground_Spr;
	SpriteRenderer* _Ground;

	// Cañón
	// Creando el cañón
	b2Body* Bdy_Canyon;
	b2BodyDef BdyCnyn_Def;
	b2Fixture* Fix_Canyon;
	b2FixtureDef FixCnyn_Def;
	// Forma del cañón
	RectangleShape* Fig_Canyon;
	// Estableciendo cañón
	Ragdoll* Canyon;
	// Textura y sprite
	Texture* Canyon_Tex;
	Sprite* Canyon_Spr;
	SpriteRenderer* _Canyon;


	// Creando la caja de victoria
	/*b2Body* Bdy_VictoryBox;
	b2BodyDef BdyVicBox_Def;
	b2Fixture* Fix_VictoryBox;
	b2FixtureDef FixVicBox_Def;
	// Forma de las cajas fijas
	Object* VictoryBox;
	// Estableciendo cajas fijas
	RectangleShape* Fig_VictoryBox;
	// Textura y sprite
	Texture* VictoryBox_Tex;
	Sprite* VictoryBox_Spr;
	SpriteRenderer* _VictoryBox;
	

	// CAJAS FIJAS
	// Creando la caja fija
	b2Body* bdy_stoppedBox;
	b2BodyDef bdyStpBox_def;
	b2Fixture* fix_stpBox;
	b2FixtureDef fixStpBox_def;
	// Forma de las caja fija
	Object* StoppedBox;
	// Estableciendo caja fija
	RectangleShape* Fig_StoppedBox;
	// Textura y sprite
	Texture* StoppedBox_Tex;
	Sprite* StoppedBox_Spr;
	SpriteRenderer* _StoppedBox;*/

	// CAJAS FIJAS
	//Identidad de caja fija
	Texture* txt_cajaFija;
	Sprite* spr_cajaFija;
	b2Body* bdy_cajaFija;
	b2BodyDef bdyDef_cajaFija;
	b2Fixture* fix_cajaFija;
	b2FixtureDef fixDef_cajaFija;
	Ragdoll* actor_cajaFija;
	RectangleShape* fig_cajaFija;
	SpriteRenderer* actorSprite_cajaFija;

	// CAJAS DINÁMICAS
	//Identidad de caja móvil
	Texture* txt_cajaMovil;
	Sprite* spr_cajaMovil;
	b2Body* bdy_cajaMovil;
	b2BodyDef bdyDef_cajaMovil;
	b2Fixture* fix_cajaMovil;
	b2FixtureDef fixDef_cajaMovil;
	Ragdoll* actor_cajaMovil;
	RectangleShape* fig_cajaMovil;
	SpriteRenderer* actorSprite_cajaMovil;

	// Instancia resortes para caja móvil
	//b2DistanceJoint* Jnt_StoppedBox;
	//b2DistanceJointDef Jnt_StoppedBoxDef;
	// Cantidad de cajas en el piso
	int floorBoxes;


	// Activa si hay ragdoll o no
	bool RagdollOn;

	// Ragdolls
	Ragdoll* RagdollNumber[5];

	//bool Collission;

public:
	// Constructor
	Game(int _width, int _height, string _title);

	// Inicia el juego
	void InitGame();

	// Inicia los elementos del juego
	void InitElements();

	// Setea la cámara
	void SetZoom(Vector2f _Focus, Vector2f _Size, Vector2f _Position);

	// Inicia las físicas del juego
	void InitPhysics();

	// Inicia las texturas de los objetos
	void InitTextures();

	// Loop principal del juego
	void Loop();

	// Controla la variable 
	void GameState();

	// Procesa cosas del menú
	void CheckWinningCondition();

	// Procesa los eventos externos
	void DoEvents();

	// Controla movimiento del arma
	void UpdateCanyon();

	// Controla cantidad de ragdolls disparados
	void UpdateRagdoll();

	// Actualiza las físicas
	void UpdatePhysics();

	// Mueve el selector hacia arriba
	void SelectPlay();

	// Mueve el selector hacia abajo
	void SelectExit();

	// Chequea las colisiones entre objetos
	void CheckCollisions();

	// Dibujar elementos del juego
	void DrawTheGame();

	// Convierte los grados en radianes
	float DegToRad(float degrees);
};