
// Label.cpp
//
// Copyright (c) 2013-2016 Michael Imamura.
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

#include "Label.h"

using namespace HoverRace::Util;

namespace HoverRace {
namespace Display {

/**
 * Constructor for automatically-sized label.
 * @param text The text.
 * @param font The font.
 * @param color The text foreground color (including alpha).
 * @param layoutFlags Optional layout flags.
 */
Label::Label(const std::string &text,
	const UiFont &font,
	Color color,
	uiLayoutFlags_t layoutFlags) :
	SUPER(text, font, color, layoutFlags),
	scale(1.0), wrapWidth(-1)
{
}

/**
 * Constructor for fixed-width label.
 * @param wrapWidth The width of the label.
 * @param text The text.
 * @param font The font.
 * @param color The text foreground color (including alpha).
 * @param layoutFlags Optional layout flags.
 */
Label::Label(double wrapWidth,
	const std::string &text,
	const UiFont &font,
	Color color,
	uiLayoutFlags_t layoutFlags) :
	SUPER(text, font, color, layoutFlags),
	scale(1.0), wrapWidth(wrapWidth)
{
}

/**
 * Automatically determine the width of the text.
 * The width will be the longest line in the text.
 */
void Label::SetAutoWidth()
{
	if (wrapWidth > 0) {
		wrapWidth = -1;
		FireModelUpdate(Props::WRAP_WIDTH);
	}
}

void Label::SetScale(double scale)
{
	if (this->scale != scale) {
		this->scale = scale;
		FireModelUpdate(Props::SCALE);
	}
}

/**
 * Set a fixed width for the label.
 * Text will be wrapped to fit the width.
 * Single words which are longer than will fit in the width will be clipped.
 * @param wrapWidth The fixed width.
 */
void Label::SetWrapWidth(double wrapWidth)
{
	if (this->wrapWidth != wrapWidth) {
		this->wrapWidth = wrapWidth;
		FireModelUpdate(Props::WRAP_WIDTH);
	}
}

}  // namespace Display
}  // namespace HoverRace
