// Lic:
// The Legend of The Kxsarl
// Error Handler
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

#include <TQSE.hpp>

#include <SlyvQCol.hpp>

#include "../Headers/Error.hpp"


namespace Kxsarl {
	void Crash(std::string s) {
		Crash(s, {});
	}

	void Crash(std::string e, std::map<std::string, std::string> extrainfo) {
		QCol->Red("\7*** YOU HAVE BEEN STRUCK BY THE FORCES OF EVIL ***\n");
		QCol->Error(e);
		std::string error{e};
		for (auto& ei : extrainfo) {
			error += "\n" + ei.first + ": " + ei.second;
			QCol->Doing(ei.first, ei.second);
		}
		Slyvina::TQSE::Notify("**** YOU HAVE BEEN STRUCK BY THE FORCES OF EVIL ****\n\n" + error);
		QCol->Reset();
		exit(e.size());
	}
}