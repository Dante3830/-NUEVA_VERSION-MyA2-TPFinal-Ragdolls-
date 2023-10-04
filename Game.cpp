#include "Game.h"

Game::Game(int _width, int _height, string _title) : width(_width), height(_height)
{
	wnd = new RenderWindow(VideoMode(width, height), _title);
	fps = 60;
	frameTime = 1.0f / fps;
	wnd->setFramerateLimit(fps);
	wnd->setVisible(true);

	InitElements();
	InitGame();
}

float Game::DegToRad(float degrees) {
	return degrees * 3.1415 / 180;
}

void Game::InitGame() {
	InitPhysics();
	InitTextures();
	Loop();
}

void Game::InitElements() {
	// Inicia reloj, tiempo y eventos
	clock = new Clock();
	time1 = new Time();
	evt = new Event();

	// Inicia procesador de niveles
	levelProcessor = new LP(wnd, width, height);

	// Para las cajas
	offSetX = 0.0f;
	offSetY = 0.0f;

	level = 1;
	state = 0;
	selectedObject = 1;

	floorBoxes = 0;
	RagdollCount = 0;
	RagdollOn = false;

	// Tiempos
	wait = 0.0f;
	time2 = 0.0f;


	SetZoom({ 19.f,19.f }, { 12.f, 12.f }, { 33.f, 77.f });
}

void Game::SetZoom(Vector2f _Focus, Vector2f _Size, Vector2f _Position) {
	// Inicia la cámara
	camera = new View({_Focus},{_Size});
	// Posicion de la cámara
	camera->move({_Position});
	// Asignamos la cámara
	wnd->setView(*camera);
}

