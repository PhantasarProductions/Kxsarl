// Lic:
// The Legend of the Kxsarl
// Time for true dungeon crawling
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
		_____TIMAGE* CrawlBack() {
			static auto _ret{ LoadUImage(MRes(),"GFX/Crawl/Background.jpg") };
			return _ret.get();
		}
		void DrawDungeon(__PlayerPos* PP, int x, int y, int w, int h) {
			// Base Background
			SetColor(65, 65, 65); Rect(x, y, w, h / 2);
			SetColor(100, 100, 100); Rect(x, y + (h / 2), w, h / 2);
			// Floor and ceiling
			// Actors (most of all enemies) 
			// Walls, illusions and doors
		}
		
		void DrawScreen() {
			CrawlBack()->Tile(0, 0, ScreenWidth(), ScreenHeight());
			DrawDungeon();
		}
		void Arrive_Crawl() {}
		bool Flow_Crawl() {
			DrawScreen();
			return true;
		}
	}
}