// Lic:
// The Legend of the Kxsarl
// Game Config
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
// Version: 23.07.30
// EndLic
#include "AllHeaders.hpp"

using namespace Slyvina;
using namespace TQSG;
using namespace std;

namespace Kxsarl {
	namespace Game {
		byte GameSkill{ 0 };
		UGINIE GameBase{ nullptr };
		TImage FaceHero{ nullptr };
		std::string CharID{""};


		void LoadGameBase(string CHID) {
			auto J{ JCR6::JCR6_Dir(SaveCharDir() + "/" + CHID + "/General.jcr") };
			GameBase = ParseUGINIE(J->GetString("Base.ini"));
			FaceHero = LoadImage(J, "Face.png");
			CharID = CHID;
			CSay("Base Loaded\t" + GameBase->Value("Gen", "Name"));
		}

		int64 ExpNeeded(byte Level, string ChClass) {
			Trans2Upper(ChClass);
			int64 minresult{ 5 }, maxresult{ 2000000000 };
			int64 base{ 5 }, factor{ 2 };
			switch (GameSkill) {
			case 0:
			case 1:
				minresult = 5;
				maxresult = 1000000000;
				base = 4;
				if (ChClass == "MAGE") { base = 2;	factor = 5; }
				if (ChClass == "ROGUE" || ChClass == "ARCHER") { base = 3; factor = 2; }
				if (ChClass == "CLERIC") { base = 1; factor = 3; }
				break;
			case 2:
				if (ChClass == "MAGE") { base = 3;	factor = 5; }
				if (ChClass == "ROGUE" || ChClass == "ARCHER") { base = 3; factor = 3; }
				if (ChClass == "CLERIC") { base = 4; factor = 3; }
				minresult = 10;
				break;
			default:
				if (ChClass == "MAGE") { base = 5;	factor = 5; }
				if (ChClass == "ROGUE" || ChClass == "ARCHER") { base = 3; factor = 2; }
				if (ChClass == "CLERIC") { base = 4; factor = 4; }
				minresult = 30;
				break;
			}
			auto ret{ base };
			for (byte l = 1; l < Level; l++) {
				// Slow, but prevents trouble with overflowing
				for (byte m = 1; m <= factor; m++) {
					auto mf{ ret };
					if (maxresult - ret < mf) return maxresult;
					ret += mf;
				}
			}
			return ret;
		}
	}
}