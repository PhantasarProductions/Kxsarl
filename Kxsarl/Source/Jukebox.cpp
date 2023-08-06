// Lic:
// The Legend Of The Kxsarl
// Jukebox
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
// Version: 23.08.06
// EndLic

#include <TQSG.hpp>
#include <TQSE.hpp>
#include <TQSA.hpp>

using namespace std;
using namespace Slyvina;
using namespace Units;
using namespace JCR6;
using namespace TQSG;
using namespace TQSE;
using namespace TQSA;

#include "AllHeaders.hpp"


namespace Kxsarl {
	struct JukeEntry{
		std::string
			Title{},
			Artist{},
			Notes{},
			Site{},
			File{};
	};
	class _Juke{
	private:
		std::map<std::string, JukeEntry> _Entries;
		GINIE Dat{ nullptr };
		void LoadG() {
			if (!Dat) {
				QCol->Doing("Loading", "Jukebox data");
				Dat = ParseGINIE(MRes()->GetString("Data/Jukebox.ini"));
			}
		}
	public:
		JukeEntry& operator[](std::string Entry) {
			auto UE{ Upper(Entry) };
			LoadG();
			if (!_Entries.count(UE)) {
				auto Tag{ "Entry:" + Entry };
				QCol->Doing("Creating Jukebox Entry", Entry);
				_Entries[UE] = { Dat->Value(Tag,"Title"),Dat->Value(Tag,"Artist"),Dat->Value(Tag,"License"),Dat->Value(Tag,"WebSite"),Entry };
			}
			return _Entries[UE];
		}
		vector<string>* Entries() { LoadG(); return Dat->List("Index", "Index"); }
	};
	_Juke Juke{};

	void Arrive_Jukebox() {
		Music("Music/Sys/Silence.ogg");
		Ryanna("The quick brown fox jumps over the lazy dog", 0, 0); 
	}

	bool Flow_Jukebox() {
		static auto Back{ LoadUImage(MRes(),"GFX/Jukebox/Background.png") };
		static auto ScrollY{ 0 };
		static auto H{ Ryanna()->Height("The quick brown fox jumps over the lazy dog") };
		static auto Entries{Juke.Entries()};
		static JukeEntry* CurrentEntry{nullptr};
		auto Y{ -ScrollY };
		SetColor(155, 155, 155);
		Back->StretchDraw(0, 0, ScreenWidth(), ScreenHeight());
		for (auto& Ent : *Entries) {
			auto DY{ ((Y++) * H) + 10 };
			auto DX{ 10 };
			if (DY >= 0 && DY <= ScreenHeight() - H) {
				if (MouseY() > ASY(DY) && MouseY() < ASY(DY + H)) {
					ColLoop();
					if (MouseHit(1)) {
						CurrentEntry = &Juke[Ent];
						Music(Ent);
					}
				} else if (CurrentEntry && CurrentEntry == &Juke[Ent]) 
					SetColor(255, 180, 0);
				else SetColor(255, 255, 255);
				Ryanna(Juke[Ent].Title, DX, DY);
			}			
			if (CurrentEntry) {
				SetColor(255, 255, 255);
				static auto EX{ ScreenWidth() - 10 };
				MiniFont("Playing track:", EX, 10, Align::Right);
				Ryanna(CurrentEntry->Title, EX, 30, Align::Right);
				MiniFont("Composed by", EX, 60, Align::Right);
				Ryanna(CurrentEntry->Artist, EX, 80, Align::Right);
				MiniFont("License:", EX, 110, Align::Right);
				if (Ryanna()->Width(CurrentEntry->Notes) > ScreenWidth() / 2)
					MiniFont(CurrentEntry->Notes, EX, 130, Align::Right);
				else
					Ryanna(CurrentEntry->Notes, EX, 130, Align::Right);
				if (CurrentEntry->Site.size()) {
					MiniFont("Website:", EX, 160, Align::Right);
					Ryanna(CurrentEntry->Site, EX, 180, Align::Right);
				}
			}
		}
		return true;
	}
}