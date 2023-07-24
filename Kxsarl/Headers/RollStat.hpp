// Lic:
// The Legend Of The Kxsarl
// Stat Roll
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
#pragma once
#include <Slyvina.hpp>
#include <SlyvRandom.hpp>
#include <SlyvString.hpp>

#include "Error.hpp"


namespace Kxsarl {
	inline int RollStat(Slyvina::byte skill,int mini=0) {
		using namespace Slyvina;
		using namespace Units;		
		byte dice[4]{ 0,0,0,0 };
		switch (skill) {
		case 1: {
			byte lowv{ 7 }, lowd{ 8 };
			for (byte i = 0; i < 4; i++) {
				dice[i] = Rand.Get(2, 6);
				if (dice[i] < lowv) {
					lowv = dice[i];
					lowd = i;
				}
			}
			int ret{ 0 };
			for (byte i = 0; i < 4; i++) if (i != lowd) ret += dice[i];
			return std::max(mini,ret);
		} 
		case 2: {
			int ret{ 0 };
			for (byte i = 0; i < 3; i++) ret += Rand.Get(2, 6);
			return std::max(mini,ret);
		}
		case 3: {
			int ret{ 0 };
			for (byte i = 0; i < 3; i++) {
				int roll{ 0 };
				do roll = Rand.Get(1, 6); while (roll >= 6 && Rand.Get(1, 6) != 6);
				ret += roll;
			}
			return ret;
		}
		default:
			Crash(TrSPrintF("RollStat(%d): Unkown skill", skill));
		}
	}
}