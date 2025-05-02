#ifndef CANON_H
#define CANON_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <deque>
#include "Box2DHelper.h"
#include "Ragdoll.h"

using namespace sf;
class Canon{
    b2Body* body;
    RectangleShape pie, base;
    RenderWindow* window;
    b2Vec2 pivote;

public:
    Canon(b2World* world, sf::RenderWindow* wnd);

    void actualizar(float angle);
    void dibujar();

    float getAngulo() const;
    b2Vec2 getPivote() const;
    b2Vec2 getPuntoDisparo(float offset = 2.0f) const;
    
    Ragdoll disparar(b2World* world, float fuerzaMax, size_t maxRagdolls, std::deque<Ragdoll>& listaRagdolls);
};

#endif