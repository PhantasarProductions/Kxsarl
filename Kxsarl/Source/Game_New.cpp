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
using namespace Slyvina;
using namespace TQSG;

namespace Kxsarl {
	namespace Game {

#pragma region Vars
		static string _CHID{ "" };
		static UGINIE GameLists{ParseUGINIE("[Nothing]\nNothing=Nothing")};
#pragma endregion

#pragma region "Flow"
		static bool FlowPickGame() {
			static int ScrollY{ 0 };		
			static auto Lijst{ GameLists->List("Game","List") };
			auto Banner{ LoadUImage(MRes(),"GFX/Banner/Pick a Game.png") }; 
			Banner->HotCenter();
			SetColor(255, 255, 255);
			DrawBackground();
			Banner->Draw(ScreenWidth() / 2, 50);
			for (int i = 0; i < GameLists->ListCount("Game", "List"); i++) {
				int 
					Y{ i - ScrollY }, 
					IY{ (Y * 50) + 100 };
				auto
					ID{ (*Lijst)[i] },
					GameTitle{ GameLists->Value(ID,"Title") },
					GameDesc{ GameLists->Value(ID,"Desc") };
				if (Y >= 0 && Y <= 18) {
					Ryanna(GameTitle, 20, IY);
					MiniFont(GameDesc, 20, IY + 30);
				}
			}
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