#include "BIOS.h"
#include "Registers.h"
#include "MemoryLocations.h"
// classes example
#include <Windows.h>
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
myTest MEMORY_DEF;

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
    std::ifstream data(BIOSNAME, std::ios::binary | std::ios::ate);
    data >> std::noskipws;

    int filesize = data.tellg(); //getting filesize in byte
    data.seekg(0); //setting the cursor back to the beginning of the file

    char* filebytes = new char[filesize];

    char currentByte;
    int rowWidth = 16;
    int cCol = 0;
    while (!data.eof()) {
        data.get(currentByte);

        // std::cout << std::hex << (int)currentByte << " ";
        filebytes[cCol] = currentByte;

        cCol++;
        if (cCol > 0 && cCol % rowWidth == 0) {
            // std::cout << std::endl;
        }

    }
    data.close();
    return filebytes;
}

char * BIOS_SET::get_rom(string ROMNAME)
{
    std::ifstream data(ROMNAME, std::ios::binary | std::ios::ate);
    data >> std::noskipws;

    int filesize = data.tellg(); //getting filesize in byte
    data.seekg(0); //setting the cursor back to the beginning of the file

    char* filebytes = new char[filesize];

    char currentByte;
    int rowWidth = 16;
    int cCol = 0;
    while (!data.eof()) {
        data.get(currentByte);

       // std::cout << std::hex << (int)currentByte << " ";
        filebytes[cCol] = currentByte;

        cCol++;
        if (cCol > 0 && cCol % rowWidth == 0) {
           // std::cout << std::endl;
        }

    }
    data.close();
    return filebytes;
}

int main() 
{
    BIOS_SET _BIOS;
    int j = 0;
    char*  rom_file = _BIOS.get_rom("C://Users//zoepe//Downloads//tetris.bin");
    char * bios_file = _BIOS.get_bios("C://Users//zoepe//Downloads//dmg_boot.bin");
    t.BIOS_GRAPHIC = bios_file;
    bool x = _BIOS.check_logo(rom_file, t.BIOS_GRAPHIC);
    cout << "result: " << x ? "true" : "false";
    
    MEMORY_MAP = (uint8_t *) rom_file;
    bool start = false; 
        
     
    for (int i = 0; i < sizeof(t.ROM_EXE); i++)
    {
        t.ROM_EXE[i] = MEMORY_MAP[i];
    }
    ofstream myFile("data.bin", ios::out | ios::binary);
    myFile.write((char *)t.ROM_EXE, sizeof(t.ROM_EXE));
    
    while (1)
    {
        if (start == false)
        {
            GB_interpretOpcode(0xff);
            start = true;
        }
       else
        {
            GB_interpretOpcode(t.ROM_EXE[j]);
            Sleep(5000);
            j++;
        }
    }
 
    return 0;
}

