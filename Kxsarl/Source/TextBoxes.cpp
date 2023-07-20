// Lic:
// The Legend of the Kxsarl
// Textboxes
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
// Version: 23.07.20
// EndLic

#include <TQSE.hpp>

#include "../Headers/Debug.hpp"
#include "../Headers/Error.hpp"
#include "../Headers/GlobGFX.hpp"
#include "../Headers/Textboxes.hpp"

using namespace std;
using namespace Slyvina;
using namespace TQSG;
using namespace TQSE;

namespace Kxsarl {
	static void __ActionNiks(TextBox*) {};

	TextBox* Kxsarl::_TextBoxGroup::Current() {
		if (!CurrentLink) CurrentLink = Boxes->FirstLink(1);
		if (!CurrentLink) return nullptr;
		return CurrentLink->Obj;
	}

	TextBox* _TextBoxGroup::NewBox(uint32 x, uint32 y, std::string defaultvalue, bool numbersonly) {
		auto ret{ new TextBox(this,x,y,defaultvalue,numbersonly) };
		(*Boxes) += ret;
		return ret;
	}

	TextBox* _TextBoxGroup::NewBox(uint32 x, uint32 y, int defaultvalue) {
		return NewBox(x, y, to_string(defaultvalue), true);
	}

	void _TextBoxGroup::Next() {
		CSay("Next");
		CurrentLink = CurrentLink->Next;
	}
	
	void _TextBoxGroup::Draw(bool noaction, bool forceenabled) {
		for (auto b = Boxes->First(0); b; b = Boxes->Next(0)) 
			b->Draw(noaction, forceenabled);		
	}

	void _TextBoxGroup::SetEnabled(bool value) {
		for (auto b = Boxes->First(0); b; b = Boxes->Next(0))
			b->enabled = value;
	}

	void TextBox::IntValue(int _value) { value = to_string(_value); }

	TextBox::TextBox(_TextBoxGroup* ouwe,uint32 _x, uint32 _y, std::string defaultvalue, bool _numbersonly) {
		if (!ouwe) Crash("Parentless textbox creation request");
		parent = ouwe;
		x = _x;
		y = _y;
		value = defaultvalue;
		numbersonly = _numbersonly;
		Action = __ActionNiks;
		Font = Ryanna();
	}

	bool TextBox::Current() {
		return parent->Current() == this;
	}

	void TextBox::Draw(bool noaction, bool forceenabled) {
		static const char* Cursor{ " _" };
		bool ienabled = enabled || forceenabled;
		if (!w) w = ScreenWidth() - x - 20;
		h = max(h, (uint32)Font->Height(value) + 4);
		SetColor(r / 10, g / 10, b / 10);
		ARect(x, y, w, h);
		SetColor(r, g, b);
		if (!ienabled) SetColor(r / 3, g / 3, b / 3);
		if (Current() && ienabled) {
			ARect(x, y, w, h, true);
			Font->Text(value + Cursor[(SDL_GetTicks() / 500) % 2], x + 2, y + 2);
			auto ch = GetChar();
			if (ch) {
				switch (ch) {
				case 8: if (value.size()) value = Left(value, value.size() - 1); break;
				case 9:
					parent->Next();
					Flush();
				case 13:
					break;
				default:
					if (Font->Width(TrSPrintF("%s%c_", value.c_str(), ch)) < w) value += ch;
				}
				if (numbersonly) IntValue(value);
				if (!noaction) Action(this);
			}
		} else {
			Font->Text(value, x + 2, y + 2);
		}
	}


	void TextBox::Color(byte r, byte g, byte b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}
}