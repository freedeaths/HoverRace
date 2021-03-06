
// MessageAnnouncement.h
//
// Copyright (c) 2014-2016 Michael Imamura.
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

#pragma once

#include "Announcement.h"

namespace HoverRace {
namespace Client {

/**
 * Basic message.
 * @author Michael Imamura
 */
class MessageAnnouncement : public Announcement
{
	using SUPER = Announcement;

public:
	MessageAnnouncement(const std::string &label,
		std::shared_ptr<Player::Player> player = {}) :
		MessageAnnouncement(label, "", std::move(player)) { }
	MessageAnnouncement(const std::string &label, const std::string &text,
		std::shared_ptr<Player::Player> player = {}) :
		MessageAnnouncement("message", label, text, player) { }
protected:
	MessageAnnouncement(const std::string &postType,
		const std::string &label, const std::string &text,
		std::shared_ptr<Player::Player> player = { });

public:
	virtual ~MessageAnnouncement() { }

public:
	std::shared_ptr<Display::Box> CreateIcon(Display::Display &display,
		Display::Container &parent) const override;
	void CreateContents(Display::Display &display,
		Display::FlexGrid &grid) const override;

private:
	const std::string text;
};

}  // namespace Client
}  // namespace HoverRace
