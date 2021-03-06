// ClientSession.h
//
// Copyright (c) 1995-1998 - Richard Langlois and Grokksoft Inc.
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
//

#pragma once

#include "../../engine/Model/GameSession.h"
#include "../../engine/VideoServices/Sprite.h"
#include "../../engine/Util/OS.h"

namespace HoverRace {
	namespace Client {
		namespace HoverScript {
			class MetaSession;
			class TrackPeer;
		}
		class Rules;
	}
	namespace MainCharacter {
		class MainCharacter;
	}
	namespace Player {
		class Player;
	}
	namespace Model {
		class Track;
	}
	namespace Script {
		class Core;
	}
	namespace Util {
		class Clock;
		class Duration;
	}
}

namespace HoverRace {
namespace Client {

class ClientSession
{
private:
	ClientSession() = delete;
public:
	ClientSession(std::shared_ptr<Rules> rules);
	virtual ~ClientSession();

public:
	enum class Phase
	{
		INIT,
		PREGAME,  ///< Players are at the starting line.
		PLAYING,  ///< Players are released and the clock is running.
		POSTGAME,  ///< The first player has finished; waiting for others.
		DONE  ///< All players have finished.
	};

protected:
	struct ChatMessage
	{
		ChatMessage() : mCreationTime(0) { }

		time_t mCreationTime;
		std::string mBuffer;
	};

public:
	Phase GetPhase() const { return phase; }
	bool AdvancePhase();
	bool AdvancePhase(Phase nextPhase);

	std::shared_ptr<HoverScript::MetaSession> GetMeta() const { return meta; };
	void SetMeta(std::shared_ptr<HoverScript::MetaSession> meta);

	// Simulation control
	virtual void Process();

	virtual bool LoadNew(const char *pTitle, Script::Core &scripting,
		std::shared_ptr<Model::Track> track,
		VideoServices::VideoBuffer *pVideo);

	void AttachPlayer(int i, std::shared_ptr<Player::Player> player);

	std::shared_ptr<Util::Clock> GetClock() { return clock; }
	std::shared_ptr<Util::Clock> GetCountdown() { return countdown; }
	void CountdownToNextPhase(const Util::Duration &duration);
	virtual void SetSimulationTime(MR_SimulationTime pTime);
	MR_SimulationTime GetSimulationTime() const;
	void UpdateCharacterSimulationTimes();

	const MR_UInt8 *GetBackImage() const;

	virtual int ResultAvaillable() const;	  // Return the number of players desc avail
	virtual void GetResult(int pPosition, const char *&pPlayerName, int &pId, BOOL & pConnected, int &pNbLap, MR_SimulationTime &pFinishTime, MR_SimulationTime &pBestLap) const;
	virtual void GetHitResult(int pPosition, const char *&pPlayerName, int &pId, BOOL & pConnected, int &pNbHitOther, int &pNbHitHimself) const;

	virtual int GetNbPlayers() const;
	virtual int GetRank(const MainCharacter::MainCharacter * pPlayer) const;

	virtual Player::Player *GetPlayer(int i) const
	{
		return players[static_cast<size_t>(i)].get();
	}

	virtual std::shared_ptr<Player::Player> SharePlayer(int i) const
	{
		return players[static_cast<size_t>(i)];
	}

	// Chat related functions (all messages are already converted in internal ASCII
	virtual void AddMessageKey(char pKey);
	virtual void GetCurrentMessage(char *pDest) const;
	BOOL GetMessageStack(int pLevel, char *pDest, int pExpiration) const;
	void AddMessage(const char *pMessage);

	const Model::Level *GetCurrentLevel() const;
	std::shared_ptr<HoverScript::TrackPeer> GetTrackPeer() const { return trackPeer; }

	std::shared_ptr<Rules> GetRules() { return rules; }

private:
	Phase phase;

	mutable boost::mutex chatMutex;
	static const int CHAT_MESSAGE_STACK = 8;
	ChatMessage mMessageStack[CHAT_MESSAGE_STACK];

	Model::GameSession mSession;
	static const int MAX_PLAYERS = 4;
	std::array<std::shared_ptr<Player::Player>, MAX_PLAYERS> players;

	std::shared_ptr<HoverScript::MetaSession> meta;
	std::shared_ptr<HoverScript::TrackPeer> trackPeer;

	MR_UInt8 *mBackImage;

	std::shared_ptr<Util::Clock> clock;
	std::shared_ptr<Util::Clock> countdown;
	boost::signals2::scoped_connection countdownConn;
	std::shared_ptr<Rules> rules;

	void ReadLevelAttrib(Parcel::RecordFile *pFile,
		VideoServices::VideoBuffer *pVideo);
};

}  // namespace Client
}  // namespace HoverRace
