// Lic:
// The Legend Of the Kxsarl
// Narration
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
// Version: 23.07.30
// EndLic

// Tag for debugging purposes. 
// Having to watch this every time something goes wrong in something following up, is gonna get frustrating VERY SOON!

#undef SKIP_NARRATION

#include "AllHeaders.hpp"

namespace Kxsarl {  
	namespace Game {
		void StartNarration(std::string NarrationEvent, bool(*ReturnFlow)(), std::string ReturnEventParameters) {
			auto NarrationFile{ TrSPrintF("Game/%s/Narration/%s/%s",GameID.c_str(),NarrationEvent.c_str(),CFG_Lang().c_str()) };
			if (!MRes()->EntryExists(NarrationFile)) { Crash("Narration text not found!", { { "Entry",NarrationFile } }); return; }
			auto Lines{ MRes()->GetLines(NarrationFile) };
			QCol->Doing("Narration", NarrationEvent);
			QCol->Doing("=> Lines", (int)Lines->size());
#ifndef SKIP_NARRATION

#endif // !SKIP_NARRATION
			if (ReturnFlow) GoFlow(ReturnFlow);
		}
	} 
}