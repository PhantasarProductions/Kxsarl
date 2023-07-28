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
// Version: 23.07.28
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
			return "???";
		}

		inline CSlot(string _CHID, string _F) {
			SlotFile = _F;
			if (_F == "*NEW") {
				CSay("Slot for new game");
				NewGame = true;
				uint64 eid{ 0 };
				do SlotFile = TrSPrintF("SLOT_%08X.jcr", eid++); while (FileExists(FullSlotFile()));
				InfoLine = "Lv 1 - Game Start";
			}
			bl += 25;
			CharID = _CHID;
			(*List) += this;
			CSay(to_string(NewGame)+">Slot created: " + _F);
		}
	};
	uint32 CSlot::bl{0};
	uint32 CSlot::nexty{0};
	std::unique_ptr<TList<CSlot>> CSlot::List{nullptr};


	static TUImage Banner{nullptr};
	static bool FlowPickSlot() {
		if (!Banner) {
			QCol->Doing("Loading", "Pick adventure banner");
			Banner = LoadUImage(MRes(), "GFX/Banner/Pick a Slot.png");
			Banner->HotCenter();
		}
		SetColor(255, 255, 255);
		DrawBackground();
		Banner->Draw(ScreenWidth() / 2, 50);
		int64 Y{ -ScrollY };
		for (auto sl = CSlot::List->First(); sl; sl = CSlot::List->Next()) {
			if (Y >= 0 && Y<18) {
				int64 IY{ 120 - (Y * 50) };
				Ryanna(sl->GameName(), 20, IY);
				MiniFont(sl->InfoLine, 20, IY + 30);
				if (MouseY() > ASY(IY) && MouseY() < ASY(IY + 50)) {
					ColLoop(250); SetAlpha(95);
					ARect(10, IY, ScreenWidth() - 20, 50);
					SetAlpha(255);
					if (MouseHit(1)) {
						CSay("Starting game not yet coded");
					}
				}

			}
		}
		SetColor(255, 255, 255, 255);
		static auto GEX{ ScreenWidth() - 70 };
		Game::FaceHero->StretchDraw(GEX, 50, 50, 50);
		//CSay("???> " + Game::GameBase->Value("Gen", "Name")); // DEBUG ONLY!
		Ryanna(Game::GameBase->Value("Gen", "Name"), GEX-10, 50, Align::Right);
		MiniFont(Game::GameBase->Value("Gen", "Sex") + " " + Game::GameBase->Value("Gen", "Class"), GEX-1, 80, Align::Right);
		SetColor(255, 255, 255);
		if (MouseX() < ASX(182) && MouseY() > ASY(ScreenHeight() - 100)) {
			SetColorHSV((SDL_GetTicks() / 100) % 360, 1, 1);
			if (MouseHit(1)) GoFlow("ChooseChar");
		}
		Arrow(EArrow::Left)->StretchDraw(0, ScreenHeight() - 100, 182, 100);
		return true;
	}
	

#pragma region "Come to me"
	void GoPickSlot(string CHID) {
		auto Dir{ SaveCharDir() + "/" + CHID };
		auto List{ FileList(Dir) };
		CSay("Analysing: " + Dir);
		CSlot::List = TList<CSlot>::CreateUnique();
		CSlot::nexty = 0; CSlot::bl = 0;
#ifdef KXSARL_DEBUG
		printf("Going to add %d slots!\n", List->size());
#endif
		for (auto F : *List) {
			if (Prefixed(Upper(F),"SLOT_"))	new CSlot(CHID, F);
		}
		new CSlot(CHID,"*NEW");
		Game::LoadGameBase(CHID);
		GoFlow(FlowPickSlot);
	}
#pragma endregion

}