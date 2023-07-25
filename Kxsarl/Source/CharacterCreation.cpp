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
// Version: 23.07.25
// EndLic

#include <TQSE.hpp>

#include <SlyvMath.hpp>
#include <SlyvLinkedList.hpp>

#include <Statistician.hpp>
#include <Statistician_SaveJCR6.hpp>

#include <JCR6_Write.hpp>

#include <map>

#include "AllHeaders.hpp"



using namespace std;
using namespace Slyvina;
using namespace TQSG;
using namespace TQSE;
using namespace Statistician;

namespace Kxsarl {

#define NTY(A) void A() {Crash("Feature not yet implemented");}
	
	static UGINIE Basic{nullptr};

#pragma region "Stage Initiation"
	enum class Stage { Skill, ClassSex, StatRolls, Name };
	static void S_Skill();
	static void S_ClassSex();
	static void StartChar();
	static void S_StatRolls();
	static void StartNameAndPicture();
	static void S_Name();
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
			SetColorHSV((SDL_GetTicks() / 105) % 360, 1, 1);
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
	inline int CSkill() { return Basic->IntValue("Skill", "Skill"); }
	static std::string CSkillName() { switch (CSkill()) { case 1:return "Easy"; case 2:return "Normal"; case 3:return "Hard"; default: return "???"; } }
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
	inline string CClass() { return Basic->Value("GEN", "Class"); }

	class TClass {
	public:
		static unique_ptr<TList<TClass>> Lijst;
		TUImage Banner{ nullptr };
		std::string Name{""};
		VecString Uitleg{ nullptr };
		uint32 y;
		inline TClass(std::string N,string U) {
			Name = N;
			Uitleg = Split(U, '\n');
			Banner = LoadUImage(MRes(), TrSPrintF("GFX/Class/%s.png", N));
			QCol->Doing("Init Class", N);
			y = 120 + (Lijst->Size() * 120);
			(*Lijst) += this;
		}
		inline ~TClass() { QCol->Doing("Destroyed Class", Name); }
	};
	unique_ptr<TList<TClass>> TClass::Lijst{TList<TClass>::CreateUnique()};

	static void S_ClassSex() {

		// Sex
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

			// Class
			static bool initclassdone{ false };
			SetColor(0, 180, 255);
			Ryanna()->Text("Class:", 180, 100);
			if (!initclassdone) {
				initclassdone = true;
				new TClass("Paladin","A knight sworn to good\nHas a decent melee attack and defenses.\nCan also do a tiny bit of healing.");
				new TClass("Warrior","Brute force make the warrior realy useful\nCan also try to distract the enemy from weaker allies");
				new TClass("Mage","Physically weak in both offense and defense\bBut the powerful spells a mage can cast should make it up for it.\nIf you can find the enemy's weaknesses,\nthe mage can be the most powerful killer");
				new TClass("Cleric","Sworn to God, the cleric takes their power from prayer.\nMight not be the strongest with offense, but healing spells can cover that up\nThe cleric also come in handy when dealing with the undead.");
				new TClass("Rogue","Are you more the tactical player?\nThe rogue is faster than the others\nand can poison and paralyze the enemy.\nMay also dodge your enemy's most powerful blows");
				new TClass("Archer","The archer can attack the enemy from a distance\nMay not be handy when the enemy is up close, but if you\nmanage to keep your distance, the archer can work");
			}
			for (auto c = TClass::Lijst->First(); c; c = TClass::Lijst->Next()) {
				if (CClass() == c->Name) {
					ColLoop(98);
					ARect(178, (c->y - 2), c->Banner->Width() + 4, c->Banner->Height() + 4);
				}
				if (MouseX() > ASX(180) && MouseX() < 180 + ASX(c->Banner->Width()) && MouseY() > ASY(c->y) && MouseY() < ASY(c->y + c->Banner->Height())) {
					int ty{ 180 };
					for (auto l : *(c->Uitleg)) {
						SetColor(255, 255, 255, 255);
						MiniFont()->Text(l, 500, ty); ty += 30;
					}
					SetAlphaD(abs(DegSin((double)SDL_GetTicks() / 25)));
					if (MouseHit(1)) {
						Basic->Value("Gen", "Class", c->Name);
					}
				}
				SetColor(255, 255, 255);
				c->Banner->Draw(180, c->y);
				SetAlpha(255);
			}

			// Next
			if (CSex().size() && CClass().size()) {
				static auto nx{ ScreenWidth() - 182 };
				if (MouseX() > ASX(nx) && MouseY() > ASY(ScreenHeight() - 100)) {
					SetColorHSV((SDL_GetTicks() / 97) % 360, 1, 1);
					if (MouseHit(1)) {
						CurrentStage = Stage::StatRolls;
						StartChar();
					}
					
				}
				Arrow(EArrow::Right)->StretchDraw(nx, ScreenHeight() - 100, 182, 100);
			}
		}
	}
