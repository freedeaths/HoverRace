
// SysConsole.h
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

#pragma once

#include <boost/circular_buffer.hpp>

#include "../../../engine/Util/Log.h"

#include "Console.h"

// Previously defined in WinGDI.h.
#ifdef ERROR
#	undef ERROR
#endif

namespace HoverRace {
	namespace Client {
		namespace HoverScript {
			class DebugPeer;
			class GamePeer;
			class InputPeer;
			class MetaSession;
		}
		class ClientSession;
		class GameDirector;
	}
}

namespace HoverRace {
namespace Client {
namespace HoverScript {

/**
 * Global always-on console.
 *
 * This is the backend for the ConsoleScene, allowing it to maintain state
 * between instances.
 *
 * @author Michael Imamura
 */
class SysConsole : public Console
{
	using SUPER = Console;

public:
	SysConsole(Script::Core &scripting, GameDirector &director,
		DebugPeer &debugPeer, GamePeer &gamePeer, InputPeer &inputPeer,
		size_t maxLogLines = 512,
		size_t maxHistory = 64);
	virtual ~SysConsole();

protected:
	void InitEnv() override;

public:
	void Advance(Util::OS::timestamp_t tick) override { HR_UNUSED(tick); }

private:
	void OnSessionChanged(std::shared_ptr<MetaSession> metaSession);

public:
	void Clear() override;
	void SubmitChunkWithHistory(const std::string &s);

public:
	enum class LogLevel { HISTORY, NOTE, INFO, ERROR };
	struct LogLine
	{
		LogLine(int idx, LogLevel level, const std::string &line) :
			idx(idx), level(level), line(line) { }

		LogLine &operator=(const LogLine&) = delete;

		const int idx;
		const LogLevel level;
		const std::string line;
	};
private:
	void OnLog(const Util::Log::Entry &entry);
	void AddLogLine(LogLevel level, const std::string &line);
public:
	void AddIntroLines();
	void LogHistory(const std::string &s);
	void LogNote(const std::string &s);
	void LogInfo(const std::string &s) override;
	void LogError(const std::string &s) override;

	void LoadPrevCmd();
	void LoadNextCmd();

public:
	int GetEndLogIndex() const;

	/**
	 * Read all log entries.
	 * @param fn The callback function (will be passed a LogLine reference).
	 */
	template<class Function>
	void ReadLogs(Function fn)
	{
		if (!logLines.empty()) {
			std::for_each(logLines.cbegin(), logLines.cend(), fn);
		}
	}

	/**
	 * Read a subset of log entries, from a starting index to the end.
	 * @param start The starting log index (inclusive).
	 * @param fn The callback function (will be passed a LogLine reference).
	 */
	template<class Function>
	void ReadLogs(size_t start, Function fn)
	{
		if (!logLines.empty()) {
			if (start < baseLogIdx) {
				ReadLogs(fn);
			}
			else {
				// Safe case since the end log index only returns -1 if empty.
				size_t endIdx = static_cast<size_t>(GetEndLogIndex());
				ReadLogs(start, endIdx, fn);
			}
		}
	}

	/**
	 * Read a subset of log entries, for a specific range.
	 * @param start The starting log index (inclusive).
	 * @param end The ending log index (inclusive).
	 * @param fn The callback function (will be passed a LogLine reference).
	 */
	template<class Function>
	void ReadLogs(size_t start, size_t end, Function fn)
	{
		if (logLines.empty()) return;
		if (start > end) return;
		size_t endIdx = baseLogIdx + logLines.size() - 1;
		if (start > endIdx) return;
		size_t idx = start - baseLogIdx;

		if (start == end) {
			fn(logLines.at(idx));
		}
		else {
			if (start < baseLogIdx) start = baseLogIdx;
			auto startIter = logLines.begin();
			std::advance(startIter, start - baseLogIdx);
			auto endIter = logLines.end();
			if (end <= endIdx) {
				endIter = startIter;
				std::advance(endIter, end - start + 1);
			}
			std::for_each(startIter, endIter, fn);
		}
	}

	std::string &GetCommandLine() { return commandLine; }

public:
	/// Fired when the log is cleared.
	using logClearedSignal_t = boost::signals2::signal<void()>;
	logClearedSignal_t &GetLogClearedSignal() { return logClearedSignal; }

	/// Fired when a log line is added.  Parameter is the log index.
	using logAddedSignal_t = boost::signals2::signal<void(size_t)>;
	logAddedSignal_t &GetLogAddedSignal() { return logAddedSignal; }

public:
	// HelpHandler.
	void HelpClass(const Script::Help::Class &cls) override;
	void HelpMethod(const Script::Help::Class &cls,
		const Script::Help::Method &method) override;

public:
	static int LQuit(lua_State *L);

private:
	GameDirector &director;
	DebugPeer &debugPeer;
	GamePeer &gamePeer;
	InputPeer &inputPeer;
	std::shared_ptr<MetaSession> metaSession;

	boost::signals2::scoped_connection sessionChangedConn;

	bool introWritten;
	size_t maxLogLines;
	std::deque<LogLine> logLines;
	size_t baseLogIdx;  ///< Index of the first item in logLines.

	using history_t = boost::circular_buffer<std::string>;
	history_t history;
	history_t::iterator curHistory;

	std::string commandLine;

	boost::signals2::scoped_connection logConn;

	logClearedSignal_t logClearedSignal;
	logAddedSignal_t logAddedSignal;
};

}  // namespace HoverScript
}  // namespace Client
}  // namespace HoverRace

