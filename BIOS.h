#ifndef __BIOS_H__
#define __BIOS_H__

#include <string>
#include "MemoryLocations.h"
class BIOS_SET
{
    public:

		myTest t;

		bool check_logo(char* BIOS_ROM, char* GAME_ROM);

		char* get_bios(std::string BIOSNAME);

		char* get_rom(std::string ROMNAME);

};

#endif

