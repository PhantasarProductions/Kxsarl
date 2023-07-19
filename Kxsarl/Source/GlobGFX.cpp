
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


