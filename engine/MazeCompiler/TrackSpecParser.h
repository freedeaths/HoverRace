// TrackSpecParser.h
// Text file parser helper
//
// Copyright (c) 1995-1998 - Richard Langlois and Grokksoft Inc.
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

#if defined(_WIN32) && defined(HR_ENGINE_SHARED)
#	ifdef MR_ENGINE
#		define MR_DllDeclare   __declspec( dllexport )
#	else
#		define MR_DllDeclare   __declspec( dllimport )
#	endif
#else
#	define MR_DllDeclare
#endif

namespace HoverRace {
namespace MazeCompiler {

class MR_DllDeclare TrackSpecParser : boost::noncopyable
{
	protected:
		std::istream &in;

		char mLineBuffer[150];
		char mReturnBuffer[30];
		char *mParsePtr;
		int mLineNumber;

		bool ReadNewLine();
		const char *InternalGetNextClass();

	public:
		TrackSpecParser(std::istream &in);
		~TrackSpecParser();

		void Reset();

		const char *GetNextClass(const char *pClassType = NULL);
												  // If the line doesn't begin by an attrib, you should use GetNextLine
		const char *GetNextAttrib(const char *pAttrib = NULL);
		bool GetNextLine();
		const char *GetNextStrParam(const char *pDefaultValue = NULL);
		double GetNextNumParam(double DefaultValue = 0.0);
		const char *GetParams();

		int GetErrorLine() const;

};

}  // namespace MazeCompiler
}  // namespace HoverRace

#undef MR_DllDeclare
