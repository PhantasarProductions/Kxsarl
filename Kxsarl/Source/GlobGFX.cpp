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
// Version: 23.07.19
// EndLic

#include <SlyvQCol.hpp>

#include "../Headers/UseJCR6.hpp"
#include "../Headers/GlobGFX.hpp"

using namespace Slyvina;
using namespace TQSG;
using namespace Units;

namespace Kxsarl {

	const char* _FileChiqueFont{ "Fonts/MorrisRoman.jfbf" };

	TUImageFont _ChiqueFont{ nullptr };

	_____TIMAGEFONT* ChiqueFont() {
		if (!_ChiqueFont) {
			QCol->Doing("Linking font", _FileChiqueFont);
			_ChiqueFont = LoadUImageFont(MRes(),_FileChiqueFont);
		}
		return _ChiqueFont.get();
	}
}