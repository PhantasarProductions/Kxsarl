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
// Version: 23.07.28
// EndLic
#include "AllHeaders.hpp"

using namespace Slyvina;
using namespace TQSG;

namespace Kxsarl {
	namespace Game {
		byte GameSkill{ 0 };
		UGINIE GameBase{ nullptr };
		TImage FaceHero{ nullptr };


		void LoadGameBase(std::string CHID) {
			auto J{ JCR6::JCR6_Dir(SaveCharDir() + "/" + CHID + "/General.jcr") };
			GameBase = ParseUGINIE(J->GetString("Base.ini"));
			FaceHero = LoadImage(J, "Face.png");
			CSay("Base Loaded\t" + GameBase->Value("Gen", "Name"));
		}
	}
}