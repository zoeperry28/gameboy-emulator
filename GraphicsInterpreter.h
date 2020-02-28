#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include "MemoryLocations.h"
#include <vector>

using namespace std;

int draw(int argc, char** argv);
void helper(uint8_t * MEMORY_STATUS);
void setGraphicsArray(vector<int> &example);
class Application
{
public:

private:
    SDL_Window* m_window;
    SDL_Renderer* m_window_renderer;
    SDL_Event     m_window_event;
};