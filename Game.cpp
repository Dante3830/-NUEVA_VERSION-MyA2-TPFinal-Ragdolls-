#include "Game.h"

Game::Game(int _width, int _height, string _title) : width(_width), height(_height)
{
	wnd = new RenderWindow(VideoMode(width, height), _title);
	fps = 60;
	frameTime = 1.0f / fps;
	wnd->setFramerateLimit(fps);
	wnd->setVisible(true);

	b2PolygonShape shp_rag[6];

	InitGame();
}

float Game::DegToRad(float degrees) {
	return degrees * 3.1415 / 180;
}

void Game::InitGame() {
	InitElements();
	InitPhysics();
	InitTextures();
	Loop();
}

void Game::InitElements() {
	// Inicia reloj, tiempo y eventos
	clock = new Clock;
	time1 = new Time;
	evt = new Event;

	// Inicia procesador de niveles
	levelProcessor = new LP(wnd, width, height);

	// Para las cajas
	offSetX = 0.0f;
	offSetY = 0.0f;

	level = 1;
	state = 0;
	selectedObject = 1;

	//floorBoxes = 0;
	RagdollCount = 0;
	RagdollOn = false;

	// Tiempos
	wait = 0.0f;
	time2 = 0.0f;

	SetZoom({ 19.f,19.f }, { 12.f, 12.f }, { 33.f, 77.f });

	Collission = false;
}

void Game::SetZoom(Vector2f _Center, Vector2f _Size, Vector2f _Position) {
	// Inicia la cámara
	camera = new View({_Center},{_Size});
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
	BdyVicBox_Def.type = b2_dynamicBody;
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

	fix_stpBox = bdy_stoppedBox->CreateFixture(&fixStpBox_def);
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
	VictoryBox_Tex = new Texture;
	VictoryBox_Tex->loadFromFile("Assets/VictoryBox.png");
	VictoryBox_Spr = new Sprite;
	VictoryBox_Spr->setTexture(*VictoryBox_Tex);

	Fig_VictoryBox = new RectangleShape(Vector2f(0.5f, 0.5f));

	_VictoryBox = new SpriteRenderer(Bdy_VictoryBox, VictoryBox_Spr);

	// Texturizando caja fija
	StoppedBox_Tex = new Texture;
	StoppedBox_Tex->loadFromFile("Assets/StoppedBox.png");
	StoppedBox_Spr = new Sprite;
	StoppedBox_Spr->setTexture(*StoppedBox_Tex);

	Fig_StoppedBox = new RectangleShape(Vector2f(0.5f, 0.5f));

	_StoppedBox = new SpriteRenderer(bdy_stoppedBox, StoppedBox_Spr);
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
			RagdollNumber[RagdollCount]->ApplyForce({ mousePosCoord.x - Bdy_Canyon->GetPosition().x, mousePosCoord.y - Bdy_Canyon->GetPosition().y });
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
		
		case Event::KeyPressed:
			//if (Keyboard::isKeyPressed(Keyboard::Left)) {
			//	Bdy_Canyon->SetTransform(Bdy_Canyon->GetPosition(), Bdy_Canyon->GetAngle() + deg2rad(3));
			//}
			//else if (Keyboard::isKeyPressed(Keyboard::Right)) {
			//	Bdy_Canyon->SetTransform(Bdy_Canyon->GetPosition(), Bdy_Canyon->GetAngle() + deg2rad(-3));
			//}
			if (evt->key.code == Keyboard::Up) {
				SelectPlay();
			}
			else if (evt->key.code == Keyboard::Down) {
				SelectExit();
			}
			else if (evt->key.code == Keyboard::Enter) {

				switch (state)
				{
				case 0:

					if (selectedObject == 1) {
						state = 1;
					}
					else if (selectedObject == 2) {
						state = 2;
					}
					else {
						state = 0;
					}
					break;

				case 3:

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
			break;

		// Si el cursor se mueve, la posicion del canon cambia
		case Event::MouseMoved:
			UpdateCanyon();
			break;

		// Si se hace clic, dispara ragdoll
		case Event::MouseButtonPressed:
			UpdateRagdoll();
			break;


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

void Game::CheckWinningCondition() {
	if (floorBoxes == 1) {
		printf("Hay colision X2");
		level++;
		state = 3;

		if (level > 3) {
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
}

void Game::CheckCollisions() {

	/*for (int RagdollIndex = 0; RagdollIndex < RagdollCount; RagdollIndex++) {
		// Bounding box de la caja de victoria
		FloatRect victoryBound = Fig_VictoryBox->getGlobalBounds();

		for (int partIndex = 0; partIndex < 6; partIndex++) {
			const Vector2f ragdollPos = RagdollNumber[RagdollIndex]->GetPosition(partIndex);
		
			const Vector2f ragdollSize = RagdollNumber[RagdollIndex]->GetSize(RagdollIndex);
			
			FloatRect ragdollBounds = FloatRect(ragdollPos, ragdollSize);

			if (victoryBound.intersects(ragdollBounds)) {
				printf("HAY COLISION");
				Collission = true;
			}
			else {
				printf("no hay colision");
				Collission = false;
			}
		}

	}*/

	FloatRect victoryBound = VictoryBox_Spr->getGlobalBounds();
	FloatRect stoppedBound = StoppedBox_Spr->getGlobalBounds();
	FloatRect groundBound = Ground_Spr->getGlobalBounds();

	if (victoryBound.intersects(stoppedBound)) {
		floorBoxes = 0;
	}
	else if (victoryBound.intersects(groundBound)) {
		floorBoxes++;
	}
	else if (!victoryBound.intersects(stoppedBound)) {
		floorBoxes++;
	}
	
}

void Game::DrawTheGame() {
	wnd->clear();
	
	// Dibuja la escena
	levelProcessor->Draw(wnd, state, level, RagdollCount);

	// Cuando comienza el juego
	if (state == 1) {

		_Ground->Draw(wnd);
		_Canyon->Draw(wnd);
		_VictoryBox->Draw(wnd);
		_StoppedBox->Draw(wnd);

		for (int i = 0; i < RagdollCount; i++) {
			if (!RagdollNumber[i] || RagdollOn == true) {
				RagdollNumber[i]->Draw(wnd);
			}
		}
	}

	wnd->display();
}