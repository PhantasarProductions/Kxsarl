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
// Version: 23.07.31
// EndLic

#include <TQSE.hpp>

#include "AllHeaders.hpp"

using namespace std;
using namespace Slyvina;
using namespace TQSG;
using namespace TQSE;

namespace Kxsarl {
	namespace Game {

#pragma region Vars
		static string _CHID{ "" };
		static UGINIE GameLists{ParseUGINIE("[Nothing]\nNothing=Nothing")};
#pragma endregion

#pragma region "Create a new game"
		static void NewGame(std::string _GameID) {
			auto StartIniSrc{ MRes("Game/" + _GameID + "/NewGame.ini") };
			auto StartIni{ ParseUGINIE(StartIniSrc) };
			GameID = _GameID;

		}
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
				if (MouseY() > ASY(IY) && MouseY() < ASY(IY + 50)) {
					ColLoop(250); SetAlpha(95);
					ARect(10, IY, ScreenWidth() - 20, 50);
					SetAlpha(255);
					if (MouseHit(1)) {
						QCol->Doing("Player chose", ID);
						NewGame(ID);
					}
				}
				if (Y >= 0 && Y <= 18) {
					Ryanna(GameTitle, 20, IY);
					MiniFont(GameDesc, 20, IY + 30);
				}
			}
			// Face
			SetColor(255, 255, 255, 255);
			static auto GEX{ ScreenWidth() - 70 };
			Game::FaceHero->StretchDraw(GEX, 50, 50, 50);
			Ryanna(Game::GameBase->Value("Gen", "Name"), GEX - 10, 50, Align::Right);
			MiniFont(Game::GameBase->Value("Gen", "Sex") + " " + Game::GameBase->Value("Gen", "Class"), GEX - 1, 80, Align::Right);

			// Back
			SetColor(255, 255, 255);
			if (MouseX() < ASX(182) && MouseY() > ASY(ScreenHeight() - 100)) {
				SetColorHSV((SDL_GetTicks() / 100) % 360, 1, 1);
				if (MouseHit(1)) GoFlow("ChooseChar");
			}
			Arrow(EArrow::Left)->StretchDraw(0, ScreenHeight() - 100, 182, 100);
			return true;
		}
#pragma endregion
		
#pragma region "Chain"
		static bool LoadGameLists() {
			QCol->Doing("Loading", "Game Lists");
			auto l{ MRes()->Directory("Game",false) };
			for (auto i : *l) {
				if (Prefixed(Upper(StripDir(i)), "LIST") && ExtractExt(Upper(i)) == "INI") {
					QCol->Doing("=>", i);
					GameLists->Parse(MRes(i), true);
#ifdef KXSARL_DEBUG
					auto lst{ GameLists->List("Game","List") };
					for (auto l : *lst)
						QCol->Doing("  =>", l);
#endif
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