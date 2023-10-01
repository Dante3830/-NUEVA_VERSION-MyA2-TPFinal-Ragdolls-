#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(b2Body* _body, Sprite* _sprite) : BdyObject(_body), SprObject(_sprite)
{
	SpritePos = BdyObject->GetPosition();

	// Dimensiones maximas
	Dims.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);
	Dims.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);

	for (b2Fixture* f = BdyObject->GetFixtureList(); f; f = f->GetNext()) {
		Dims = f->GetAABB(0);
	}

	SprObject->setOrigin(SprObject->getTexture()->getSize().x / 2.0f, SprObject->getTexture()->getSize().y / 2.0f);
	SprObject->setPosition(SpritePos.x, SpritePos.y);
	SprObject->setRotation(RadToDeg(BdyObject->GetAngle()));
	SprObject->setScale(Dims.GetExtents().x * 2.0f / SprObject->getTexture()->getSize().x, Dims.GetExtents().y * 2.0f / SprObject->getTexture()->getSize().y);
}

void SpriteRenderer::Draw(RenderWindow* _window) {
	SpritePos = BdyObject->GetPosition();

	SprObject->setPosition(SpritePos.x, SpritePos.y);
	SprObject->setRotation(RadToDeg(BdyObject->GetAngle()));

	_window->draw(*SprObject);
}

float SpriteRenderer::RadToDeg(float _radians) {
	return _radians * 180 / 3.14f;
}