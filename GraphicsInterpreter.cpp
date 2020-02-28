#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <iostream>
#include <vector>
#include "MemoryLocations.h"
#include "Registers.h"

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
    SDL_Init(SDL_INIT_EVERYTHING);
    atexit(SDL_Quit);

    SDL_Window* window = SDL_CreateWindow
    (
        "SDL2",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        600, 600,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer
    (
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    cout << "Renderer name: " << info.name << endl;
    cout << "Texture formats: " << endl;
    for (Uint32 i = 0; i < info.num_texture_formats; i++)
    {
        cout << SDL_GetPixelFormatName(info.texture_formats[i]) << endl;
    }

    const unsigned int texWidth = 1024;
    const unsigned int texHeight = 1024;
    SDL_Texture* texture = SDL_CreateTexture
    (
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        texWidth, texHeight
    );

    vector< unsigned char > pixels(texWidth * texHeight * 4, 0);

    SDL_Event event;
    bool running = true;
    while (running)
    {
        const Uint64 start = SDL_GetPerformanceCounter();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        while (SDL_PollEvent(&event))
        {
            if ((SDL_QUIT == event.type) ||
                (SDL_KEYDOWN == event.type && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode))
            {
                running = false;
                break;
            }
        }

    std::string myBIN;

    SDL_Rect rect;
    
    rect.x = 0;
    rect.y = 0;
    rect.w = 5;
    rect.h = 5;
    int bitcnt = 0;

    for (int i = 0; i < sizeof(LOGO_data); i++)
    {
        myBIN = LOGO_data[i];
        for (int j = 0; j < myBIN.length(); j++)
        {
            if (myBIN.substr(j, j) == "1")
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            else if (myBIN.substr(j, j) == "0")
            {
                SDL_SetRenderDrawColor(renderer, 0,0, 0, 0);
            }
            
            //SDL_RenderFillRect(m_window_renderer, &rect);
            SDL_RenderDrawPoint(renderer, rect.x, rect.y); //Renders on middle of screen
            bitcnt++;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderPresent(renderer);

            if (bitcnt % (12 *4 )== 1)
            {
                rect.x = 0;
                rect.y++;
            }
            else
            {
                rect.x ++;
            }
        }
    }

        SDL_UpdateTexture
        (
            texture,
            NULL,
            &pixels[0],
            texWidth * 4
        );

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
}
