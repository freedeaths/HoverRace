
// HudPeer.h
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

#pragma once

#include <luabind/luabind.hpp>
#include <luabind/object.hpp>

#include "../../../engine/Display/Hud.h"
#include "../../../engine/Script/Core.h"
#include "../../../engine/Script/Peer.h"

namespace HoverRace {
	namespace Display {
		class Counter;
		class Display;
	}
}

namespace HoverRace {
namespace Client {
namespace HoverScript {

/**
 * Scripting peer for access to the HUD for a player.
 * @author Michael Imamura
 */
class HudPeer : public Script::Peer {
	typedef Script::Peer SUPER;
	typedef HoverRace::Display::Hud::HudAlignment HudAlignment;
	public:
		HudPeer(Script::Core *scripting, Display::Display &display,
			std::weak_ptr<Display::Hud> hud);
		virtual ~HudPeer();

	public:
		static void Register(Script::Core *scripting);

	private:
		/**
		 * Convert an int into an alignment value, raising a Lua error if the
		 * the value is out of range.
		 * @param align The alignment value passed from Lua.
		 * @return The HUD alignment.
		 */
		HudAlignment::type ValidateAlignment(int align)
		{
			try {
				return HudAlignment::FromInt(align);
			}
			catch (Exception &ex) {
				luaL_error(GetScripting()->GetState(), "%s", ex.what());
				throw;  // Never actually reached (luaL_error doesn't return).
			}
		}

	public:
		std::shared_ptr<Display::Counter> LAddCounter_V(int align,
			const std::string &title, double initValue);
		std::shared_ptr<Display::Counter> LAddCounter_VT(int align,
			const std::string &title, double initValue, double total);

		template<class T>
		std::shared_ptr<T> LAddDecor(int align)
		{
			HudAlignment::type ha = ValidateAlignment(align);
			if (auto sp = hud.lock()) {
				return sp->AddHudChild(ha, new T(display));
			}
			else {
				return std::shared_ptr<T>();
			}
		}

		void LUseRaceDefault();

	private:
		Display::Display &display;
		std::weak_ptr<Display::Hud> hud;
};

}  // namespace HoverScript
}  // namespace Client
}  // namespace HoverRace
