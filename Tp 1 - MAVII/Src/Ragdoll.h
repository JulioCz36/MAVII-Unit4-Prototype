#ifndef RAGDOLL_H
#define RAGDOLL_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Box2DHelper.h"
#include <Box2D/Box2D.h>
#include <vector>

using namespace sf;
class Ragdoll{
	std::vector<b2Body*> partes;
	b2Vec2 RotarYDesplazar(b2Vec2 base, b2Vec2 offset, float angulo);
    void ConectarDistance(b2World* world, b2Body* bodyA, b2Body* bodyB, b2Vec2 offsetA, b2Vec2 offsetB, float angulo);
public:
	Ragdoll(b2World* world, b2Vec2 posicion, float angulo, b2Vec2 impulsoInicial);
	const std::vector<b2Body*>& getPartes() const;
};

#endif