void Game::InitPhysics() {
	// Inicializamos el mundo con la gravedad por defecto
	World = new b2World(b2Vec2(0.0f, 9.8f));

	// Creamos un piso y lo posicionamos
	BdyGrd_Def.type = b2_staticBody;
	BdyGrd_Def.position = b2Vec2(50.0f, 104.0f);
	Bdy_Ground = World->CreateBody(&BdyGrd_Def);

	b2PolygonShape shp_grnd;
	shp_grnd.SetAsBox(50.f, 2.5f);

	FixGrd_Def.shape = &shp_grnd;
	FixGrd_Def.density = 1.0f;
	FixGrd_Def.restitution = 0.0f;
	FixGrd_Def.friction = 0.3f;

	Fix_Ground = Bdy_Ground->CreateFixture(&FixGrd_Def);

	// Creando el cañon
	BdyCnyn_Def.type = b2_staticBody;
	BdyCnyn_Def.position = b2Vec2(47.f, 100.f);
	Bdy_Canyon = World->CreateBody(&BdyCnyn_Def);

	b2PolygonShape shp_cnyn;
	shp_cnyn.SetAsBox(0.8f, 0.8f);

	FixCnyn_Def.shape = &shp_cnyn;
	FixCnyn_Def.density = 1.f;
	FixCnyn_Def.restitution = 0.1f;
	FixCnyn_Def.friction = 0.3f;

	Fix_Canyon = Bdy_Canyon->CreateFixture(&FixCnyn_Def);


	// Caja de victoria
	/*BdyVicBox_Def.type = b2_dynamicBody;
		BdyVicBox_Def.position = b2Vec2(53.f, 98.f);
		Bdy_VictoryBox = World->CreateBody(&BdyVicBox_Def);

		b2PolygonShape shp_VicBox;
		shp_VicBox.SetAsBox(0.5f, 0.5f);

		FixVicBox_Def.shape = &shp_VicBox;
		FixVicBox_Def.density = 1.f;
		FixVicBox_Def.restitution = 0.1f;
		FixVicBox_Def.friction = 0.1f;

		Fix_VictoryBox = Bdy_VictoryBox->CreateFixture(&FixVicBox_Def);

		// Caja fija
		bdyStpBox_def.type = b2_staticBody;
		bdyStpBox_def.position = b2Vec2(53.f, 99.f);
		bdy_stoppedBox = World->CreateBody(&bdyStpBox_def);

		b2PolygonShape shp_StpBox;
		shp_StpBox.SetAsBox(0.5f, 0.5f);

		fixStpBox_def.shape = &shp_StpBox;
		fixStpBox_def.density = 1.f;
		fixStpBox_def.restitution = 0.0f;
		fixStpBox_def.friction = 0.3f;

		fix_stpBox = bdy_stoppedBox->CreateFixture(&fixStpBox_def);*/

	//Inicializa caja fija
	bdyDef_cajaFija.type = b2_staticBody;
	bdyDef_cajaFija.position = b2Vec2(StoppedBox_X + offsetX_cajas, StoppedBox_Y + offsetY_cajas);
	bdy_cajaFija = World->CreateBody(&bdyDef_cajaFija);

	b2PolygonShape shape_cajaFija;
	shape_cajaFija.SetAsBox(0.4f, 0.4f);

	fixDef_cajaFija.shape = &shape_cajaFija;
	fixDef_cajaFija.density = 1.f;
	fixDef_cajaFija.restitution = 0.0f;
	fixDef_cajaFija.friction = 0.3f;

	fix_cajaFija = bdy_cajaFija->CreateFixture(&fixDef_cajaFija);

	//Inicializa caja movil
	float coordenadaX_cajaMovil = StoppedBox_X - 0.3f;
	float coordenadaY_cajaMovil = StoppedBox_Y - 0.45f;
	bdyDef_cajaMovil.type = b2_dynamicBody;
	bdyDef_cajaMovil.position = b2Vec2(coordenadaX_cajaMovil + offsetX_cajas, coordenadaY_cajaMovil + offsetY_cajas);
	bdy_cajaMovil = World->CreateBody(&bdyDef_cajaMovil);

	b2PolygonShape shape_cajaMovil;
	shape_cajaMovil.SetAsBox(0.5f, 0.5f);
	
	fixDef_cajaMovil.shape = &shape_cajaMovil;
	fixDef_cajaMovil.density = 1.f;
	fixDef_cajaMovil.restitution = 0.0f;
	fixDef_cajaMovil.friction = 0.3f;

	fix_cajaMovil = bdy_cajaMovil->CreateFixture(&fixDef_cajaMovil);


	//cambia la posición de las cajas en X e Y para que queden en escalera
	offsetX_cajas += 1.5f;
	offsetY_cajas -= 0.21f;
	

}

void Game::InitTextures() {
	// Texturizando en cañón
	Canyon_Tex = new Texture;
	Canyon_Tex->loadFromFile("Assets/Canyon.png");
	Canyon_Spr = new Sprite;
	Canyon_Spr->setTexture(*Canyon_Tex);

	Fig_Canyon = new RectangleShape(Vector2f(0.8f, 0.8f));

	_Canyon = new SpriteRenderer(Bdy_Canyon, Canyon_Spr);

	// Texturizando el piso
	Ground_Tex = new Texture;
	Ground_Tex->loadFromFile("Assets/Ground.png");
	Ground_Spr = new Sprite;
	Ground_Spr->setTexture(*Ground_Tex);

	Fig_Ground = new RectangleShape(Vector2f(50.f, 2.5f));

	_Ground = new SpriteRenderer(Bdy_Ground, Ground_Spr);

	// Texturizando la caja de victoria
	txt_cajaFija = new Texture;
	txt_cajaFija->loadFromFile("Assets/StoppedBox.png");
	spr_cajaFija = new Sprite;
	spr_cajaFija->setTexture(*txt_cajaFija);

	fig_cajaFija = new RectangleShape;

	actorSprite_cajaFija = new SpriteRenderer(bdy_cajaFija, spr_cajaFija);

	// Texturizando caja fija
	txt_cajaMovil = new Texture;
	txt_cajaMovil->loadFromFile("Assets/VictoryBox.png");
	spr_cajaMovil = new Sprite;
	spr_cajaMovil->setTexture(*txt_cajaMovil);

	fig_cajaMovil = new RectangleShape;

	actorSprite_cajaMovil = new SpriteRenderer(bdy_cajaMovil, spr_cajaMovil);
	

}

