#include "BIOS.h"
#include "Registers.h"
#include "MemoryLocations.h"
// classes example
#include <iostream>
#include <stdbool.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


uint8_t logo_data_CART = 0x104;
uint8_t logo_data_BIOS = 0xA8;
bool logo_status; 
 
int width, height;
uint8_t logo_total = 0u;

bool BIOS_SET::check_logo(char * BIOS_ROM, char * GAME_ROM)
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
                logo_total = GAME_ROM[logo_data_CART + i] + logo_total;
                logo_status = true;
            }
            else
            {
                logo_status = false;
                return logo_status;
            }
        }

        logo_total = logo_total + 25;
        printf("%d", logo_total);
        return logo_status;
    }
    else
    {
        return logo_status;
    }
}
      
char * BIOS_SET::get_bios(string BIOSNAME)
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

char * BIOS_SET::get_rom(string ROMNAME)
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

    for (int i = 0; i < 10; i++)
    {
        printf("\n%x ", buffer[logo_data_BIOS + i]);
    }
    return buffer;
}

int main() 
{
    BIOS_SET _BIOS;
    int i = 0;
    char* rom_file = _BIOS.get_rom("C://Users//zoepe//Downloads//tetris.bin");
    char * bios_file = _BIOS.get_bios("C://Users//zoepe//Downloads//dmg_boot.bin");
    _BIOS.t.BIOS_GRAPHIC = bios_file;
    bool x = _BIOS.check_logo(rom_file, _BIOS.t.BIOS_GRAPHIC);

    cout << "result: " << x ? "true" : "false";
    
    if (logo_status == true)
    {
        GB_interpretOpcode(0x0);
        GB_interpretOpcode(0x0);

    }
    else
    {
        return -1;
    }

    return 0;
}