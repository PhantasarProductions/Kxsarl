// Lic:
// The legend of the Kxsarl
// Armor Class Calculation
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
#include "AllHeaders.hpp"

using namespace std;
using namespace Slyvina;
using namespace Statistician;
using namespace Kxsarl::Game;

#undef ACDEBUG

#ifdef ACDEBUG
#include <iostream>
#define Chat(A) cout << "\x1b[33mAC>\x1b[0m "<<A<<endl;
#else
#define Chat(A) 
#endif

namespace Kxsarl {
#pragma region "Calculate AC per class"
	static void AC_Paladin(_Stat* S, std::string script) {
		auto Ch{ S->GetCharacter() };
		double Base = 8 * GameSkill; Chat("Base: " << Base<< "\t(Skill: "<<GameSkill<<")");
		double Level{ (double)Ch->Statistic("Level")->Total() }; Chat("Level: " << Level);
		double Tough{ (double)Ch->Statistic("Toughness")->Total() }; Chat("Toughness: " << Tough);
		double Will{ (double)Ch->Statistic("Will")->Total() }; Chat("Will: " << Will);
		double Stat{ 0 };
		Stat += (Tough + (Level/20)); 
		Stat += (Will + (Level/20) );
		Chat("Raw Stat: " << Stat);
		Stat /= 4;
		Stat -= GameSkill;
		Chat("Half Stat: " << Stat);
		Ch->Statistic("AC")->Base = (int)ceil(Base - Stat);
		Chat("AC: " << Ch->Statistic("AC")->Base);
	}

	static void AC_Warrior(_Stat* S, std::string script) {
		auto Ch{ S->GetCharacter() };
		double Base = 7 * GameSkill; Chat("Base: " << Base << "\t(Skill: " << GameSkill << ")");
		double Level{ (double)Ch->Statistic("Level")->Total() }; Chat("Level: " << Level);
		double Tough{ (double)Ch->Statistic("Toughness")->Total() }; Chat("Toughness: " << Tough);
		double Power{ (double)Ch->Statistic("Power")->Total() }; Chat("Power: " << Tough);
		double Stat{ 0 };
		Stat += ((Tough / 18) * 6) + (Level / 18);
		Stat += ((Power / 18) * 2) + (Level / 25);
		Ch->Statistic("AC")->Base = (int)ceil(Base - Stat);

	}

	static void AC_Mage(_Stat* S, std::string script) {
		auto Ch{ S->GetCharacter() };
		Ch->Statistic("MageCloth")->Mini(3 - GameSkill);
		double Level{ (double)Ch->Statistic("Level")->Total() }; Chat("Level: " << Level);
		Ch->Statistic("AC")->Base = 15 - Ch->Statistic("MageCloth")->Total() - (Ch->Statistic("Level")->Total() / (10 * GameSkill))-(Ch->Statistic("Will")->Total()*(Level/(10*GameSkill)));
	}


	static void AC_Cleric(_Stat* S, std::string script) {
		auto Ch{ S->GetCharacter() };
		double Level{ (double)Ch->Statistic("Level")->Total() }; Chat("Level: " << Level);
		double Tough{ (double)Ch->Statistic("Toughness")->Total() }; Chat("Toughness: " << Tough);
		double Power{ (double)Ch->Statistic("Power")->Total() }; Chat("Power: " << Tough);
		double Intelligence{ (double)Ch->Statistic("Power")->Total() }; Chat("Power: " << Tough);
		double Total{ (Tough * 3) + (Intelligence * 2) + Power };
		double Avg{ Total / 6 };
		double Mod{ Avg * (Level / 20) };
		Ch->Statistic("AC")->Base = (7 * GameSkill) - Mod;
	}

	static void AC_Rogue(_Stat* S, std::string script) {
		auto Ch{ S->GetCharacter() };
		double Level{ (double)Ch->Statistic("Level")->Total() }; Chat("Level: " << Level);
		double Agility{ (double)Ch->Statistic("Agility")->Total() }; Chat("Agility: " << Tough);
		double Tough{ (double)Ch->Statistic("Toughness")->Total() }; Chat("Toughness: " << Tough);
		double Mod{ ((Agility / 18) * 4) - (Tough / 72) + (Level / 25) };
		Ch->Statistic("AC")->Base = 9+GameSkill-Mod;
	}

	static void AC_Archer(_Stat* S, std::string script) {
		auto Ch{ S->GetCharacter() };
		double Level{ (double)Ch->Statistic("Level")->Total() }; Chat("Level: " << Level);
		double Agility{ (double)Ch->Statistic("Agility")->Total() };
		double Tough{ (double)Ch->Statistic("Toughness")->Total() }; 
		double Stamina{ (double)Ch->Statistic("Stamina")->Total() };
		double Will{ (double)Ch->Statistic("Will")->Total() };
		double Total{ (Agility * 4) + (Tough * 3) + (Will * 2) + Stamina };
		double Avg{ Total / 10 };
		double Mod{ Avg * (Level / 20) };
		Ch->Statistic("AC")->Base = (8 * GameSkill) - Mod;
	}

	static std::map<std::string, StatScript> AC_PerClass{
		{"PALADIN", AC_Paladin},
		{"WARRIOR",AC_Warrior},
		{"MAGE",AC_Mage},
		{"CLERIC",AC_Cleric},
		{"ROGUE",AC_Rogue},
		{"ARCHER",AC_Archer}
	};

#pragma endregion

#pragma region "Calculate THAC0 per class"
	static void THAC0_Paladin(_Stat* S, std::string script) {
		auto Ch{ S->GetCharacter() };
		double Base{ 20 };
		double PW1{ (double) Ch->Statistic("Power")->Total()*3 };
		double PW2{ (double) Ch->Statistic("Intelligence")->Total()*2};
		double PW3{ (double) Ch->Statistic("Level")->Total() };
		double AVG{ (PW1 + PW2 + PW3) / 6 };
		Ch->Statistic("THAC0")->Base = (int)(Base - (AVG / 3));
	}

