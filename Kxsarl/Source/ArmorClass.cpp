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
// Version: 23.07.24
// EndLic
#include "AllHeaders.hpp"

using namespace std;
using namespace Slyvina;
using namespace Statistician;
using namespace Kxsarl::Game;

#define ACDEBUG

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
		double Base = 5 * GameSkill; Chat("Base: " << Base<< "\t(Skill: "<<GameSkill<<")");
		double Level{ (double)Ch->Statistic("Level")->Total() }; Chat("Level: " << Level);
		double Tough{ (double)Ch->Statistic("Toughness")->Total() }; Chat("Toughness: " << Tough);
		double Will{ (double)Ch->Statistic("Will")->Total() }; Chat("Will: " << Will);
		double Stat{ 0 };
		Stat += ((Tough + Level) / 20); 
		Stat += ((Will + Level) / 18);
		Chat("Raw Stat: " << Stat);
		Stat /= 2;
		Chat("Half Stat: " << Stat);
		Ch->Statistic("AC")->Base = (int)ceil(Base - Stat);
		Chat("AC: " << Ch->Statistic("AC")->Base);
	}

	static std::map<std::string, StatScript> AC_PerClass{
		{"PALADIN", AC_Paladin}
	};
#pragma endregion


	void SetArmorClass(Character Ch, std::string CClass) {
		auto AC{ Ch->Statistic("AC") };
		Trans2Upper(CClass);
		AC->Maxi(10);
		AC->Mini(-10);
		if (!AC_PerClass.count(CClass)) throw std::runtime_error("AC cannot be set up for class" + CClass);
		AC->StatScriptFunction = AC_PerClass[CClass];
	}
}