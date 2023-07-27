// Lic:
// The Legend of the Kxsarl
// Global Graphics (header)
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
// Version: 23.07.27
// EndLic
#pragma once

#include <TQSG.hpp>

namespace Kxsarl {

	extern bool FullScreenMode;

	inline void ColLoop(int divider=100){ Slyvina::TQSG::SetColorHSV((SDL_GetTicks() / divider) % 360, 1, 1); }

	Slyvina::TQSG::_____TIMAGEFONT* ChiqueFont();
	Slyvina::TQSG::_____TIMAGEFONT* Ryanna();
	Slyvina::TQSG::_____TIMAGEFONT* MiniFont();

	Slyvina::TQSG::_____TIMAGE* CheckBox(Slyvina::byte Checked = 0);
	Slyvina::TQSG::_____TIMAGE* ImgPickButton();

	Slyvina::TQSG::_____TIMAGE* ImgBack();

	inline void Ryanna(std::string t, int x, int y, Slyvina::TQSG::Align ax = Slyvina::TQSG::Align::Left, Slyvina::TQSG::Align ay = Slyvina::TQSG::Align::Top) { Ryanna()->Text(t, x, y, ax, ay); }
	inline void MiniFont(std::string t, int x, int y, Slyvina::TQSG::Align ax = Slyvina::TQSG::Align::Left, Slyvina::TQSG::Align ay = Slyvina::TQSG::Align::Top) { MiniFont()->Text(t, x, y, ax, ay); }
	inline void Ryanna(int t, int x, int y, Slyvina::TQSG::Align ax = Slyvina::TQSG::Align::Left, Slyvina::TQSG::Align ay = Slyvina::TQSG::Align::Top) { Ryanna()->Text(std::to_string(t), x, y, ax, ay); }

	void DrawBackground();

#pragma region Arrows
	enum class EArrow { Left, Right, Up, Down, UpLeft, UpRight, DownLeft, DownRight };
	Slyvina::TQSG::_____TIMAGE* Arrow(EArrow A);
#pragma endregion

}