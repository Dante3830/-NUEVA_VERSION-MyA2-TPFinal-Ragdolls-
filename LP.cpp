#include "LP.h"

LP::LP(RenderWindow* _Window, int _Width, int _Height): Width(_Width), Height(_Height)
{
	window = new RenderWindow;
	window = _Window;
	SelectedItem = 1;
	GameState = 0;

	InitTextAndSprites();
}

void LP::InitTextAndSprites() {
	for (int i = 0; i < 7; i++) {
		LevelTex[i] = new Texture;
		LevelSpr[i] = new Sprite;

		LevelTex[i]->loadFromFile("Assets/level" + to_string(i) + ".png");

		LevelSpr[i]->setTexture(*LevelTex[i]);
		LevelSpr[i]->setScale(0.015f, 0.020f);
		LevelSpr[i]->setPosition(46.f, 90.f);
	}

	font = new Font;
	font->loadFromFile("Assets/ParadeGroovy.ttf");

	Main[0] = new Text;
	Main[0]->setFont(*font);
	Main[0]->setScale(0.015f, 0.020f);
	Main[0]->setCharacterSize(60);
	Main[0]->setPosition(Vector2f(51.0f, 95.0f));
	Main[0]->setFillColor(Color::Red);

	Main[1] = new Text;
	Main[1]->setFont(*font);
	Main[1]->setScale(0.015f, 0.020f);
	Main[1]->setCharacterSize(60);
	Main[1]->setPosition(Vector2f(51.0f, 97.0f));
	Main[1]->setFillColor(Color::Black);

	Ragdolls = new Text;
	Ragdolls->setFont(*font);
	Ragdolls->setScale(0.015f, 0.020f);
	Ragdolls->setCharacterSize(30);
	Ragdolls->setPosition(Vector2f(46.5f, 90.0f));
	Ragdolls->setFillColor(Color::Black);

	Level = new Text;
	Level->setFont(*font);
	Level->setScale(0.015f, 0.020f);
	Level->setCharacterSize(30);
	Level->setPosition(Vector2f(56.0f, 90.0f));
	Level->setFillColor(Color::Red);

	Message = new Text;
	Message->setFont(*font);
	Message->setScale(0.015f, 0.020f);
	Message->setCharacterSize(80);
	Message->setPosition(Vector2f(50.0f, 93.0f));
	
	Exit = new Text;
	Exit->setFont(*font);
	Exit->setScale(0.015f, 0.020f);
	Exit->setCharacterSize(30);
	Exit->setPosition(Vector2f(50.0f, 96.0f));

}

void LP::MoveUpMenu() {
	Main[0]->setFillColor(Color::Red);
	Main[1]->setFillColor(Color::Black);
}

void LP::MoveDownMenu() {
	Main[0]->setFillColor(Color::Black);
	Main[1]->setFillColor(Color::Red);
}

void LP::Draw(RenderWindow* _Window, int _State, int _Level, int _Ragdolls) {

	switch (_State) {
	// Menú principal
	case 0:
		Main[0]->setString("Play");
		Main[1]->setString("Exit");

		_Window->draw(*LevelSpr[0]);
		_Window->draw(*Main[0]);
		_Window->draw(*Main[1]);
		break;

	// Menú de salida
	case 2:
		Exit->setFillColor(Color::Black);
		Exit->setString("Goodbye");

		_Window->draw(*LevelSpr[0]);
		_Window->draw(*Exit);
		break;
	
	// Victoria del nivel
	case 3:
		Message->setFillColor(Color::Yellow);
		Message->setString("LEVEL CLEAR");
		Main[0]->setString("Next Level");
		Main[1]->setString("Reset game");

		_Window->draw(*LevelSpr[_Level]);
		_Window->draw(*Message);
		_Window->draw(*Main[0]);
		_Window->draw(*Main[1]);
		break;

	// Derrota del nivel
	case 4:
		Message->setFillColor(Color::Yellow);
		Message->setString("LEVEL LOST");
		Main[0]->setString("Try again");
		Main[1]->setString("Reset game");

		_Window->draw(*LevelSpr[_Level]);
		_Window->draw(*Message);
		_Window->draw(*Main[0]);
		_Window->draw(*Main[1]);
		break;

	// Victoria del juego
	case 5:
		Message->setString("YOU WIN!!!");
		Message->setFillColor(Color::Green);
		Message->setCharacterSize(200);
		Message->setPosition(Vector2f(47.0f, 93.0f));
		_Window->draw(*LevelSpr[6]);
		_Window->draw(*Message);
		break;
	}

	if (_State == 1 && _Level <= 5) {
		_Window->draw(*LevelSpr[_Level]);

		Ragdolls->setString("Lasting Ragdolls: " + to_string(5 - _Ragdolls));
		_Window->draw(*Ragdolls);

		if (_Level == 5) {
			Level->setString("Level " + to_string(_Level - 2));
		}
		else if (_Level == 3) {
			Level->setString("Level " + to_string(_Level - 1));
		}
		else {
			Level->setString("Level " + to_string(_Level));
		}

		_Window->draw(*Level);
	}
}