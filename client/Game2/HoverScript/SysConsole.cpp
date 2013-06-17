
// SysConsole.cpp
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

#include "../../../engine/Script/Help/HelpHandler.h"
#include "../../../engine/Script/Help/Class.h"
#include "../../../engine/Util/Config.h"
#include "GamePeer.h"

#include "SysConsole.h"

using HoverRace::Util::Config;
namespace Log = HoverRace::Util::Log;

namespace HoverRace {
namespace Client {
namespace HoverScript {

/**
 * Constructor.
 * @param scripting The underlying scripting engine.
 * @param gamePeer Game scripting peer.
 * @param maxLogLines The maximum number of log lines to store in the
 *                    log history.
 */
SysConsole::SysConsole(Script::Core *scripting, GamePeer *gamePeer,
                       int maxLogLines) :
	SUPER(scripting),
	gamePeer(gamePeer),
	introWritten(false), maxLogLines(maxLogLines), logLines(), baseLogIdx(0)
{
	commandLine.reserve(1024);

	// Add introductory text.
	LogInfo(_("System console initialized."));

	logConn = Log::logAddedSignal.connect(
		std::bind(&SysConsole::OnLog, this, std::placeholders::_1));
}

SysConsole::~SysConsole()
{
	logConn.disconnect();
}

void SysConsole::InitEnv()
{
	using namespace luabind;

	SUPER::InitEnv();

	lua_State *L = GetScripting()->GetState();

	// Start with the standard global environment.
	CopyGlobals();

	object env(from_stack(L, -1));
	env["game"] = gamePeer;
}

void SysConsole::Clear()
{
	// Bump the base log index so that AddLogLine() will generate the correct
	// next log index.
	baseLogIdx += logLines.size();

	logLines.clear();

	// Notify listeners.
	logClearedSignal();
}

void SysConsole::OnLog(const Util::Log::Entry &entry)
{
	LogLevel::level_t logLevel;
	switch (entry.level) {
		case Log::Level::DEBUG:
		case Log::Level::INFO:
		case Log::Level::WARN:
			logLevel = LogLevel::INFO;
			break;
		case Log::Level::ERROR:
		case Log::Level::FATAL:
		default:
			logLevel = LogLevel::INFO;
	}

	AddLogLine(logLevel, entry.message);
}

void SysConsole::AddLogLine(LogLevel::level_t level, const std::string &line)
{
	int sz = logLines.size();
	int idx = baseLogIdx + sz;

	// Trim excess old log lines.
	while (sz >= maxLogLines) {
		logLines.pop_front();
		sz--;
		baseLogIdx++;
	}

	logLines.emplace_back(LogLine(idx, level, line));

	// Notify listeners.
	logAddedSignal(idx);
}

/**
 * Add the introductory text to the log.
 * The text is only written once; after that, this function has no effect.
 * The idea is that we wait until the console is first visible before writing
 * the intro text -- otherwise, the intro text will get lost in the sea of
 * other log entries.
 */
void SysConsole::AddIntroLines()
{
	if (introWritten) return;
	introWritten = true;

	Config *cfg = Config::GetInstance();

	std::string heading = PACKAGE_NAME " version ";
	heading += cfg->GetVersion();
	std::string headingDecor(heading.length(), '-');

	LogNote(headingDecor);
	LogNote(heading);
	LogNote(headingDecor);

	Script::Core *env = GetScripting();
	std::string intro = env->GetVersionString();
	intro += " :: Console active.";
	LogNote(intro);

	LogNote("Available global objects: game");

	std::string helpInstructions = boost::str(boost::format(
		_("For help on a class or method, call the %s method: %s")) %
		"help()" % "game:help(); game:help(\"on_init\")");
	LogNote(helpInstructions);
}

void SysConsole::LogHistory(const std::string &s)
{
	AddLogLine(LogLevel::HISTORY, s);
}

void SysConsole::LogNote(const std::string &s)
{
	AddLogLine(LogLevel::NOTE, s);
}

void SysConsole::LogInfo(const std::string &s)
{
	AddLogLine(LogLevel::INFO, s);
}

void SysConsole::LogError(const std::string &s)
{
	AddLogLine(LogLevel::ERROR, s);
}

/**
 * Retrieve the index of the last log entry.
 * @return The index or -1 if the log has been cleared.
 */
int SysConsole::GetEndLogIndex() const
{
	return logLines.empty() ? -1 : logLines.back().idx;
}

void SysConsole::HelpClass(const Script::Help::Class &cls)
{
	using Script::Help::Class;
	using Script::Help::MethodPtr;

	LogInfo(boost::str(boost::format(_("Methods for class %s:")) % cls.GetName()));

	std::string s;
	s.reserve(1024);
	BOOST_FOREACH(const Class::methods_t::value_type &ent, cls.GetMethods()) {
		MethodPtr method = ent.second;
		s.clear();
		s += "  ";
		s += method->GetName();
		
		const std::string &brief = method->GetBrief();
		if (!brief.empty()) {
			s += " - ";
			s += brief;
		}

		LogInfo(s);
	}
}

void SysConsole::HelpMethod(const Script::Help::Class &cls,
                            const Script::Help::Method &method)
{
	using Script::Help::Method;
	
	LogInfo("---");

	BOOST_FOREACH(const std::string &s, method.GetSigs()) {
		LogInfo(s);
	}

	LogInfo(method.GetBrief());
	const std::string &desc = method.GetDesc();
	if (!desc.empty()) {
		LogInfo(method.GetDesc());
	}
}

}  // namespace HoverScript
}  // namespace Client
}  // namespace HoverRace