#pragma endregion

#pragma region "Stat Rolls"
	static UParty CParty{nullptr};
	static Character CChar{nullptr};
	static void RollBaseStats() {
		Game::GameSkill = CSkill();
		static map<string, map<string, int>> mini {
			{ "Warrior", { { "Power",14 },{"Toughness",10} }},
			{ "Paladin", { { "Will",17},{"Power",12},{"Thoughness",9}} },
			{ "Mage", {{"Intelligence",16},{"Will",16} } },
			{ "Rogue", { {"Power",8},{"Agility",14}, {"Will",9} } },
			{ "Archer",{{"Agility",10},{"Power",9},{"Stamina",9} } }
		};
		for (auto S : StatNames) {
			CChar->Statistic(S)->Base=RollStat(Basic->IntValue("SKILL", "SKILL"),mini[CClass()][S]);
			CSay(TrSPrintF("Rolled %d for stat %s", CChar->Statistic(S)->Base, S.c_str()));
		}
		SetArmorClass(CChar, CClass());
		SetBaseHP(CChar, CClass());
	}

	static void StartChar() {
		CParty = CreateUniqueParty();
		CChar = CParty->NewChar("MAIN");
		CChar->Statistic("Level")->Base = 1;
		RollBaseStats();
	}

	static void S_StatRolls() {
		static auto Reroll{ LoadUImage(MRes(),"GFX/Buttons/Stats/Reroll.png") };
		SetColor(255, 255, 255);
		Ryanna(CSkillName() + " mode / " + CClass() + " " + CSex(), ScreenWidth() / 2, ScreenHeight(), Align::Center, Align::Bottom);
		SetColor(0, 180, 255);
		Ryanna("Character statistics:", 20, 100);
		int y{ 150 };
		for (auto S : StatNames) {
			SetColor(255, 255, 255);
			Ryanna(S, 20, y);
			SetColor(255, 180, 0);
			Ryanna(CChar->Statistic(S)->Base, 300, y);
			y += 50;
		}
		SetColor(255, 255, 255); Ryanna("Armor Class", 20, y);
		SetColor(255, 180, 0); Ryanna(CChar->Statistic("AC")->Total(), 300, y);
		y += 50;
		SetColor(255, 255, 255); Ryanna("Strike Class", 20, y);
		SetColor(255, 180, 0); Ryanna(CChar->Statistic("THAC0")->Total(), 300, y);
		y += 50;
		SetColor(255, 255, 255); Ryanna("Hit Points", 20, y);
		SetColor(255, 180, 0); Ryanna(CChar->Statistic("HP")->Total(), 300, y);
		y += 50;

		SetColor(255, 255, 255);
		Reroll->StretchDraw(20, y, 120, 55);
		if (MouseHit(1) && MouseX() < ASX(140) && MouseY() > ASY(y) && MouseY() < ASY(y + 55))
			RollBaseStats();
		{
			static auto nx{ ScreenWidth() - 182 };
			if (MouseX() > ASX(nx) && MouseY() > ASY(ScreenHeight() - 100)) {
				SetColorHSV((SDL_GetTicks() / 97) % 360, 1, 1);
				if (MouseHit(1)) {
					CurrentStage = Stage::Name;
					StartNameAndPicture();
				}

			}
			Arrow(EArrow::Right)->StretchDraw(nx, ScreenHeight() - 100, 182, 100);
		}
	}
	
