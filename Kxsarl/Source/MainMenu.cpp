// Lic:
// The Legend Of The Kxsarl
// Main Menu
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
// Version: 23.07.22
// EndLic

#include <SlyvOpenURL.hpp>

#include <TQSG.hpp>
#include <TQSE.hpp>

#include <SlyvLinkedList.hpp>

#include "../Headers/Error.hpp"
#include "../Headers/UseJCR6.hpp"
#include "../Headers/MainMenu.hpp"
#include "../Headers/GlobGFX.hpp"
#include "../Headers/Flow.hpp"
#include "../Headers/Music.hpp"

using namespace std;
using namespace Slyvina;
using namespace Units;
using namespace TQSG;
using namespace TQSE;

namespace Kxsarl {

	const char* MainMenuMusic = "music/mainmenu/tempest.mp3";

	static unique_ptr<TList<MainMenuItem>> MenuItems{TList<MainMenuItem>::CreateUnique() };
	static bool AlwaysAllowed(MainMenuItem*) { return true; }

#pragma region "Main menu specific graphics"	
	static TUImage Background{nullptr};
	static TUImage Title{nullptr};
	static TUImage GPL{nullptr};

	static TUImageFont Fantasy{nullptr};
#pragma endregion

#pragma region "Exit the game through the main menu"
	static bool Running{ true };
	static void ActExit(MainMenuItem*) { Running = false; }
#pragma endregion

#pragma region "Action Functions"
	static void ActCharGen(MainMenuItem*) { GoFlow("CharGen"); }
	static void ActConfig(MainMenuItem*) { GoFlow("Config"); }
	static void ActBugs(MainMenuItem*) { OpenURL("https://github.com/PhantasarProductions/Kxsarl/issues"); QCol->Doing("Opening", "Issue tracker on GitHub"); }
#pragma endregion

	MainMenuItem::MainMenuItem(std::string _Caption, void (*_Action)(MainMenuItem*) , bool (*_Allow)(MainMenuItem*) ) {
		Caption = _Caption;
		Action = _Action;
		if (_Allow)
			Allow = _Allow;
		else
			Allow = AlwaysAllowed;
		MenuItems->AddLast(this);
		CSay("Item " + _Caption + " added to main menu");
	}

	void MainMenuItem::InitMenuItems() {
		static bool _done{ false }; if (_done) return; _done = true;
		new MainMenuItem("Character Creation",ActCharGen);
		new MainMenuItem("Start Game");
		new MainMenuItem("Jukebox");
		new MainMenuItem("Report Bugs", ActBugs);
		new MainMenuItem("System Configuration",ActConfig);
		new MainMenuItem("Transfer");
		new MainMenuItem("Exit", ActExit);
	}
	

	void Arrive_MainMenu() {
		if (!Title) { Title = LoadUImage(MRes(), "GFX/Banner/Title.png"); Title->Hot(Title->Width() / 2, 0); }
		if (!Background) Background = LoadUImage(MRes(), "GFX/MainMenu/Background.png");
		if (!GPL) { GPL = LoadUImage(MRes(), "GFX/MainMenu/GPL.png"); GPL->Hot(0, GPL->Height()); }
		if (!Fantasy) { Fantasy = LoadUImageFont(MRes(), "Fonts/Fantasy.jfbf"); }
		MainMenuItem::InitMenuItems();
		Music(MainMenuMusic);
		
	}

	bool Flow_MainMenu() {
		SetColor(50, 50, 50);
		Background->Tile(0, 0, ScreenWidth(), ScreenHeight());
		SetColor(255, 255, 255);
		Title->Draw(ScreenWidth() / 2, 25);
		GPL->Draw(20, ScreenHeight() - 20);
		int miy{ 120 };
		for (auto mi = MenuItems->First(0); mi; mi = MenuItems->Next(0)) {
			// CSay(mi->Caption); // Debug
			if (MouseY() > ASY(miy) && MouseY() < ASY(miy + 95) && mi->Allow(mi)) {
				SetColorHSV((SDL_GetTicks() / 100) % 360, 1, 1);
				if (MouseHit(1)) {
					if (mi->Action)
						mi->Action(mi);
					else
						Crash("There is no action set for menu item \"" + mi->Caption + "\"");
				}
			} else if (mi->Allow(mi))
				SetColor(255, 255, 255);
			else
				SetColor(120, 120, 120);
			Fantasy->Text(mi->Caption, ScreenWidth() / 2, miy, Align::Center, Align::Top);
			miy += 95;
		}
		SetColor(255, 255, 255);
		//printf("Debug: TextHeight=%d\n", ChiqueFont()->Height("Copyright  Jeroen P. Broks, 2023, licened under the GPL3")); // debug
		ChiqueFont()->Text("Copyright |cr Jeroen P. Broks, 2023, licened under the GPL3", ScreenWidth() / 2, ScreenHeight() - 10, Align::Center, Align::Bottom);
		//printf("Debug: Drawn\n");
		return Running;
	}
}