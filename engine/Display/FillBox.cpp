
// FillBox.cpp
//
// Copyright (c) 2013 Michael Imamura.
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

#include "StdAfx.h"

#include "FillBox.h"

using namespace HoverRace::Util;

namespace HoverRace {
namespace Display {

/**
 * Constructor.
 * @param size The size of the box, where @c x is the width
 *             and @c y is the height.
 * @param color The color of the box (including alpha).
 */
FillBox::FillBox(const Vec2 &size, Color color) :
	SUPER(),
	size(size), color(color)
{
}

/**
 * Constructor.
 * @param w The width of the box.
 * @param h The height of the box.
 * @param color The color of the box (including alpha).
 */
FillBox::FillBox(double w, double h, Color color) :
	SUPER(),
	size(Vec2(w, h)), color(color)
{
}

/**
 * Set the color of the box.
 * @param color The color (including alpha).
 */
void FillBox::SetColor(const Color color)
{
	if (this->color != color) {
		this->color = color;
		FireModelUpdate(Props::COLOR);
	}
}

/**
 * Set the size of the box.
 * @param size The size of the box, where @c x is the width
 *             and @c y is the height.
 */
void FillBox::SetSize(const Vec2 &size)
{
	if (this->size != size) {
		this->size = size;
		FireModelUpdate(Props::SIZE);
	}
}

}  // namespace Display
}  // namespace HoverRace
