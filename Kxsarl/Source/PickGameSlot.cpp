// Lic:
// The Legend of the Kxsarl
// Pick Game Slot
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

#include <TQSE.hpp>

#include <SlyvDir.hpp>
#include <SlyvLinkedList.hpp>

#include "AllHeaders.hpp"

using namespace std;
using namespace Slyvina;
using namespace Units;
using namespace TQSG;
using namespace TQSE;

namespace Kxsarl {

	static int64 ScrollY{0};

	class CSlot {
	public:
		static uint32 bl;
		static uint32 nexty;
		static std::unique_ptr<TList<CSlot>> List;
		bool NewGame{false};
		uint32 y{ nexty++ };
		uint32 lw{ bl };
		string SlotFile{ "" };
		string GameID{ "" };
		string CharID{ "" };
		string InfoLine{ "" };
		string SlotDir() { return SaveCharDir() + "/" + CharID; }
		string FullSlotFile() { return SlotDir() + "/" + SlotFile; }
		string GameName() {
			if (NewGame) return "* New Adventure *";
		}

		inline CSlot(string _CHID, string _F) {
			if (_F == "*NEW") {
				NewGame = true;
				uint64 eid{ 0 };
				do SlotFile = TrSPrintF("SLOT_%08X.jcr", eid++); while (FileExists(FullSlotFile()));
				InfoLine = "Lv 1 - Game Start";
			}
			bl += 25;
			SlotFile = _F;
			CharID = _CHID;
			(*List) += this;
			CSay(to_string(NewGame)+">Slot created: " + _F);
		}
	};
	uint32 CSlot::bl{0};
	uint32 CSlot::nexty{0};
	std::unique_ptr<TList<CSlot>> CSlot::List{nullptr};


	static bool FlowPickSlot() {
		static auto Banner{ LoadUImage(MRes(),"GFX/Banner/Pick a Slot.png") }; Banner->HotCenter();
		SetColor(255, 255, 255);
		DrawBackground();
		Banner->Draw(ScreenWidth() / 2, 50);
		int64 Y{ -ScrollY };
		for (auto sl = CSlot::List->First(); sl; sl = CSlot::List->Next()) {
			if (Y > 0) {
				int64 IY{ 120 - (Y * 50) };
				Ryanna(sl->GameName(), 20, IY);
				MiniFont(sl->InfoLine, 20, IY + 30);
			}
		}
		return true;
	}
	

#pragma region "Come to me"
	void GoPickSlot(string CHID) {
		auto Dir{ SaveCharDir() + "/" + CHID };
		auto List{ FileList(Dir) };
		CSlot::List = TList<CSlot>::CreateUnique();
		CSlot::nexty = 0; CSlot::bl = 0;
		new CSlot(CHID,"*NEW");

		GoFlow(FlowPickSlot);
	}
#pragma endregion

}