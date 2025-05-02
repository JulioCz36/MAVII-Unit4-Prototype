#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SFMLRenderer.h"
#include <list>
#include <deque>
#include "Canon.h"
#include "Ragdoll.h"

using namespace sf;
class Game{
private:
	// Propiedades de la ventana
	int alto;
	int ancho;
	RenderWindow *wnd;
	Color clearColor;

	// box2d
	b2World *phyWorld;
	SFMLRenderer *debugRender;

	//tiempo de frame
	float frameTime;
	int fps;

	//Bodys
	b2Body* sueloBody;b2Body* techoBody; b2Body* paredIzqBody; b2Body* paredDerBody;

	Canon* canon; 

	std::deque<Ragdoll> ragdollsActivos;

public:

	Game(int ancho, int alto,std::string titulo);
	void CheckCollitions();
	~Game(void);
	void InitPhysics();

	// Main game loop
	void Loop();
	void DrawGame();
	void UpdatePhysics();
	void DoEvents();
	void SetZoom();
};

