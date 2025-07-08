#include "Game.h"
#include <tchar.h> 
#include <windows.h> 
using namespace sf;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Game* Juego;
    Juego = new Game(800, 600, "TP - Unidad 4 - MAVII");
    Juego->Loop();

    return 0;
}