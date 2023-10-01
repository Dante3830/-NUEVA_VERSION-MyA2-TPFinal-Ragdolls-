#include "Ragdoll.h"

Ragdoll::Ragdoll(Vector2f pos, b2World& World) {

	b2PolygonShape shp_rag[6];

	shp_rag[0].SetAsBox(0.1, 0.1);
	shp_rag[1].SetAsBox(0.2, 0.4);
	shp_rag[2].SetAsBox(0.07, 0.3);
	shp_rag[3].SetAsBox(0.07, 0.3);
	shp_rag[4].SetAsBox(0.07, 0.4);
	shp_rag[5].SetAsBox(0.07, 0.4);

	//x = 50.0, y = 90.0

	//Posicionando el ragdoll
	// Cabeza
	BdyDef_Rag[0].position = b2Vec2(pos.x, pos.y + 0.1f);
	// Torso
	BdyDef_Rag[1].position = b2Vec2(pos.x, pos.y + 0.65f);
	// Brazo Izq
	BdyDef_Rag[2].position = b2Vec2(pos.x - 0.32f, pos.y + 0.5f);
	// Brazo Der
	BdyDef_Rag[3].position = b2Vec2(pos.x + 0.32f, pos.y + 0.5f);
	// Pierna Izq
	BdyDef_Rag[4].position = b2Vec2(pos.x - 0.09f, pos.y + 1.5f); 
	// Pierna Der
	BdyDef_Rag[5].position = b2Vec2(pos.x + 0.09f, pos.y + 1.5f); 

	//Definimos al radgoll
	for (int i = 0; i < 6; i++) {
		BdyDef_Rag[i].type = b2_dynamicBody;
		Bdy_Rag[i] = World.CreateBody(&BdyDef_Rag[i]);

		FixDef_Rag[i].shape = &shp_rag[i];
		FixDef_Rag[i].density = 0.1f;
		FixDef_Rag[i].friction = 0.5f;
		FixDef_Rag[i].restitution = 0.1f;

		Fix_Rag[i] = Bdy_Rag[i]->CreateFixture(&FixDef_Rag[i]);
	}

	// Definimos sus resortes

	// 1: Cabeza + Torso
	JointRagDef[0].Initialize(Bdy_Rag[0], Bdy_Rag[1], b2Vec2(Bdy_Rag[0]->GetPosition().x, Bdy_Rag[0]->GetPosition().y + 0.08),
		b2Vec2(Bdy_Rag[1]->GetPosition().x, Bdy_Rag[1]->GetPosition().y - 0.38));
	// 2: Brazo Der + Torso
	JointRagDef[1].Initialize(Bdy_Rag[1], Bdy_Rag[2], b2Vec2(Bdy_Rag[1]->GetPosition().x + 0.18, Bdy_Rag[1]->GetPosition().y - 0.38),
		b2Vec2(Bdy_Rag[2]->GetPosition().x - 0.05, Bdy_Rag[2]->GetPosition().y - 0.28));
	// 3: : Brazo Izq + Torso
	JointRagDef[2].Initialize(Bdy_Rag[1], Bdy_Rag[3], b2Vec2(Bdy_Rag[1]->GetPosition().x - 0.18, Bdy_Rag[1]->GetPosition().y - 0.38),
		b2Vec2(Bdy_Rag[3]->GetPosition().x + 0.05, Bdy_Rag[3]->GetPosition().y - 0.28));
	// 4: Pierna Der + Torso
	JointRagDef[3].Initialize(Bdy_Rag[1], Bdy_Rag[4], b2Vec2(Bdy_Rag[1]->GetPosition().x + 0.09, Bdy_Rag[1]->GetPosition().y + 0.38),
		b2Vec2(Bdy_Rag[4]->GetPosition().x, Bdy_Rag[4]->GetPosition().y - 0.38));
	// 5: Pierna Izq + Torso
	JointRagDef[4].Initialize(Bdy_Rag[1], Bdy_Rag[5], b2Vec2(Bdy_Rag[1]->GetPosition().x + 0.09, Bdy_Rag[1]->GetPosition().y + 0.38),
		b2Vec2(Bdy_Rag[5]->GetPosition().x, Bdy_Rag[5]->GetPosition().y - 0.38));

	for (int i = 0; i < 5; i++) {
		JointRagDef[i].damping = 0.3f;
		JointRagDef[i].stiffness = 0.9f;
		JointRagDef[i].collideConnected = true;
		JointRagDef[i].length = 0.00025f;
		JointRag[i] = (b2DistanceJoint*)World.CreateJoint(&JointRagDef[i]);
	}

	for (int i = 0; i < 6; i++) {
		Shape_Rag[i] = new RectangleShape;
	}

	Shape_Rag[0]->setFillColor(Color::Color(0, 200, 0));
	Shape_Rag[1]->setFillColor(Color::Yellow);
	Shape_Rag[2]->setFillColor(Color::Red);
	Shape_Rag[3]->setFillColor(Color::Red);
	Shape_Rag[4]->setFillColor(Color::Blue);
	Shape_Rag[5]->setFillColor(Color::Blue);

	for (int i = 0; i < 6; i++) {
		Ragd[i] = new Object(Bdy_Rag[i], Shape_Rag[i]);
	}

}

void Ragdoll::Draw(RenderWindow* _window) {
	for (int i = 0; i < 6; i++) {
		Ragd[i]->Draw(_window);
	}
}

void Ragdoll::ApplyForce(Vector2f _mousePos) {
	for (int i = 0; i < 6; i++) {
		Bdy_Rag[i]->ApplyForceToCenter(b2Vec2(_mousePos.x * 10, _mousePos.y * 10), false);
	}
}

FloatRect Ragdoll::GetRagdollBounds(int i) {
	FloatRect RagdollBox = Shape_Rag[i]->getGlobalBounds();
	return RagdollBox;
}

Vector2f Ragdoll::GetPosition(int i) {
	Vector2f RagdollPos = Vector2f (Shape_Rag[i]->getPosition());
	return RagdollPos;
}

float Ragdoll::RadToDeg(float radians) {
	return radians * 180 / 3.1415;
}