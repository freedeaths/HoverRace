
// SysEnv.cpp
//
// Copyright (c) 2010, 2013-2016 Michael Imamura.
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

#include <iostream>

#include <luabind/luabind.hpp>

#include "../../../engine/Script/Core.h"
#include "../../../engine/Util/Str.h"

#include "DebugPeer.h"
#include "GamePeer.h"
#include "InputPeer.h"

#include "SysEnv.h"

namespace fs = boost::filesystem;
using namespace HoverRace::Util;

namespace {

class LogStreamBuf : public std::stringbuf /*{{{*/
{
	using SUPER = std::stringbuf;

public:
	LogStreamBuf() { }
	virtual ~LogStreamBuf() { sync(); }

protected:
	int sync() override
	{
#		ifdef _WIN32
			OutputDebugStringW((const wchar_t*)Str::UW(str()));
#		else
			std::cout << str() << std::flush;
#		endif

		str({});
		return 0;
	}
}; //}}}

class LogStream : public std::ostream /*{{{*/
{
	using SUPER = std::ostream;

public:
	LogStream() : SUPER(new LogStreamBuf()) { }
	virtual ~LogStream() { delete rdbuf(); }
}; //}}}

}  // namespace

namespace HoverRace {
namespace Client {
namespace HoverScript {

SysEnv::SysEnv(Script::Core &scripting, DebugPeer &debugPeer,
	GamePeer &gamePeer, InputPeer &inputPeer) :
	SUPER(scripting), debugPeer(debugPeer), gamePeer(gamePeer),
	inputPeer(inputPeer),
	outHandle(scripting.AddOutput(std::make_shared<LogStream>()))
{
}

SysEnv::~SysEnv()
{
	GetScripting().RemoveOutput(outHandle);
}

void SysEnv::InitEnv()
{
	using namespace luabind;

	lua_State *L = GetState();

	// Start with the standard global environment.
	CopyGlobals();

	object env(from_stack(L, -1));
	env["debug"] = &debugPeer;
	env["game"] = &gamePeer;
	env["input"] = &inputPeer;
}

void SysEnv::LogInfo(const std::string &s)
{
#	ifdef _WIN32
		OutputDebugStringW((const wchar_t*)Str::UW(s));
		OutputDebugStringW(L"\n");
#	else
		std::cout << s << std::endl;
#	endif
}

void SysEnv::LogError(const std::string &s)
{
#	ifdef _WIN32
		OutputDebugStringW((const wchar_t*)Str::UW(s));
		OutputDebugStringW(L"\n");
#	else
		std::cerr << s << std::endl;
#	endif
}

}  // namespace HoverScript
}  // namespace Client
}  // namespace HoverRace
