
// Button.cpp
//
// Copyright (c) 2013-2015 Michael Imamura.
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

#include "../Control/Action.h"
#include "../Util/Config.h"
#include "UiFont.h"
#include "FillBox.h"
#include "Label.h"

#include "Button.h"

using namespace HoverRace::Util;

namespace HoverRace {
namespace Display {

namespace {
	const double DEFAULT_PADDING_HEIGHT = 10;
	const double DEFAULT_PADDING_WIDTH = 30;
	const double DEFAULT_ICON_GAP = 10;
	const double DEFAULT_PRESS_DEPTH = 4;
}

namespace {

/**
 * Determine the position coordinates for a widget.
 * @param x The upper-left corner of the bounding box.
 * @param y The upper-left corner of the bounding box.
 * @param w The width of the bounding box.
 * @param h The height of the bounding box.
 * @param alignment The alignment of the contained widget.
 * @return The position.
 */
Vec2 AlignToBounds(
	double x, double y, double w, double h,
	UiViewModel::Alignment alignment)
{
	using Alignment = UiViewModel::Alignment;

	switch (alignment) {
		case Alignment::NW: return Vec2(x, y);
		case Alignment::N: return Vec2(x + (w / 2.0), y);
		case Alignment::NE: return Vec2(x + w, y);
		case Alignment::E: return Vec2(x + w, y + (h / 2.0));
		case Alignment::SE: return Vec2(x + w, y + h);
		case Alignment::S: return Vec2(x + (w / 2.0), y + h);
		case Alignment::SW: return Vec2(x, y + h);
		case Alignment::W: return Vec2(x, y + (h / 2.0));
		case Alignment::CENTER: return Vec2(x + (w / 2.0), y + (h / 2.0));
		default:
			throw Exception("Unknown alignment: " +
				boost::lexical_cast<std::string>(static_cast<int>(alignment)));
	}
}

}

/**
 * Constructor for automatically-sized button.
 * @param display The display child elements will be attached to.
 * @param text The text for the button.
 * @param layoutFlags Optional layout flags.
 */
Button::Button(Display &display, const std::string &text,
               uiLayoutFlags_t layoutFlags) :
	SUPER(display, layoutFlags),
	paddingTop(DEFAULT_PADDING_HEIGHT), paddingRight(DEFAULT_PADDING_WIDTH),
	paddingBottom(DEFAULT_PADDING_HEIGHT), paddingLeft(DEFAULT_PADDING_WIDTH),
	iconGap(DEFAULT_ICON_GAP)
{
	Init(text);
}

/**
 * Constructor for fixed-sized button.
 * @param display The display child elements will be attached to.
 * @param size The fixed button size.
 * @param text The text for the button.
 * @param layoutFlags Optional layout flags.
 */
Button::Button(Display &display, const Vec2 &size, const std::string &text,
               uiLayoutFlags_t layoutFlags) :
	SUPER(display, size, layoutFlags),
	paddingTop(DEFAULT_PADDING_HEIGHT), paddingRight(DEFAULT_PADDING_WIDTH),
	paddingBottom(DEFAULT_PADDING_HEIGHT), paddingLeft(DEFAULT_PADDING_WIDTH)
{
	Init(text);
}

Button::~Button()
{
}

void Button::Init(const std::string &text)
{
	const auto &s = display.styles;

	label.reset(new Label(text, s.formFont, s.formFg));
	label->SetAlignment(Alignment::CENTER);
	label->AttachView(display);

	background.reset(new FillBox(0, 0, s.buttonBg));
	background->AttachView(display);
}

bool Button::OnNavigate(const Control::Nav &nav)
{
	if (nav.AsDigital() != Control::Nav::NEUTRAL) {
		RelinquishFocus(nav);
		return true;
	}
	else {
		return false;
	}
}

void Button::Layout()
{
	const Vec2 size = GetSize();
	const auto &s = display.styles;

	background->SetSize(size);

	Vec2 boundsPos(paddingLeft, paddingTop);
	Vec2 boundsSize(
		size.x - (paddingLeft + paddingRight),
		size.y - (paddingTop + paddingBottom));

	// If there is an icon, position it and shift the text to make room.
	if (icon) {
		double iconY = paddingTop;
		if (IsPressed()) {
			iconY += DEFAULT_PRESS_DEPTH;
		}
		icon->SetPos(paddingLeft, iconY);
		double iw = icon->Measure().x + iconGap;
		boundsPos.x += iw;
		boundsSize.x -= iw;
	}

	Vec2 labelPos = AlignToBounds(
		boundsPos.x, boundsPos.y,
		boundsSize.x, boundsSize.y,
		label->GetAlignment());

	if (!IsEnabled()) {
		background->SetColor(s.buttonDisabledBg);
		background->SetBorder(0);
		label->SetColor(s.formDisabledFg);
		label->SetPos(labelPos);
	}
	else if (IsPressed()) {
		background->SetColor(s.buttonPressedBg);
		label->SetColor(s.formFg);
		label->SetPos(labelPos.x, labelPos.y + DEFAULT_PRESS_DEPTH);
	}
	else {
		background->SetColor(s.buttonBg);
		label->SetColor(s.formFg);
		label->SetPos(labelPos);
	}

	if (IsFocused()) {
		background->SetBorder(4);
		background->SetBorderColor(s.buttonFocusedBg);
	}
	else {
		background->SetBorder(0);
	}
}

const std::string &Button::GetText() const
{
	return label->GetText();
}

void Button::SetText(const std::string &text)
{
	label->SetText(text);
}

UiViewModel::Alignment Button::getTextAlignment() const
{
	return label->GetAlignment();
}

/**
 * Sets the alignment of the text within the button.
 * @param textAlignment The text alignment.
 */
void Button::SetTextAlignment(Alignment textAlignment)
{
	if (label->GetAlignment() != textAlignment) {
		label->SetAlignment(textAlignment);
		RequestLayout();
	}
}

/**
 * Set the optional icon to appear on the button, in addition to the text.
 *
 * The icon will be sized to the standard size for the button (namely, the
 * height of the text), so while it is safe to share the icon between
 * buttons, be aware that it may be modified.
 *
 * @param icon A subclass of FillBox to use as an icon, or @c nullptr to
 *             unset the icon.
 */
void Button::SetIcon(std::shared_ptr<FillBox> icon)
{
	if (this->icon != icon) {
		this->icon = std::move(icon);
		FireModelUpdate(Props::ICON);
		RequestSizing();
		RequestLayout();
	}
}

Vec3 Button::Measure()
{
	if (IsAutoSize()) {
		const Vec3 labelSize = label->Measure();
		double iconPart = 0;
		if (icon) {
			// The icon's width may vary depending on the height, so set the
			// height now and measure the new width.
			icon->AdjustHeight(labelSize.y);
			iconPart = icon->Measure().x + iconGap;
		}
		return Vec3(
			labelSize.x + paddingLeft + paddingRight + iconPart,
			labelSize.y + paddingTop + paddingBottom,
			0);
	}
	else {
		return SUPER::Measure();
	}
}

void Button::FireModelUpdate(int prop)
{
	switch (prop) {
		case UiViewModel::Props::FOCUSED:
		case SUPER::Props::SIZE:
		case SUPER::Props::ENABLED:
		case SUPER::Props::PRESSED:
			RequestLayout();
	}
	SUPER::FireModelUpdate(prop);
}

}  // namespace Display
}  // namespace HoverRace
