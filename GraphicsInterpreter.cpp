#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <iostream>
#include <vector>
#include "MemoryLocations.h"
#include "Registers.h"
#include "GraphicsInterpreter.h"

using namespace std;

uint8_t graphicsARR[48];
int init = 0;
static std::string LOGO_data[] = {
"1100", "0110", "1100", "0000", "0000", "0000", "0000", "0000", "0000", "0001", "1000", "0000",
"1110", "0110", "1100", "0000", "0011", "0000", "0000", "0000", "0000", "0001", "1000", "0000",
"1110", "0110", "0000", "0000", "0111", "1000", "0000", "0000", "0000", "0001", "1000", "0000",
"1101", "0110", "1101", "1011", "0011", "0011", "1100", "1101", "1000", "1111", "1001", "1110",
"1101", "0110", "1101", "1101", "1011", "0110", "0110", "1110", "1101", "1001", "1011", "0011",
"1100", "1110", "1101", "1001", "1011", "0111", "1110", "1100", "1101", "1001", "1011", "0011",
"1100", "1110", "1101", "1001", "1011", "0110", "0000", "1100", "1101", "1001", "1011", "0011",
"1100", "0110", "1101", "1001", "1011", "0011", "1110", "1100", "1100", "1111", "1001", "1110",
};

// CE ED 66 66 CC 0D 00 0B 03 73 00 83 00 0C 00 0D 00 08 11 
// 1F 88 89 00 0E DC CC 6E E6 DD DD D9 99 BB BB 67 63 6E 0E 
// EC CC DD DC 99 9F BB B9 33 3E

void setGraphicsArray(vector<int> &example)
{
    int incr = 0;
    for (int i = 0; i < (sizeof(graphicsARR)-1); i++)
    {
        graphicsARR[i] = example[incr];
        incr++;
  
    }
}

void helper(uint8_t * MEMORY_STATUS)
{
    uint8_t TILE_LOC = VIDEO_RAM;
    int COUNT_X = 0;
    int COUNT_Y = 0;
    int inc = 0;
    std::string test[4][12];
    std::string test_1[4][12];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            test[i][j] = "0";
            test_1[i][j] = "0";
        }
    }

    std::cout << sizeof(graphicsARR);


    for (int i = 0; i < (sizeof(graphicsARR) + 1); i++)
    {

        unsigned int DIG1 = ((graphicsARR[i] >> 4) & 0xFF);
        unsigned int DIG2 = ((graphicsARR[i]) & 0x0F);

        printf("\nHEX: %x, DIG1 : %x, DIG2 : %x", graphicsARR[i], DIG1, DIG2);

        std::string BIN1 = GBA_ConvertHextoBin(DIG1);
        std::string BIN2 = GBA_ConvertHextoBin(DIG2);

        std::cout << "\nRES1 : " << BIN1;
        std::cout << "\nRES2 : " << BIN2;
             
        if (i <= (sizeof(graphicsARR) / 2))
        {
            test[COUNT_Y][COUNT_X] = BIN1;
            COUNT_Y++;
            test[COUNT_Y][COUNT_X] = BIN2;
            COUNT_Y++;
        }
        else if (i > (sizeof(graphicsARR) / 2)) 
        {
            if (init == 0)
            {
                COUNT_Y = 0 ;
                COUNT_X = 0 ;
                init = 1;
            }
            test_1[COUNT_Y][COUNT_X] = BIN1;
            COUNT_Y++;
            test_1[COUNT_Y][COUNT_X] = BIN2;
            COUNT_Y++;
        }
        cout << "\n";

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                cout << " " << test_1[i][j] << " ";
            }
            cout << "\n";
        }

        cout << "\n";

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                cout << " " << test[i][j] << " ";
            }
            cout << "\n";
        }


        if (COUNT_Y % 4 == 0)
        {
            COUNT_X++;
            COUNT_Y = 0;
        }

    }
    exit(420);
}

int draw(int argc, char** argv)
{

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL2 Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        200, 200,
        0);

    if (!window)
    {
        std::cout << "Failed to create window\n";
        return -1;
    }

    SDL_Surface* window_surface = SDL_GetWindowSurface(window);

        
    int y = 0;
    for (int i = 0; i < 200; i++)
    {
        for (int j = 0; j < 50; j++)
        {
            set_pixel(window_surface, i, j, 0xFFFFFF);
        }
    }
    if (!window_surface)
    {
        std::cout << "Failed to get the surface from the window\n";
        return -1;
    }

    SDL_UpdateWindowSurface(window);
    
    
    SDL_Delay(5000);
}
void set_pixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
    Uint32* target_pixel = (Uint32*)surface->pixels + y * surface->pitch +
        x * sizeof * target_pixel;
    *target_pixel = pixel;
}

