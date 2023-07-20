
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
		void Draw(bool noaction = false, bool forceenabled = false);
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
	};


}