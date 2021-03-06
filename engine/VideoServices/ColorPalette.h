// ColorPalette.h
//
// The color palette is a module, not a class. It is the same
// colorpalette that is always used
//
//
// Copyright (c) 1995-1998 - Richard Langlois and Grokksoft Inc.
//
// Licensed under GrokkSoft HoverRace SourceCode License v1.0(the "License");
// you may not use this file except in compliance with the License.
//
// A copy of the license should have been attached to the package from which
// you have taken this file. If you can not find the license you can not use
// this file.
//
//
// The author makes no representations about the suitability of
// this software for any purpose.  It is provided "as is" "AS IS",
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied.
//
// See the License for the specific language governing permissions
// and limitations under the License.
//

#pragma once

#include <SDL2/SDL.h>

#include "../Util/MR_Types.h"

#if defined(_WIN32) && defined(HR_ENGINE_SHARED)
#	ifdef MR_ENGINE
#		define MR_DllDeclare   __declspec( dllexport )
#	else
#		define MR_DllDeclare   __declspec( dllimport )
#	endif
#else
#	define MR_DllDeclare
#endif

namespace HoverRace {
namespace VideoServices {

namespace ColorPalette {

#define MR_NB_COLORS                  256
#define MR_RESERVED_COLORS_BEGINNING   10
#define MR_RESERVED_COLORS_END         15
#define MR_RESERVED_COLORS              6
#define MR_BASIC_COLORS               100		  // Includes some extra space
#define MR_BACK_COLORS                128

#define MR_NB_COLOR_INTENSITY       256
#define MR_NORMAL_INTENSITY         128

// Color convertion tables
MR_DllDeclare extern MR_UInt8 colorTable[MR_NB_COLOR_INTENSITY][MR_NB_COLORS];
MR_DllDeclare extern MR_UInt8 colorAdditionTable[MR_NB_COLORS][MR_NB_COLORS];
extern double basicPalette[][3];
extern int basicPaletteSize;

using paletteEntry_t = SDL_Color;
inline constexpr MR_UInt8 &PalR(paletteEntry_t &ent) noexcept { return ent.r; }
inline constexpr MR_UInt8 &PalG(paletteEntry_t &ent) noexcept { return ent.g; }
inline constexpr MR_UInt8 &PalB(paletteEntry_t &ent) noexcept { return ent.b; }

MR_DllDeclare paletteEntry_t *GetColors(double pGamma, double pIntensity = 0.8, double pIntensityBase = 0.0);
MR_DllDeclare const paletteEntry_t &ConvertColor(MR_UInt8 pRed, MR_UInt8 pGreen, MR_UInt8 pBlue, double pGamma, double pIntensity = 0.8, double pIntensityBase = 0.0) noexcept;

std::ostream &operator<<(std::ostream &os, const paletteEntry_t &ent);

}  // namespace ColorPalette

}  // namespace VideoServices
}  // namespace HoverRace

#undef MR_DllDeclare
