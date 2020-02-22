#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <memory>


int draw(int argc, char** argv);

class Application
{
public:

private:
    SDL_Window* m_window;
    SDL_Renderer* m_window_renderer;
    SDL_Event     m_window_event;
};