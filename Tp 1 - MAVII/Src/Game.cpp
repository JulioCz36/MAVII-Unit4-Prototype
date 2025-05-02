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

    canon = new Canon(phyWorld, wnd);
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

    Vector2i mousePosPixel = Mouse::getPosition(*wnd);
    Vector2f mouseWorld = wnd->mapPixelToCoords(mousePosPixel);

    b2Vec2 pivot = canon->getPivote();

    float dx = mouseWorld.x - pivot.x;
    float dy = mouseWorld.y - pivot.y;
    float angle = atan2(dy, dx);

    // Actualizar ángulo del cañón
    canon->actualizar(angle);
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

    canon->dibujar();

}


void Game::DoEvents()
{
 
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
                Ragdoll nuevo = canon->disparar(phyWorld, 1550.0f, 5, ragdollsActivos);
                ragdollsActivos.push_back(nuevo);
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