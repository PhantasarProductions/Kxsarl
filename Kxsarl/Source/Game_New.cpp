// Lic:
// The Legend of the Kxsarl
// New Game
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
// Version: 23.07.29
// EndLic

#include "AllHeaders.hpp"

using namespace std;

namespace Kxsarl {
	namespace Game {

#pragma region Vars
		static string _CHID{ "" };
		static UGINIE GameLists{ParseUGINIE("[Nothing]\nNothing=Nothing")};
#pragma endregion

#pragma region "Flow"
		static bool FlowPickGame() {
			return true;
		}
#pragma endregion
		
#pragma region "Chain"
		static bool LoadGameLists() {
			QCol->Doing("Loading", "Game Lists");
			auto l{ MRes()->Directory("Game",false) };
			for (auto i : *l) {
				if (Prefixed(Upper(i), "LIST") && ExtractExt(Upper(i)) == "INI") {
					QCol->Doing("=>", i);
					GameLists->Parse(MRes(i), true);
				}
			}
			return true;
		}

		void PickGame(std::string CHID){
			bool GameListsLoaded{ LoadGameLists() };
			_CHID = CHID;
			GoFlow(FlowPickGame);
		}
	}
#pragma endregion
}