// Lic:
// The Legend of the Kxsarl
// Global Graphics
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

#include <algorithm>

#include <SlyvQCol.hpp>

#include "../Headers/Error.hpp"
#include "../Headers/UseJCR6.hpp"
#include "../Headers/GlobGFX.hpp"

using namespace Slyvina;
using namespace TQSG;
using namespace Units;

#define KXSARL_GetFont(VARIABLE,FONTFILE)\
	if (!VARIABLE) {\
		QCol->Doing("Linking font", FONTFILE);\
		VARIABLE = LoadUImageFont(MRes(), FONTFILE);\
	}\
	return VARIABLE.get();

#define KXSARL_GetPic(FILE) static auto ret{LoadUImage(MRes(),FILE)}; return ret.get()

namespace Kxsarl {

	const char* _FileChiqueFont{ "Fonts/MorrisRoman.jfbf" };
	const char* _FileRyannaFont{ "Fonts/Ryanna.jfbf" };

	TUImageFont _ChiqueFont{ nullptr };
	TUImageFont _RyannaFont{ nullptr };
	TUImageFont _MiniFont{ nullptr };

	_____TIMAGEFONT* ChiqueFont() { KXSARL_GetFont(_ChiqueFont, _FileChiqueFont); }
	_____TIMAGEFONT* Ryanna() { KXSARL_GetFont(_RyannaFont, _FileRyannaFont); }
	_____TIMAGEFONT* MiniFont() { KXSARL_GetFont(_MiniFont, "Fonts/Mini.jfbf"); }

	

	

	_____TIMAGE* CheckBox(byte Checked) {
		static TUImage _Checkbox[2];
		Checked = std::min((byte)1, Checked);
		if (!_Checkbox[Checked]) {
			QCol->Doing("Loading", TrSPrintF("Checkbox #%d", Checked));
			_Checkbox[Checked] = LoadUImage(MRes(), TrSPrintF("GFX/BasicUI/Check%d.png", Checked));
		}
		return _Checkbox[Checked].get();
	}

	Slyvina::TQSG::_____TIMAGE* ImgPickButton() { KXSARL_GetPic("GFX/BasicUI/Pick.png"); }
	Slyvina::TQSG::_____TIMAGE* ImgBack() { KXSARL_GetPic("GFX/Alg/Background.jpg"); }

	void DrawBackground() { ImgBack()->Tile(0, 0, ScreenWidth(), ScreenHeight()); }

	Slyvina::TQSG::_____TIMAGE* Arrow(EArrow A) {
		static std::map<EArrow, TUImage>_Arrow{};
		std::string n{""};
		if (!_Arrow[A]) {
			switch (A) {
			case EArrow::Left: n = "Left"; break;
			case EArrow::Right: n = "Right"; break;
			case EArrow::Up: n = "Up"; break;
			case EArrow::Down: n = "Down"; break;
			case EArrow::UpLeft: n = "UpLeft"; break;
			case EArrow::UpRight: n = "UpRight"; break;
			case EArrow::DownLeft: n = "DownLeft"; break;
			case EArrow::DownRight: n = "DownRight"; break;
			default: Crash("Unknown Arrow Code"); break;
			}
			QCol->Doing("Loading", n + " arrow");
			_Arrow[A] = LoadUImage(MRes(), "GFX/Arrows/" + n + ".png");
		}
		return _Arrow[A].get();
	}

}