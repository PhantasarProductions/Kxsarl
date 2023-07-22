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
#include "AllHeaders.hpp"
#include <map>

using namespace std;

namespace Kxsarl {

#pragma region "Stage Initiation"
	enum class Stage { Skill, ClassSex, StatRolls, Name };
	void S_Skill();
	void S_ClassSex();
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
		DrawBackground();
		if (!StageMap.count(CurrentStage)) Crash("Internal error - Please report!", { {"Error",TrSPrintF("Unknown stage identifier (%d)",(int)CurrentStage)} });
		StageMap[CurrentStage]();
		return true;
	}

	void Arrive_CharCreation() {
		CurrentStage = Stage::Skill;
	}
#pragma endregion
}