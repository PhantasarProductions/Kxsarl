// Lic:
// The Legend of the Ksxarl
// Mini Console (header)
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
// Version: 23.08.05
// EndLic

#pragma once
#include <string>

namespace Kxsarl {
	namespace Game {
		void MCWrite(std::string msg, Slyvina::byte r, Slyvina::byte g, Slyvina::byte b);
		inline void MCWrite(std::string msg) { MCWrite(msg, 255, 255, 255); }
		void MCNext();
		inline void MCWriteLn(std::string msg) { MCWrite(msg); MCNext(); }
		inline void MCWriteLn(std::string msg, Slyvina::byte r, Slyvina::byte g, Slyvina::byte b) { MCWrite(msg, r, g, b); MCNext(); }

		void MCShow(Slyvina::uint32 linenr, Slyvina::uint32 Y);

		void MCMiniShow();
	}
}