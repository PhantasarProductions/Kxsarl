// Lic:
// The Legend of the Kxarl
// Config
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

#include <SlyvStream.hpp>
#include <SlyvGINIE.hpp>
#include <SlyvDirry.hpp>
#include <SlyvQCol.hpp>
#include <SlyvRequestFile.hpp>
#include <SlyvTime.hpp>
#include <SlyvVolumes.hpp>

#include <TQSG.hpp>
#include <TQSE.hpp>

#include "../Headers/Flow.hpp"
#include "../Headers/GlobGFX.hpp"
#include "../Headers/Config.hpp"
#include "../Headers/UseJCR6.hpp"
#include "../Headers/Textboxes.hpp"


using namespace Slyvina;
using namespace Units;
using namespace TQSG;
using namespace TQSE;

namespace Kxsarl {

	std::string Executable{};


	static GINIE _Config{nullptr};

	static GINIE Config() {
		if (!_Config) {
			if (!FileExists(ConfigFile())) {
				QCol->Doing("Creating", ConfigFile());
				SaveString(ConfigFile(), "[Creation]\nDate=" + CurrentDate() + "\nTime=" + CurrentTime());
			}
			QCol->Doing("Loading", ConfigFile());
			_Config = LoadGINIE(ConfigFile(), ConfigFile(), "The Legend Of The Kxarl\n(C) Jeroen P. Broks");
		}
		return _Config;
	}

#pragma region "TextBox Action"
	struct sTBAct {
		std::string
			Error = "",
			Field = "";
		int
			mini = 0,
			maxi = 0;
	};
	std::map<TextBox*, sTBAct> TBAct{};

	void fTBAct(TextBox* o) {
		auto v{ o->IntValue() };
		auto a{ &TBAct[o] };
		a->Error = "";
		if (v < 0)
			a->Error = "Value may noy be negative";
		else if (v < a->mini)
			a->Error = TrSPrintF("Minimal value is %d", a->mini);
		else if (v > a->maxi)
			a->Error = TrSPrintF("Maximal value is %d", a->maxi);
		if (!a->Error.size()) {
			CSay(TrSPrintF("Screen %s is now %d", a->Field.c_str(), v)); 
			Config()->Value("Screen", a->Field, v);
		}
	}
#pragma enregion

	inline void DefaultScreen() {
		Config()->NewValue("Screen", "Windowed", "Yes");
		Config()->NewValue("Screen", "Width", std::to_string(DesktopWidth() - 20));
		Config()->NewValue("Screen", "Height", std::to_string(DesktopHeight() - 60));
	}

	std::string ConfigDir() { return Dirry("$AppSupport$/Kxsarl/"); }

	std::string ConfigFile() {
		auto _D{ ConfigDir() + "Config" }; 
		if (!DirectoryExists(_D)) {
			QCol->Doing("Creating dir", _D);
			MakeDir(_D);
		}
		return _D + "/Config.ini";
	}
	std::string SaveCharDir() {
		Config()->NewValue("Directory", "SavedChars", ConfigDir() + "Characters");
		return AVolPath( Dirry(Config()->Value("Directory", "SavedChars")));
	}
	int CFG_Width() {
		DefaultScreen();
		return Config()->IntValue("Screen","Width");
	}
	int CFG_Height() {
		DefaultScreen();
		return Config()->IntValue("Screen", "Height");
	}
	bool CFG_Windowed() {
		DefaultScreen();
		auto c{ Upper(Config()->Value("Screen", "Windowed")) };
		return c == "YES" || c == "TRUE";
	}

	std::string CFG_ScreenModeString() {
		if (CFG_Windowed())
			return TrSPrintF("%d x %d (Windowed)", CFG_Width(), CFG_Height());
		else
		return TrSPrintF("%d x %d (FullScreen)", DesktopWidth(),DesktopHeight());
	}
	std::string CFG_Lang() {
		Config()->NewValue("Scenario", "Language", "English");
		return Config()->Value("Scenario", "Language");
	}


	void Arrive_Config() {
#ifdef KXSARL_DEBUG
		//for (auto j : MRes()->_Entries) std::cout << j.first << " -> " << j.second->Name() << "\n";
#endif
	}