void Game::SelectPlay() {
	if (selectedObject == 2) {
		selectedObject = (selectedObject - 1);
		levelProcessor->MoveUpMenu();
	}
	else if (selectedObject == 1) {
		levelProcessor->MoveUpMenu();
	}
}

void Game::SelectExit() {
	if (selectedObject == 1) {
		selectedObject = (selectedObject + 1);
		levelProcessor->MoveDownMenu();
	}
	else if (selectedObject == 2) {
		levelProcessor->MoveDownMenu();
	}
}

void Game::UpdatePhysics() {
	World->Step(frameTime, 8, 8);
	World->ClearForces();
	World->DebugDraw();
}

void Game::UpdateCanyon() {
	Vector2i mousePos;
	Vector2f mousePosCoord;
	mousePos = Mouse::getPosition(*wnd);
	mousePosCoord = wnd->mapPixelToCoords(mousePos);

	Bdy_Canyon->SetTransform(Bdy_Canyon->GetPosition(), 
		atan2f(mousePosCoord.y - Bdy_Canyon->GetPosition().y, 
		 mousePosCoord.x - Bdy_Canyon->GetPosition().x));
}

void Game::UpdateRagdoll() {
	Vector2i mousePos;
	Vector2f mousePosCoord;
	mousePos = Mouse::getPosition(*wnd);
	mousePosCoord = wnd->mapPixelToCoords(mousePos);

	if (level > 0 && RagdollCount < 5) {
		RagdollOn = true;

		if (RagdollCount < 5) {
			RagdollNumber[RagdollCount] = new Ragdoll({ Bdy_Canyon->GetPosition().x + 0.5f, Bdy_Canyon->GetPosition().y - 2.f }, *World);
			RagdollNumber[RagdollCount]->ApplyForce({ mousePosCoord.x - Bdy_Canyon->GetPosition().x, 
													  mousePosCoord.y - Bdy_Canyon->GetPosition().y });
			RagdollCount++;
		}

	}
}

void Game::DoEvents() {
	
	while (wnd->pollEvent(*evt)) {
		switch (evt->type) {
		// Cierra la ventana
		case Event::Closed:
			wnd->close();
			break;

		// Si el cursor se mueve, la posicion del canon cambia
		case Event::MouseMoved:
			UpdateCanyon();
			break;

		// Si se hace clic, dispara ragdoll
		case Event::MouseButtonPressed:
			UpdateRagdoll();
			break;

		case Event::KeyPressed:
			if (evt->key.code == Keyboard::Up) {
				SelectPlay();
			}
			else if (evt->key.code == Keyboard::Down) {
				SelectExit();
			}
			else if (evt->key.code == Keyboard::Enter) {

				// Seleccionar opción actual
				switch (state)
				{
					// Inicio del juego (menú principal)
				case 0:

					if (selectedObject == 1) {
						state = 1;
						printf("Ganar Nivel");
					}
					else if (selectedObject == 2) {
						state = 2;
					}
					else {
						state = 0;
					}
					break;

					// Ganar nivel
				case 3:

					if (selectedObject == 1) {
						state = 1;
						level++;
						printf("Ganar Nivel");
					}
					else if (selectedObject == 2) {
						state = 6;
					}
					else {
						state = 0;
					}
					break;

				// Perder nivel
				case 4:

					if (selectedObject == 1) {
						state = 1;
					}
					else if (selectedObject == 2) {
						state = 6;
					}
					else {
						state = 0;
					}
					break;
				}

			}
			//break;
		}

	}
}

void Game::Loop() {
	while (wnd->isOpen()) {
		// Mientras la ventana esté abierta, 
		// se realizará el siguiente recorrido:
		
		DoEvents();

		DrawTheGame();
		
		GameState();
	}
}

