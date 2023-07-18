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
// Version: 23.07.18
// EndLic

#include <TQSG.hpp>

#include <SlyvLinkedList.hpp>

#include "../Headers/UseJCR6.hpp"
#include "../Headers/MainMenu.hpp"

using namespace std;
using namespace Slyvina;
using namespace Units;
using namespace TQSG;

namespace Kxsarl {
	static unique_ptr<TList<MainMenuItem>> MenuItems{TList<MainMenuItem>::CreateUnique() };
	static bool AlwaysAllowed(MainMenuItem*) { return true; }
	
	static TUImage Background{nullptr};
	static TUImage Title{nullptr};
	static TUImage GPL{nullptr};


	MainMenuItem::MainMenuItem(std::string _Caption, void (*_Action)(MainMenuItem*) , bool (*_Allow)(MainMenuItem*) ) {
		Caption = _Caption;
		Action = _Action;
		if (_Allow)
			Allow = _Allow;
		else
			Allow = AlwaysAllowed;
		(*MenuItems) += this;
	}

	void MainMenuItem::InitMenuItems() {
		new MainMenuItem("Character Creation");
		new MainMenuItem("Start Game");
		new MainMenuItem("Jukebox");
		new MainMenuItem("Report bugs");
		new MainMenuItem("System configuration");
		new MainMenuItem("Transfer");
		new MainMenuItem("Exit");
	}
	

	void Arrive_MainMenu() {
		if (!Title) { Title = LoadUImage(MRes(), "GFX/Banner/Title.png"); Title->Hot(Title->Width() / 2, 0); }
		if (!Background) Background = LoadUImage(MRes(), "GFX/MainMenu/Background.png");
		if (!GPL) { GPL = LoadUImage(MRes(), "GFX/MainMenu/GPL.png"); GPL->Hot(0, GPL->Height()); }
		
	}

	bool Flow_MainMenu() {
		SetColor(50, 50, 50);
		Background->Tile(0, 0, ScreenWidth(), ScreenHeight());
		SetColor(255, 255, 255);
		Title->Draw(ScreenWidth() / 2, 25);
		GPL->Draw(20, ScreenHeight() - 20);
		return true;
	}
}