	static void THAC0_Warrior(_Stat* S, std::string script) {
		auto Ch{ S->GetCharacter() };
		double Base{ 20 };
		double PW1{ (double) Ch->Statistic("Power")->Total() * 5 };
		double PW2{ (double) Ch->Statistic("Level")->Total() };
		double AVG{ (PW1 + PW2) / (3 + GameSkill) };
		Ch->Statistic("THAC0")->Base = (int)(Base - (AVG / 3));
	}

	static void THAC0_Mage(_Stat* S, std::string script) {
		auto Ch{ S->GetCharacter() };
		double Base{ 20 };
		double PW1{ (double) Ch->Statistic("Intelligence")->Total() * 5 };
		double PW2{ (double) Ch->Statistic("Will")->Total() * 5 };
		double PW3{ (double) Ch->Statistic("Level")->Total() };
		double AVG{ (PW1 + PW2 + PW3) / 12 };
		Ch->Statistic("THAC0")->Base = (int)(Base - (AVG / 3));
	}

	static void THAC0_Cleric(_Stat* S, std::string script) {
		auto Ch{ S->GetCharacter() };
		double Base{ 20 };
		double PW1{ (double) Ch->Statistic("Power")->Total() * 5 };
		double PW2{ (double) Ch->Statistic("Intelligence")->Total() * 2.5 };
		double PW3{ (double) Ch->Statistic("Will")->Total() * 1.5 };
		double PW4{ (double) Ch->Statistic("Level")->Total() };
		double AVG{ (PW1 + PW2 + PW3 + PW4) / 9 };
		Ch->Statistic("THAC0")->Base = (int)(Base - (AVG / 3));
	}

	static void THAC0_Rogue(_Stat* S, std::string script) {
		auto Ch{ S->GetCharacter() };
		double Base{ 20 };
		double PW1{ (double)Ch->Statistic("Agility")->Total() * 5 };
		double PW2{ (double)Ch->Statistic("Power")->Total() * 2.5 };
		double PW3{ (double)Ch->Statistic("Stamina")->Total() * 1.5 };
		double PW4{ (double)Ch->Statistic("Level")->Total() };
		double AVG{ (PW1 + PW2 + PW3 + PW4) / 9 };
		Ch->Statistic("THAC0")->Base = (int)(Base - (AVG / 3));
	}

	static void THAC0_Archer(_Stat* S, std::string script) {
		auto Ch{ S->GetCharacter() };
		double Base{ 20 };
		double PW1{ (double)Ch->Statistic("Stamina")->Total() * 6 };
		double PW2{ (double)Ch->Statistic("Power")->Total() * 1.5 };
		double PW3{ (double)Ch->Statistic("Agility")->Total() * 1.5 };
		double PW4{ (double)Ch->Statistic("Level")->Total() };
		double AVG{ (PW1 + PW2 + PW3 + PW4) / 9 };
		Ch->Statistic("THAC0")->Base = (int)(Base - (AVG / 3));
	}

	static std::map<std::string, StatScript> THAC0_PerClass{
		{ "PALADIN", THAC0_Paladin},
		{ "WARRIOR",THAC0_Warrior },
		{ "MAGE",THAC0_Mage },
		{ "CLERIC",THAC0_Cleric },
		{ "ROGUE",THAC0_Rogue },
		{ "ARCHER",THAC0_Archer }
	};
#pragma endregion

	void SetArmorClass(Character Ch, std::string CClass) {
		auto AC{ Ch->Statistic("AC") };
		auto THAC0{ Ch->Statistic("THAC0") };
		Trans2Upper(CClass);
		AC->Maxi(10);
		AC->Mini(-10);
		if (!AC_PerClass.count(CClass)) throw std::runtime_error("AC cannot be set up for class: " + CClass);
		if (!THAC0_PerClass.count(CClass)) throw std::runtime_error("THAC0 cannot be set up for class: " + CClass);
		AC->StatScriptFunction = AC_PerClass[CClass];
		THAC0->Mini(1 + GameSkill);
		THAC0->Maxi(16 + GameSkill);
		THAC0->StatScriptFunction = THAC0_PerClass[CClass];
	}


#pragma region "Base HP"
	void SetBaseHP(Slyvina::Statistician::Character Ch, std::string CClass, byte Level) {		
		byte die = 6;
		int16 HP = 0;
		Trans2Upper(CClass);
		if (CClass == "MAGE") {
			HP = Ch->TStat("Stamina");
			die = 4;
		} else if (CClass == "CLERIC") {
			HP = Ch->TStat("Stamina") + Rand.Get(1, 10 - (GameSkill * 2));
			if (GameSkill == 1) die = 6;
			die = 4;
		} else if (CClass == "ROGUE" || CClass=="ARCHER") {
			HP = Ch->TStat("Stamina") + Rand.Get(1, 6);
			die = 6;
		} else if (CClass == "PALADIN" || CClass == "WARRIOR") {
			HP = Ch->TStat("Stamina") + (Rand.Get(1, 6) * (4 - GameSkill));
			if (GameSkill == 1) die = 8;
			die = 6;
		}
		for (byte i = 2; i <= Level && i <= Ch->TStat("Stamina"); ++i) HP += Rand.Get(1, die);
		Ch->BStat["HP"] = HP;
		auto P{ Ch->GetPoints("HP") };
		P->MaxCopy = "HP";
		P->Have(HP);
	}
#pragma endregion
}