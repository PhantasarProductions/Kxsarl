// Lic:
// The Legend of the Kxsarl
// Game Config (header)
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
// Version: 23.07.30
// EndLic

#pragma once
#include <Slyvina.hpp>
#include <SlyvGINIE.hpp>

#include <Statistician.hpp>

namespace Kxsarl {
	namespace Game {
		extern Slyvina::byte GameSkill; 
		extern Slyvina::Units::UGINIE GameBase;
		extern Slyvina::TQSG::TImage FaceHero;
		extern std::string CharID;
		extern std::string GameID;
		extern Slyvina::Statistician::UParty GameParty;

		void LoadGameBase(std::string CHID);
		Slyvina::int64 ExpNeeded(Slyvina::byte Level,std::string ChClass);
		
	}
}