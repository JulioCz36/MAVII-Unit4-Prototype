#include "Canon.h"

Canon::Canon(b2World* world, sf::RenderWindow* wnd) : window(wnd) {
    body = Box2DHelper::CreateRectangularKinematicBody(world, 12, 4);

    base.setSize(sf::Vector2f(8.f, 2.f));
    base.setFillColor(sf::Color(128, 128, 128));
    base.setPosition(2.5f, 95.5f);

    pie.setSize(sf::Vector2f(1.5f, 7.f));
    pie.setFillColor(sf::Color(128, 128, 128));
    float pieX = base.getPosition().x + (base.getSize().x / 2.f) - (pie.getSize().x / 2.f);
    float pieY = base.getPosition().y - pie.getSize().y;
    pie.setPosition(pieX, pieY);

    pivote = b2Vec2(pie.getPosition().x + pie.getSize().x / 2.f, pie.getPosition().y);
}

void Canon::actualizar(float angle){
    b2Vec2 pivotPos(getPivote());
    b2Vec2 offsetCanon(cos(angle) * 4.f, sin(angle) * 4.f);
    body->SetTransform(pivotPos + offsetCanon, angle);
}

void Canon::dibujar(){
    window->draw(base);
    window->draw(pie);
}


Ragdoll Canon::disparar(b2World* world, float fuerzaMax, size_t maxRagdolls, std::deque<Ragdoll>& listaRagdolls) {
    float angle = getAngulo();

    b2Vec2 mouse = b2Vec2(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);
    sf::Vector2f worldMouse = window->mapPixelToCoords(sf::Vector2i(mouse.x, mouse.y));
    b2Vec2 worldTarget(worldMouse.x, worldMouse.y);

    float dx = worldTarget.x - pivote.x;
    float dy = worldTarget.y - pivote.y;
    float distancia = sqrt(dx * dx + dy * dy);
    float fuerza = std::min(distancia * 12.0f, fuerzaMax);

    b2Vec2 direccion(cos(angle) * fuerza, sin(angle) * fuerza);
    b2Vec2 spawn = getPuntoDisparo(4.0f);

    if (listaRagdolls.size() >= maxRagdolls) {
        for (b2Body* body : listaRagdolls.front().getPartes()) {
            world->DestroyBody(body);
        }
        listaRagdolls.pop_front();
    }

    return Ragdoll(world, spawn, angle, direccion);
}

b2Vec2 Canon::getPivote() const{
    return pivote;
}

b2Vec2 Canon::getPuntoDisparo(float offset) const{
    float angle = getAngulo();
    b2Vec2 pos = body->GetPosition();
    return pos + b2Vec2(cos(angle) * offset, sin(angle) * offset);
}

float Canon::getAngulo() const
{
    return body->GetAngle();
}

