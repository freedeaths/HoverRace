
// RulebookLibrary.cpp
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

#include <boost/filesystem.hpp>

#include "../../engine/Util/Config.h"
#include "../../engine/Util/Log.h"
#include "../../engine/Util/Str.h"

#include "HoverScript/RulebookEnv.h"
#include "Rulebook.h"

#include "RulebookLibrary.h"

using namespace HoverRace::Util;
namespace fs = boost::filesystem;

namespace HoverRace {
namespace Client {

RulebookLibrary::RulebookLibrary(Script::Core &scripting) :
	scripting(scripting),
	library(), sorted()
{
}

/**
 * Run all rulebook scripts to re-populate the rulebook library.
 */
void RulebookLibrary::Reload()
{
	Config *cfg = Config::GetInstance();

	OS::path_t dir = cfg->GetMediaPath();
	dir /= Str::UP("rulebooks");

	if (!fs::exists(dir)) {
		HR_LOG(error) << "Rulebook path does not exist: " << dir;
		return;
	}
	if (!fs::is_directory(dir)) {
		HR_LOG(error) << "Rulebook path is not a directory (skipping): " << dir;
		return;
	}

	int rulebooksLoaded = 0;
	const OS::dirIter_t END;
	for (OS::dirIter_t iter(dir); iter != END; ++iter) {
		const OS::path_t &path = iter->path();

		if (!fs::is_directory(path)) {
			HR_LOG(warning) <<
				"Ignored file in rulebook dir (old rulebook?): " << path;
			continue;
		}

		// We add the rulebook to the library even if it failed to load
		// so that we can display an error to the user in the selection screen.
		auto rulebook = std::make_shared<Rulebook>(scripting, path);
		if (rulebook->LoadMetadata()) rulebooksLoaded++;
		Add(rulebook);
	}

	if (rulebooksLoaded == 0) {
		HR_LOG(error) << "Rulebook path contains no rulebooks: " << dir;
	}
	else {
		HR_LOG(info) << "Loaded " << rulebooksLoaded <<
			" rulebooks(s): " << dir;
	}
}

void RulebookLibrary::Add(std::shared_ptr<Rulebook> &rulebook)
{
	library.emplace(rulebook->GetName(), rulebook);
	sorted.insert(rulebook);

	if (rulebook->GetName() == "Race") {
		defaultRulebook = rulebook;
	}
}

/**
 * Retrieve the default rulebook.
 * This is normally the "Race" rulebook, but if it's not defined, then
 * the first rulebook in sorted order is returned instead.
 * If no rulebooks are loaded, then a dummy rulebook will be returned.
 * @return The default rulebook (never null).
 */
std::shared_ptr<const Rulebook> RulebookLibrary::GetDefault() const
{
	if (defaultRulebook) {
		return defaultRulebook;
	}
	else {
		return library.empty() ?
			std::make_shared<Rulebook>(scripting, OS::path_t()) :
			*sorted.cbegin();
	}
}

/**
 * Search for the Rulebook with the given name.
 * @param name The name to search for (case-sensitive).
 * @return The requested Rulebook, or @c nullptr if
 *         there is no Rulebook with that name.
 */
std::shared_ptr<const Rulebook> RulebookLibrary::Find(const std::string &name)
{
	auto iter = library.find(name);
	return iter == library.end() ? std::shared_ptr<Rulebook>() : iter->second;
}

}  // namespace Client
}  // namespace HoverRace
