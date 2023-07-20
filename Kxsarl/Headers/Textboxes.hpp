// Lic:
// The Legend Of The Ksxarl
// Textboxes (header)
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

#include <TQSG.hpp>

#include <SlyvLinkedList.hpp>

namespace Kxsarl {
	class _TextBoxGroup; typedef std::unique_ptr<_TextBoxGroup> TextBoxGroup;
	class TextBox;

	typedef void (*TextBoxAction)(TextBox*);

	class _TextBoxGroup {
	private:
		std::unique_ptr<Slyvina::Units::TList<TextBox>> Boxes{ Slyvina::Units::TList<TextBox>::CreateUnique() };
	public:
		Slyvina::Units::TLink<TextBox>* CurrentLink{ nullptr };
		TextBox* Current();
		TextBox* NewBox(Slyvina::uint32 x, Slyvina::uint32 y, std::string defaultvalue = "", bool numbersonly = false);
		TextBox* NewBox(Slyvina::uint32 x, Slyvina::uint32 y, int defaultvalue);
		void Next();
		void Draw(bool noaction = false, bool forceenabled = false);
		void SetEnabled(bool value);
		inline void Enable() { SetEnabled(true); }
		inline void Disable() { SetEnabled(false); }

	};

	class TextBox {
	private:
		_TextBoxGroup* parent{ nullptr };
	public:
		Slyvina::uint32
			x{ 0 },
			y{ 0 },
			h{ 0 },
			w{ 0 };
		std::string
			value{""};
		bool
			numbersonly{ false },
			enabled{ true };
		TextBoxAction
			Action{ nullptr };
		Slyvina::byte
			r{ 255 },
			g{ 255 },
			b{ 255 },
			a{ 255 };
		Slyvina::TQSG::_____TIMAGEFONT* Font{nullptr};

		TextBox(_TextBoxGroup* ouwe, Slyvina::uint32 _x, Slyvina::uint32 _y, std::string defaultvalue = "", bool numbersonly = false);

		bool Current();
		void Draw(bool noaction = false, bool forceenabled = false);
		void Color(Slyvina::byte r, Slyvina::byte g, Slyvina::byte b);
		void IntValue(int _value);
		inline void IntValue(std::string value) { IntValue(ToInt(value)); }
		int IntValue() { return ToInt(value); }

	};
		
	inline TextBoxGroup CreateTextBoxGroup() { return std::unique_ptr<_TextBoxGroup>(new _TextBoxGroup()); }


}