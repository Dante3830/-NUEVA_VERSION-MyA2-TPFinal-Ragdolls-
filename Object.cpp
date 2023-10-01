#include "Object.h"

Object::Object(b2Body* _body, RectangleShape* _figure): Bdy_Object(_body), Fig_Object(_figure) 
{
	Pos = Bdy_Object->GetPosition();

	Dims.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);
	Dims.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);

	for (b2Fixture* i = Bdy_Object->GetFixtureList(); i; i = i->GetNext()) {
		Dims = i->GetAABB(0);
	}

	Fig_Object->setSize({ Dims.GetExtents().x * 2, Dims.GetExtents().y * 2 });
	Fig_Object->setOrigin(Fig_Object->getSize().x / 2.f, Fig_Object->getSize().y / 2.f);
	Fig_Object->setPosition(Pos.x, Pos.y);
	Fig_Object->setRotation(RadToDeg(Bdy_Object->GetAngle()));
}

void Object::Draw(RenderWindow* window) {
	Pos = Bdy_Object->GetPosition();
	Fig_Object->setPosition(Pos.x, Pos.y);
	Fig_Object->setRotation(RadToDeg(Bdy_Object->GetAngle()));
	window->draw(*Fig_Object);
}

FloatRect Object::BoxObject() {
	FloatRect BoundingBox = Fig_Object->getGlobalBounds();
	return BoundingBox;
}

float Object::RadToDeg(float radians) {
	return radians * 180 / 3.1415f;
}