	bool Flow_Config() {
		static TUImage Background{nullptr}; if (!Background) Background = LoadUImage(MRes(),"GFX/Config/comp024.jpg");
		static TUImage Banner{nullptr}; if (!Banner) Banner = LoadUImage(MRes(),"GFX/Banner/Config.png");
		SetColor(127,127,127);
		Background->Tile(0, 0, ScreenWidth(), ScreenHeight());
		SetColor(255, 255, 255);
		Banner->StretchDraw(0, 0, ScreenWidth(), 104);
		SetColor(0, 180, 255);
		// Screen
		Ryanna()->Text("Screen settings:", 20, 120); //printf("H: %9d\n", Ryanna()->Height("ABCabc")); 
		SetColor(255, 0, 0); MiniFont()->Text("NOTE!", 20, 150); SetColor(255, 255, 255);
		MiniFont()->Text("The base screen setting is 1900x1000. Any other setting is allowed, but could \"misform\" the graphics a bit", 20, 165);
		MiniFont()->Text("Also note that these settings only take effect during the game's main executable start up.", 20, 180);
		MiniFont()->Text("In other words, you'll need to exit the game and start the game again in order for these to take effect!", 20, 195);
		CheckBox(!CFG_Windowed())->StretchDraw(20, 210, 30, 30); Ryanna()->Text("Full Screen",60, 210);
		//QCol->Reset();  printf("Mouse(%d,%d); Start(%d,%d); End(%d,%d); Click: %s\n", MouseX(), MouseY(), ASX(20), ASY(210), ASX(50), ASY(240), boolstring(MouseHit(1)).c_str()); // debug
		if (MouseX() > ASX(20) && MouseX() < ASX(50) && MouseY() > ASY(210) && MouseY() < ASY(240) && MouseHit(1)) {
			Config()->Value("Screen", "Windowed", boolstring(!CFG_Windowed()));
			CSay("Windowed = " + boolstring(CFG_Windowed()));
		}
		Ryanna()->Text("Width:",20,240);
		Ryanna()->Text("Height:", 20, 280);
		static auto TBL{ CreateTextBoxGroup() };
		static auto TBW{ TBL->NewBox(220,250,CFG_Width()) }; TBW->w = 200; TBW->Color(255, 180, 0); TBW->Action = fTBAct;
		static auto TBH{ TBL->NewBox(220,280,CFG_Height()) }; TBH->w = 200; TBH->Color(255, 180, 0); TBH->Action = fTBAct;
		if (!TBAct.count(TBW)) TBAct[TBW] = sTBAct{ "", "Width", 800,ScreenWidth() - 10 };
		if (!TBAct.count(TBH)) TBAct[TBH] = sTBAct{ "", "Height", 600,ScreenHeight() - 100 };
		SetColor(255, 0, 0); 
		Ryanna()->Text(TBAct[TBW].Error, 450, 250);
		Ryanna()->Text(TBAct[TBH].Error, 450, 290);
		TBL->SetEnabled(CFG_Windowed());
		TBL->Draw();

		// Character Folder
		SetColor(0, 180, 255);
		Ryanna()->Text("Saved character data path", 20, 350);
		static auto TBCP{ TBL->NewBox(20, 380,SaveCharDir()) };
		TBCP->enabled = true;
		TBCP->Color(180, 0, 255);
		TBCP->Font = MiniFont();
		/*
		if (MouseX() > ASX(20) && MouseX() < ASX(220) && MouseY() > ASY(400) && MouseY() < ASY(500)) {
			ColLoop();
		} else
			SetColor(255, 255, 255);
		ImgPickButton()->StretchDraw(20, 400, 200, 100);
		//*/

		// Back
		SetColor(255, 255, 255);
		if (MouseX() < ASX(182) && MouseY() > ASY(ScreenHeight() - 100)) {
			SetColorHSV((SDL_GetTicks() / 100) % 360, 1, 1);
			if (MouseHit(1)) GoFlow("MainMenu");
		}
		Arrow(EArrow::Left)->StretchDraw(0, ScreenHeight() - 100, 182, 100);
		return true;
	}
}