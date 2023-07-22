// Lic:
// The legend of the Kxsarl
// Character Creation
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

#include <TQSE.hpp>

#include <SlyvMath.hpp>

#include <map>

#include "AllHeaders.hpp"



using namespace std;
using namespace Slyvina;
using namespace TQSG;
using namespace TQSE;

namespace Kxsarl {

#define NTY(A) void A() {Crash("Feature not yet implemented");}
	
	static UGINIE Basic{nullptr};

#pragma region "Stage Initiation"
	enum class Stage { Skill, ClassSex, StatRolls, Name };
	static void S_Skill();
	static void S_ClassSex();
	void S_StatRolls();
	void S_Name();
	map<Stage, void (*)()> StageMap{
		{Stage::Skill, S_Skill},
		{ Stage::ClassSex,S_ClassSex },
		{ Stage::StatRolls,S_StatRolls },
		{ Stage::Name,S_Name }
	};
	Stage CurrentStage{ Stage::Skill };
#pragma endregion

#pragma region "Flow initiation and execution"
	bool Flow_CharacterCreation() {
		static TImage Banner;
		SetColor(255, 255, 255);
		if (!Banner) {
			QCol->Doing("Loading", "Character Creation Banner");
			Banner = LoadImage(MRes(),"GFX/Banner/Character Creation.png");
			if (!Banner) Crash("Banner not loaded properly", { {"JCR6",JCR6::Last()->ErrorMessage},{"TQSG",TQSG::LastError()} });
			Banner->HotCenter();
		}
		DrawBackground();
		int tm{ (SDL_GetTicks() / 50) % 360 };
		double fx{ DegSin(tm) };
		//printf("tm: %03d; fx: %09.6f\n", tm,fx);
		SetScale(1+abs(fx/4));
		Rotate(5*fx);

		Banner->XDraw(ScreenWidth() / 2, 60);
		SetScale(1);
		if (MouseX() < ASX(182) && MouseY() > ASY(ScreenHeight() - 100)) {
			SetColorHSV((SDL_GetTicks() / 101) % 360, 1, 1);
			if (MouseHit(1)) {
				if (CurrentStage == Stage::Skill)
					GoFlow("MainMenu");
				else
					CurrentStage = (Stage)((int)CurrentStage - 1);				
			}
		}
		Arrow(EArrow::Left)->StretchDraw(0, ScreenHeight() - 100, 182, 100);

		if (!StageMap.count(CurrentStage)) Crash("Internal error - Please report!", { {"Error",TrSPrintF("Unknown stage identifier (%d)",(int)CurrentStage)} });
		StageMap[CurrentStage]();
		return true;
	}

	void Arrive_CharCreation() {
		CurrentStage = Stage::Skill;
		Basic = ParseUGINIE(TrSPrintF("[Creation]\nCHARGEN_BUILD=%s; %s\nDATE=%s\TIME=%s", __DATE__, __TIME__,CurrentDate().c_str(),CurrentTime().c_str()),"","# Basic character data!");
	}
#pragma endregion

#pragma region "Skill Selection"
	static void S_Skill() {
		static TUImage SkI[3]{ nullptr,nullptr,nullptr };
		static int CX{ ScreenWidth() / 2 };
		static const string SkX[3]{ "This mode is for beginners or people who are not very good in dungeon crawlers","This is for people want the game the way it was meant to be","You think all games are too easy!? Well, then this is the mode for you!" };
		static int CSX{ ScreenWidth() / 4 };
		SetColor(255, 180, 0);
		Ryanna()->Text("Choose the game difficulty:", CX, 100, Align::Center);
		SetColor(255, 255, 255);
		MiniFont()->Text("What is VERY important to note is that the difficulty settings do more than just", CX, 150, Align::Center);
		MiniFont()->Text("making monsters more or less agressive. Inventory space can be bigger or smaller", CX, 180, Align::Center);
		MiniFont()->Text("your character can be stronger or weaker, experience gains can go faster or slower", CX, 210, Align::Center);
		MiniFont()->Text("and many more parameters will be affected by what you choose here.", CX, 240, Align::Center);
		MiniFont()->Text("As such, once chosen, you cannot change this setting anymore, so choose wisely!", CX, 300, Align::Center);
		for (int i = 0; i < 3; i++) {
			int i1{ i + 1 };
			if (!SkI[i]) {
				char fname[35]; 
				sprintf_s(fname, "GFX/Buttons/Skill/Skill%d.png", i+1);
				QCol->Doing("Loading", fname);
				SkI[i] = LoadUImage(MRes(), fname);
				if (!SkI[i]) Crash("Loading skill button failed", { { "Button",to_string(i) },{"File",fname} });
				SkI[i]->HotCenter();
				break;
			}
			if (MouseX() > ASX((CSX*i1) - 100) && MouseX() < ASX((CSX*i1) + 100) && MouseY() > ASY(350) && MouseY() < ASY(450)) {
				MiniFont()->Text(SkX[i], CX, 550, Align::Center);
				SetScale(1.25);
				if (MouseHit(1)) {
					Basic->Value("SKILL", "SKILL", i1);
					CurrentStage = Stage::ClassSex;
				}
			}
			SkI[i]->Draw(CSX * (i+1), 400);
			SetScale(1);
		}
	}
#pragma endregion

#pragma region "Class & Sex"
	struct TSex {
		TUImage Symbol{ nullptr };
		string name{ "???" };
	};
	inline string CSex() { return Basic->Value("Gen", "Sex"); }
	static void S_ClassSex() {
		static TSex Sex[2]{ {LoadUImage(MRes(),"GFX/Sex/Male.png"),"Male"},{LoadUImage(MRes(),"GFX/Sex/Female.png"),"Female"} };
		SetColor(0, 180, 255);
		Ryanna()->Text("Gender:", 20, 100);
		for (int gi = 0; gi < 2; gi++) {
			auto g{ &Sex[gi] };
			auto y{ 140 + (gi * 120) };
			if (CSex() == g->name) {
				ColLoop();
				ARect(18, (y-2), 104, 104);
			}
			SetColor(255, 255, 255);
			if (MouseX() < 120 && MouseY() > y && MouseY() < y + 100) {
				SetAlphaD(abs(DegSin((double)SDL_GetTicks() / 25)));
				if (MouseHit(1)) {
					Basic->Value("Gen", "Sex", g->name);
					CSay("Player chose gender: " + g->name);
				}
			}
			g->Symbol->StretchDraw(20, y,100,100);
			SetAlpha(255);
		}
	}
#pragma endregion

#pragma region "Stat Rolls"
	NTY(S_StatRolls);
#pragma endregion

#pragma region "Name"
	NTY(S_Name);
#pragma endregion



}