#pragma endregion

#pragma region "Name"
	static TextBoxGroup TBG{nullptr};
	static TextBox* TBName{nullptr};
	static VecString Faces{nullptr};
	static uint32 CFace{0};
	static TUImage IFace{nullptr};

	static void StartNameAndPicture(){
		if (!TBG) {
			TBG = CreateTextBoxGroup();
			TBName = TBG->NewBox(20, 150, "");
			TBName->h = Ryanna()->Height("The quick brown fox jumps over the lazy dog");
			TBName->Color(255, 180, 0);
		}
		TBName->value = "";
		Faces = MRes()->Directory("GFX/Faces/" + CSex()); CSay(CSex() + " faces found: " + to_string(Faces->size()));
		CFace = Rand.Get(0, Faces->size() - 1);
		IFace = nullptr;;
	}

	static void S_Name() {
		auto
			PX{ (ScreenWidth() / 2) - 250 },
			PY{ (ScreenHeight() / 2) - 250 };
		SetColor(0, 180, 255);
		Ryanna("Name your " + Lower(CSex()) + " " + Lower(CClass())+":", 20, 120);
		TBG->Draw(); TBName->value = Trim(TBName->value);
		if (!IFace) {
			CSay(TrSPrintF("Loading: %s (%d/%d)", ((*Faces)[CFace]).c_str(), CFace, Faces->size()));
			IFace = LoadUImage(MRes(), (*Faces)[CFace]);
		}
		SetColor(255, 255, 255);
		IFace->StretchDraw(PX, PY, 500, 500);
		if (MouseHit(1) && MouseX() > ASX(PX) && MouseX() < ASX(PX + 500) && MouseY() > ASY(PY) && MouseY() < ASY(500 + PY)) {
			IFace = nullptr;
			CFace = (CFace + 1) % Faces->size();
		}
		if (Trim(TBName->value).size()) {
			static auto nx{ ScreenWidth() - 182 };
			if (MouseX() > ASX(nx) && MouseY() > ASY(ScreenHeight() - 100)) {
				SetColorHSV((SDL_GetTicks() / 97) % 360, 1, 1);
				if (MouseHit(1)) {					
					CChar->Name = Trim(TBName->value);
					CChar->Data["Class"] = CClass();
					CChar->Data["Sex"] = CSex();
					CChar->BStat["Skill"] = CSkill();
					CChar->BStat["Experience"] = 0;
					CChar->BStat["Level"] = 1;
					SysStatus("Saving");
					if (!DirectoryExists(SaveCharDir())) {
						QCol->Doing("Creating dir", SaveCharDir());
						MakeDir(SaveCharDir());
					}
					int CID{ 0 };
					std::string OutDir;
					do { OutDir = TrSPrintF("%s/CHAR_%08x", SaveCharDir().c_str(), CID++); } while (DirectoryExists(OutDir));
					QCol->Doing("Creating dir", OutDir);
					MakeDir(OutDir);
					auto JO{ JCR6::CreateJCR6(OutDir + "/General.jcr","zlib") };
					CSay("Base"); JO->AddString(Basic->UnParse(), "Base.ini", "zlib");
					CSay("Face");  JO->AddBank(MRes()->B((*Faces)[CFace]), "Face.png");
					CSay("Party");  Statistician::StatSaveJCR6(CParty.get(), JO, "Char", "Store");
					CSay("Finalize"); JO->Close();
					//Crash("No next stage set yet");
					GoFlow("MainMenu");
				}

			}
			Arrow(EArrow::Right)->StretchDraw(nx, ScreenHeight() - 100, 182, 100);
		}
	}
#pragma endregion



}