void Game::GameState() {
	if (state == 1) {
		*time1 = clock->getElapsedTime();
		if (time2 + frameTime < time1->asSeconds()) {
			time2 = time1->asSeconds();

			UpdatePhysics();

			CheckCollisions();

			CheckWinningCondition();
		}
	}

	if (state == 6) {
		wnd->close();
		new Game(800, 600, "TP Final - MyA2 - Dante Rizzi");
	}
}

void Game::CheckCollisions() {


		FloatRect victoryBound = spr_cajaMovil->getGlobalBounds();
		FloatRect stoppedBound = spr_cajaFija->getGlobalBounds();
		FloatRect groundBound = Ground_Spr->getGlobalBounds();

		if (victoryBound.intersects(stoppedBound)) {
			floorBoxes = 0;
		}
		else if (victoryBound.intersects(groundBound)) {
			floorBoxes = 1;
		}
		else if (!victoryBound.intersects(stoppedBound)) {
			floorBoxes = 1;
		}


}

void Game::CheckWinningCondition() {
	if (floorBoxes == 1) {
		level++;
		state = 3;
		printf("Ganar Nivel");

		if (level > 5) {
			state = 5;
		}
		else {
			RagdollCount = 0;
			floorBoxes = 0;
			offSetX = 0;
			offSetY = 0;
			InitGame();
		}
	}

	if (floorBoxes < 1 && RagdollCount == 5)
	{
		/*
			si el ragdoll4 no es nulo => verdadero
				if (ragdoll4 intercepto con el piso) o (ragdoll4 interpecto a otro ragdoll) o (ragdoll4 salió fuera de pantalla) => verdadero
					gameState = 4 (perdiste, repetir el nivel)
					modificar variables
					Iniciar el jeugo
		*/

		if (ragdolls[4] != NULL)
		{
			Vector2f positionCoordRagdoll = Vector2f(ragdolls[4]->GetPosition(4));
			Vector2i positionPixelRagdoll = wnd->mapCoordsToPixel(positionCoordRagdoll);

			FloatRect boxRagdoll4 = ragdolls[4]->GetRagdollBounds(4);
			FloatRect box_suelo = Ground_Spr->getGlobalBounds();

			bool Ragdoll4SobreOtroRagdoll = false;

			cout << positionPixelRagdoll.x << " " << positionPixelRagdoll.y << endl;

			if (positionPixelRagdoll.x < 0.f || positionPixelRagdoll.x > 800.f || positionPixelRagdoll.y < 0.f)
			{
				cout << "ragdoll fuera de pantalla" << endl;
			}
			if (boxRagdoll4.intersects(box_suelo))
			{
				cout << "ragdoll en el suelo" << endl;
			}

			/*for (int i = 0; i < RagdollCount; i++)
			{*/
			Ragdoll4SobreOtroRagdoll = boxRagdoll4.intersects(ragdolls[0]->GetRagdollBounds(0));
			if (Ragdoll4SobreOtroRagdoll)
			{
				cout << "ragdoll arriba de otro " << 0 << endl;
			}
			//}

			/*
			gameState = 4;
			for (int i = 0; i < RagdollCount; i++)
			{
				ragdoll[i] = NULL;
			}

			FloorBoxes = 0;
			RagdollCount = 0;
			offsetX_cajas = 0;
			offsetY_cajas = 0;
			InitGame();*/

		}
	}
}

void Game::DrawTheGame() {
	wnd->clear();
	
	// Dibuja la escena
	levelProcessor->Draw(wnd, state, level, RagdollCount);

	// Cuando comienza el juego
	if (state == 1) {

		// Dibuja la escena
		levelProcessor->Draw(wnd, state, level, RagdollCount);

		_Ground->Draw(wnd);
		_Canyon->Draw(wnd);

		actorSprite_cajaFija->Draw(wnd);
		actorSprite_cajaMovil->Draw(wnd);

		for (int i = 0; i < RagdollCount; i++) {
			if (!RagdollNumber[i] || RagdollOn) {
				RagdollNumber[i]->Draw(wnd);
			}
		}
	}

	wnd->display();
}