#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

Game::Game(int ancho, int alto, std::string titulo)
{
    // Inicialización de la ventana y configuración de propiedades
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 1.0f / fps;
    SetZoom(); // vista del juego
    InitPhysics(); // motor de física
}

void Game::Loop()
{
    while (wnd->isOpen())
    {
        wnd->clear(clearColor);
        DoEvents(); 
        CheckCollitions(); 
        UpdatePhysics(); 
        DrawGame();
        wnd->display();
    }
}

void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8); // Simular el mundo físico
    phyWorld->ClearForces(); // Limpiar las fuerzas 
    phyWorld->DebugDraw(); // Dibujar el mundo
}

void Game::DrawGame()
{
    // Dibujar el suelo
    RectangleShape suelo(sf::Vector2f(100, 2.5f));
    suelo.setFillColor(sf::Color::Blue);
    suelo.setPosition(0, 97.5f);
    wnd->draw(suelo);

    //Dibujar el techo
    RectangleShape techo(sf::Vector2f(100, 2.5f));
    techo.setFillColor(sf::Color::Blue);
    techo.setPosition(0, 0);
    wnd->draw(techo);

    //Dibujar pared izquierda
    RectangleShape paredIzq(sf::Vector2f(2.5f, 100));
    paredIzq.setFillColor(sf::Color::Blue);
    paredIzq.setPosition(0, 0);
    wnd->draw(paredIzq);

    //Dibujar pared izquierda
    RectangleShape paredDer(sf::Vector2f(2.5f, 100));
    paredDer.setFillColor(sf::Color::Blue);
    paredDer.setPosition(97.5f, 0);
    wnd->draw(paredDer);

    RectangleShape base(Vector2f(8.f, 2.f));
    base.setFillColor(Color(128, 128, 128));

    base.setPosition(2.5f, 95.5f);
    wnd->draw(base);

    pie =  RectangleShape (Vector2f(1.5f, 7.f));
    pie.setFillColor(Color(128, 128, 128));

    float pieX = base.getPosition().x + (base.getSize().x / 2.f) - (pie.getSize().x / 2.f);
    float pieY = base.getPosition().y - pie.getSize().y;
    pie.setPosition(pieX, pieY);

    wnd->draw(pie);

}

//metodos para ahorrar codigo
b2Vec2 RotarYDesplazar(b2Vec2 base, b2Vec2 offset, float angulo) {
    float cosA = cos(angulo);
    float sinA = sin(angulo);
    b2Vec2 rotado(
        offset.x * cosA - offset.y * sinA,
        offset.x * sinA + offset.y * cosA
    );
    return base + rotado;
}

void ConectarDistance(b2World* world, b2Body* bodyA, b2Body* bodyB, b2Vec2 offsetA, b2Vec2 offsetB, float angulo) {
    b2Vec2 anchorA = RotarYDesplazar(bodyA->GetWorldCenter(), offsetA, angulo);
    b2Vec2 anchorB = RotarYDesplazar(bodyB->GetWorldCenter(), offsetB, angulo);

    b2DistanceJointDef jointDef;
    jointDef.bodyA = bodyA;
    jointDef.bodyB = bodyB;
    jointDef.localAnchorA = bodyA->GetLocalPoint(anchorA);
    jointDef.localAnchorB = bodyB->GetLocalPoint(anchorB);
    jointDef.length = (anchorA - anchorB).Length();
    jointDef.collideConnected = true;

    b2LinearStiffness(jointDef.stiffness, jointDef.damping, 3.0f, 0.7f, bodyA, bodyB);

    world->CreateJoint(&jointDef);
}

std::vector<b2Body*> CrearRagdoll(b2World* world, b2Vec2 posicion, float angulo, b2Vec2 fuerzaInicial){
    std::vector<b2Body*> partes;

    // Creo Bodys
    b2Body* torso = Box2DHelper::CreateRectangularDynamicBody(world, 2.5f, 4.0f, 0.2f, 0.2f, 0.0f);
    b2Body* cabeza = Box2DHelper::CreateRectangularDynamicBody(world, 1.5f, 1.5f, 0.3f, 0.2f, 0.0f);
    b2Body* brazoIzq = Box2DHelper::CreateRectangularDynamicBody(world, 1.0f, 3.0f, 0.3f, 0.2f, 0.0f);
    b2Body* brazoDer = Box2DHelper::CreateRectangularDynamicBody(world, 1.0f, 3.0f, 0.3f, 0.2f, 0.0f);
    b2Body* piernaIzq = Box2DHelper::CreateRectangularDynamicBody(world, 1.0f, 5.0f, 0.3f, 0.2f, 0.0f);
    b2Body* piernaDer = Box2DHelper::CreateRectangularDynamicBody(world, 1.0f, 5.0f, 0.3f, 0.2f, 0.0f);

    // Partes relativas al torso
    torso->SetTransform(posicion, angulo);
    cabeza->SetTransform(RotarYDesplazar(posicion, b2Vec2(0, -2.75f), angulo), angulo);
    brazoIzq->SetTransform(RotarYDesplazar(posicion, b2Vec2(-1.75f, -0.5f), angulo), angulo);
    brazoDer->SetTransform(RotarYDesplazar(posicion, b2Vec2(1.75f, -0.5f), angulo), angulo);
    piernaIzq->SetTransform(RotarYDesplazar(posicion, b2Vec2(-1.0f, 4.5f), angulo), angulo);
    piernaDer->SetTransform(RotarYDesplazar(posicion, b2Vec2(1.0f, 4.5f), angulo), angulo);

    // Conectar articulaciones
    ConectarDistance(world, cabeza, torso, b2Vec2(0, 0.75f), b2Vec2(0, -2.0f), angulo);
    ConectarDistance(world, brazoIzq, torso, b2Vec2(0, -1.5f), b2Vec2(-1.25f, -2.0f), angulo);
    ConectarDistance(world, brazoDer, torso, b2Vec2(0, -1.5f), b2Vec2(1.25f, -2.0f), angulo);
    ConectarDistance(world, piernaIzq, torso, b2Vec2(0.0f, -2.5f), b2Vec2(-1.0f, 2.0f), angulo);
    ConectarDistance(world, piernaDer, torso, b2Vec2(0.0f, -2.5f), b2Vec2(1.0f, 2.0f), angulo);

    // Aplicar fuerza
    torso->ApplyLinearImpulseToCenter(fuerzaInicial, true);

    partes = { cabeza, torso, brazoIzq, brazoDer, piernaIzq, piernaDer };

    return partes;
}

