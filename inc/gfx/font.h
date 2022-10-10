#pragma once
#include <stdint.h>
#include <std/decl.h>
#include <gfx/fontdata.hpp>
namespace system::kernel::gfx
{
    typedef struct
	{
		uint8_t  Width;
		uint8_t  Height;
		uint8_t  SpacingX;
		uint8_t  SpacingY;
		uint8_t* Data;
	} packed__ Font;

    namespace Fonts
    {
		static const Font System8x16 = { 8, 16, 0, 0, (uint8_t*)FontData::Font_System8x16_Data };
		static const Font System8x8  = { 8, 8,  0, 0, (uint8_t*)FontData::Font_System8x8_Data  };
		static const Font Serif8x16  = { 8, 16, 0, 0, (uint8_t*)FontData::Font_Serif8x16_Data  };
		static const Font Serif8x8   = { 8, 8,  0, 0, (uint8_t*)FontData::Font_Serif8x8_Data   };
    }
}