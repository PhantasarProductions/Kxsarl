// Lic:
// The Legend Of The Kxsarl
// Version information (and quick inline functions)
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
// Version: 23.07.17
// EndLic
#pragma once

#include <SlyvString.hpp>
#include <SlyvTime.hpp>

#include "Debug.hpp"

namespace Kxsarl {
	const int
		V1{ 1 },
		V2{ 0 },
		V3{ 0 };

	inline std::string Version() { 
#ifndef KXSARL_DEBUG
		return Slyvina::Units::TrSPrintF("%d.%02d.%04d",V1,V2,V3); 
#else
		return Slyvina::Units::TrSPrintF("%d.%02d.%04d - DEBUG",V1,V2,V3);
#endif
	}

	inline std::string CYear() {
		using namespace Slyvina::Units;
		int Y{ CurrentYear() };
		if (Y == 2023) return "2023";
		return TrSPrintF("2023-%04d", Y);
	}
}