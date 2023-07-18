// Lic:
// The Legend of The Kxsarl
// JCR6 User
// 
// 
// 
// (c) Jeroen P. Broks, 2023
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Please note that some references to data like pictures or audio, do not automatically
// fall under this licenses. Mostly this is noted in the respective files.
// 
// Version: 23.07.18
// EndLic
#include <SlyvQCol.hpp>
#include <SlyvString.hpp>

#include <JCR6_JQL.hpp>
#include <JCR6_RealDir.hpp>
#include <JCR6_zlib.hpp>

#include "../Headers/Config.hpp"
#include "../Headers/UseJCR6.hpp"
#include "../Headers/Error.hpp"

using namespace std;

using namespace Slyvina;
using namespace JCR6;

namespace Kxsarl {
	static void Paniek(std::string s) {
		QCol->Error("JCR6 goes in panic");
		map<string, string> d{};
		if (Last() && Last()->MainFile.size()) {
			d["Main File"] = Last()->MainFile;
			d["Entry"] = Last()->Entry;
		}
		Crash(s, d);
	}

	static void InitJCRDrivers() {
		static bool _done{ false };
		if (_done) return;
		QCol->Doing("Init", "JCR6 drivers");
		
		JCR6_InitRealDir();
		InitJQL();
		JCR_InitZlib();
		JCR6PANIC = Paniek;
		_done = true;
	}

	static JT_Dir GetRes(string s) {
		QCol->Doing("Reading JCR6", s);
		return JCR6_Dir(ExeDir() + "/" + s);
	}

	JT_Dir _MRes{ nullptr };
	JT_Dir MRes() {
		InitJCRDrivers();
		if (!_MRes) {
#ifdef KXSARL_DEBUG
			_MRes = GetRes("../Debug_JQL/Assets.jql");
			_MRes->Patch(GetRes("../Debug_JQL/Music.jql"));			
#else
			_MRes = GetRes(StripExt(Executable) + ".jcr");
#endif
		}
		return _MRes;
	}
}