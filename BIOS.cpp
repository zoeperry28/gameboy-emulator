#include "BIOS.h"
// classes example
#include <iostream>
#include <stdbool.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Logo
{

    uint8_t logo_data_CART = 0x104;
    uint8_t logo_data_BIOS = 0xA8;
    bool logo_status; 
 
    int width, height;

    public:

        bool check_logo(char * BIOS_ROM, char * GAME_ROM)
        {
            /*
             * Compare the rom on the bios with the logo data on the ACTUAL CARTRIDGE
             * if these match, then the value produced is TRUE
             */
            
            if (BIOS_ROM[logo_data_BIOS] == GAME_ROM[logo_data_CART])
            {
                for (int i = 0; i < 29; i++)
                {
                    if (BIOS_ROM[logo_data_BIOS + i] == GAME_ROM[logo_data_CART + i])
                    {
                        logo_status = true;
                    }
                    else
                    {
                        logo_status = false;
                        return logo_status;
                    }
                }

                return logo_status;
            }
            else
            {
                return logo_status;
            }
            

        }

      
        char * get_bios(string BIOSNAME)
        {
            char buffer[100];
            ifstream myFile(BIOSNAME, ios::in | ios::binary);
            myFile.read(buffer, 100);
            if (!myFile) 
            {
                printf("no file found!");
            }
            
            if (!myFile.read(buffer, 100)) 
            {
                printf("no file found!");
            }
            return buffer;

        }

        char * get_rom(string ROMNAME)
        {
            //load the rom 
            char buffer[8000];
            ifstream myFile(ROMNAME, ios::in | ios::binary);
            myFile.read(buffer, 100);
            if (!myFile) 
            {
                printf("no file found!");
            }

            if (!myFile.read(buffer, 100)) 
            {
                printf("no file found!");
            }
            return buffer;
        }
};


int main() 
{
    Logo bios;
    char* rom_file = bios.get_rom("C://Users//zoepe//Downloads//tetris.bin");
    char* bios_file = bios.get_bios("C://Users//zoepe//Downloads//dmg_boot.bin");
    bool x = bios.check_logo(rom_file, bios_file);
    cout << "result: " << x ? "true" : "false";
    return 0;
}