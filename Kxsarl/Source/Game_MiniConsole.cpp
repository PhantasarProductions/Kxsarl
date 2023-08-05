// Lic:
// The Legend of the Kxsarl
// Mini Console
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
// Version: 23.08.05
// EndLic

#include <TQSG.hpp>
#include <TQSE.hpp>
#include <TQSA.hpp>

#include "AllHeaders.hpp"


using namespace std;
using namespace Slyvina;
using namespace Units;
using namespace JCR6;
using namespace TQSG;
using namespace TQSE;
using namespace TQSA;

namespace Kxsarl {  
	namespace Game {
		const int MaxLines{ 50 };
		struct MCText {
			byte r, g, b;
			std::string msg;
			int X{ 0 };
		};
		
		static int XPos{ 0 }, YPos{ 0 };
		static shared_ptr<TList<MCText>> Lines[MaxLines]{};
		
		inline void InitL() {
			static bool _DoneInit{ false };
			if (_DoneInit) return;
			QCol->Doing("Initiating", "Mini Console");
			for (uint32 i = 0; i < MaxLines; ++i) Lines[i] = TList<MCText>::CreateShared();
			MCWriteLn("The Legend Of The Kxsarl", 255, 180, 0);
			MCWrite("Coded and copyrighted by: ", 180, 255, 0);
			MCWriteLn("Jeroen P. Broks", 180, 0, 255);
			_DoneInit = false;
		}

		void MCWrite(std::string msg, byte r, byte g, byte b) {
			InitL();
			*Lines[YPos] += new MCText{ r, g, b, msg, XPos };
			XPos += MiniFont()->Width(msg);
		}

		void MCNext() {
			InitL();
			XPos = 0;
			if (++YPos >= MaxLines) {
				YPos = MaxLines;
				for (int i = 0; i < MaxLines-1; i++) Lines[i] = Lines[i + 1];
				Lines[MaxLines - 1] = TList<MCText>::CreateShared();
			}
		}

		void MCShow(uint32 linenr, uint32 Y) {
			InitL();
			for (auto L = Lines[linenr]->First(); L; L = Lines[linenr]->Next()) {
				MiniFont(L->msg, L->X, Y);
			}
		}

	}
}