void Game::DoEvents()
{
    Vector2i mousePosPixel = Mouse::getPosition(*wnd);
    Vector2f mouseWorld = wnd->mapPixelToCoords(mousePosPixel);

    b2Vec2 pivotPos(pie.getPosition().x + pie.getSize().x / 2.f, pie.getPosition().y);

    float dx = mouseWorld.x - pivotPos.x;
    float dy = mouseWorld.y - pivotPos.y;

    float angle = atan2(dy, dx); 

    b2Vec2 offsetCanon(cos(angle) * 4.f, sin(angle) * 3.0f);
    b2Vec2 posCanon = pivotPos + offsetCanon;

    canonBody->SetTransform(posCanon, angle);

    // Rotar pivot
    b2Vec2 offset(cos(angle) * 6.0f, sin(angle) * 6.0f);

    Vector2f respawnPos = Vector2f(canonBody->GetPosition().x + offset.x, canonBody->GetPosition().y + offset.y);

    Event evt;
    while (wnd->pollEvent(evt))
    {
        switch (evt.type)
        {
        case Event::Closed:
            wnd->close();
            break;
        case Event::MouseButtonPressed:
            if (evt.mouseButton.button == Mouse::Left) {

                float distancia = std::sqrt(dx * dx + dy * dy);
                float fuerza = std::min(distancia * 12.0f, 1550.0f);
                b2Vec2 direccion(std::cos(angle) * fuerza, std::sin(angle) * fuerza);

                // Obtener la posición del spawn para crear el ragdoll
                b2Vec2 offset2(cos(angle) * 2.0f, sin(angle) * 2.0f);
                b2Vec2 spawnPos(respawnPos.x + offset2.x, respawnPos.y + offset2.y);

                //limito los ragdoll (como el juego de referencia)
                if (ragdollsActivos.size() >= 5)
                {
                    for (b2Body* body : ragdollsActivos.front()) {
                        phyWorld->DestroyBody(body);
                    }
                    ragdollsActivos.pop_front();
                }

                // Crear el ragdoll
                std::vector<b2Body*> ragdoll = CrearRagdoll(phyWorld, spawnPos, angle, direccion);
                ragdollsActivos.push_back(ragdoll);

            }
        }
    }    
}

void Game::CheckCollitions() {}

// Configuración de la vista del juego
void Game::SetZoom(){
    View camara;

    camara.setSize(100.0f, 100.0f);
    camara.setCenter(50.0f, 50.0f);
    wnd->setView(camara);
}

void Game::InitPhysics()
{
    // Inicializar el mundo
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    // Crear un renderer de debug para visualizar el mundo físico
    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX);
    phyWorld->SetDebugDraw(debugRender);

    // Crear el suelo 
    sueloBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 2.5);
    sueloBody->SetTransform(b2Vec2(50.0f, 99), 0.0f);

    // Crear el techo
    techoBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 2.5);
    techoBody->SetTransform(b2Vec2(50.0f, 1.0f), 0.0f);
    
    // Crear paredIzq
    paredIzqBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 2.5, 100);
    paredIzqBody->SetTransform(b2Vec2(1.0f, 50.0f), 0.0f);

    // Crear paredDer
    paredDerBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 2.5, 100);
    paredDerBody->SetTransform(b2Vec2(99.0f, 50.0f), 0.0f);
    paredDerBody->SetTransform(b2Vec2(99.0f, 50.0f), 0.0f);

    // Crear canon
    canonBody = Box2DHelper::Box2DHelper::CreateRectangularKinematicBody(phyWorld, 12, 4);

    //objetos para colisionar
    b2Body* piso1 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 15, 2);
    piso1->SetTransform(b2Vec2(50.0f, 30.0f), 0.0f);

    b2Body* piso2 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 25, 2);
    piso2->SetTransform(b2Vec2(70.0f, 40.0f), -45.0f);

    b2Body* piso3 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 2, 2);
    piso3->SetTransform(b2Vec2(37.0f, 40.0f), -45.0f);

    b2Body* bola = Box2DHelper::CreateCircularDynamicBody(phyWorld, 4, 0.5f, 0.3f, 0.2f);
    bola->SetTransform(b2Vec2(31.f, 40.0f), 0.0f);

    b2RevoluteJointDef rjd;
    rjd.bodyA = bola;
    rjd.bodyB = piso3;      
    rjd.localAnchorA.Set(15.f, 15.f); 
    rjd.localAnchorB = piso3->GetLocalCenter();
    rjd.collideConnected = false;

    phyWorld->CreateJoint(&rjd);

    b2Body* barril1 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 2, 0.5f, 0.3f, 0.2f);
    barril1->SetTransform(b2Vec2(46.5f, 28.0f), 0.0f);

    b2Body* barril2 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 2, 0.5f, 0.3f, 0.2f);
    barril2->SetTransform(b2Vec2(53.5f, 28.0f), 0.0f);
}

Game::~Game(void)
{ }