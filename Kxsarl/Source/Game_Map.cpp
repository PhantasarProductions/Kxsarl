// Lic:
// The Legend of the Kxsarl
// Game Map Class
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
using namespace SuperTed;
using namespace Statistician;

namespace Kxsarl {  
	namespace Game {
		GMap Map{};

			

		void GMap::GetForward(__PlayerPos&P,int tiles, int& x, int& y) {
			switch (Player.Wind) {
			case GWind::North: x = P.X; y = P.Y - tiles; return;
			case GWind::East: x = P.X + tiles; y = P.Y; return;
			case GWind::South: x = P.X; y = P.Y + tiles; return;
			case GWind::West: x = P.X - tiles; y = P.Y; return;
			}
			Crash("GetForward(): Wind error");
		}

		void GMap::GoForward(int tiles) {
			GetForward(tiles, Player.X, Player.Y);
		}

		void GMap::GetSideward(int tiles, int& x, int& y) {
			switch (Player.Wind) {
			case GWind::North: x = Player.X + tiles; y = Player.Y; return;
			case GWind::East: x = Player.X; y = Player.Y + tiles; return;
			case GWind::South: x = Player.X - tiles; y = Player.Y; return;
			case GWind::West: x = Player.X; y = Player.Y - tiles; return;
			}
			Crash("GetSide(): Wind error");
		}

		void GMap::GoSideward(int tiles) {
			GetSideward(tiles, Player.X, Player.Y);
		}

		void GMap::TurnLeft() {
			int W{ (int)Player.Wind };
			Player.Wind = (GWind)((W - 1) % 4);
		}

		void GMap::TurnRight() {
			int W{ (int)Player.Wind };
			Player.Wind = (GWind)((W + 1) % 4);
		}

		__PlayerPos& GMap::Spot(std::string _Room, std::string _Spot) {
			Trans2Upper(_Room);
			Trans2Upper(_Spot);
			return ExitSpots[_Room][_Spot];
		}

		void GMap::ScanExitSpots() {
			ExitSpots.clear();
			if (!Map) Crash("Can't search for exit spots when no map has been loaded");
			for (auto& kvr : Map->Rooms) {
				auto _Room{ Upper(kvr.first) };
				for (int _y = 0; _y < kvr.second->H(); _y++) {
					for (int _x = 0; _x < kvr.second->W(); _x++) {
						auto OL{ kvr.second->MapObjects->Value(_x, _y) };
						if (OL) {
							for (auto O : *OL) {
								if (Upper(O->Data["__kind"]) == "EXIT") {
									auto &ESR{ ExitSpots[Upper(kvr.first)] };
									auto Tag{ Upper(O->Data["TAG"]) };
									if (ESR.count(Tag)) {
										Crash("Dupe exit (" + O->Data["TAG"] + ") in room " + kvr.first);
										return;
									}
									ESR[Tag] = { _x,_y,CWind(O->Data["WIND"]) };
								}
							}
						}
					}
				}
			}
		}

		void GMap::Load(std::string mapdir) {
			QCol->Doing("Loading map", mapdir);
			Map = LoadTeddy(MRes(), "Game/" + GameID + "/Maps/" + mapdir);
			ScanExitSpots();
		}

		void GMap::GoTo(std::string _Map, std::string _Room, int x, int y, GWind wind) {
			Load(_Map);
			CurrentRoom = _Room;
			Player.X = x;
			Player.Y = y;
			Player.Wind = wind;
		}


		void GMap::GoTo(std::string _Map, std::string _Room, std::string spot) {
			Crash("GoTo based on spot not yet coded");
		}

		GMap::GMap() {
			Actors = TList<GActor>::CreateUnique();
		}

		Statistician::UParty GActor::_Party{CreateUniqueParty()};
		Statistician::_Party* GActor::Party() { return _Party.get(); }
		
		GWind CWind(std::string _Wind) {
			Trans2Upper(_Wind);
			if (_Wind.size()) switch (_Wind[0]) {
			case 'N': return GWind::North;
			case 'E': return GWind::East;
			case 'S': return GWind::South;
			case 'W': return GWind::West;
			}
			Crash("Unknown wind string (" + _Wind + ")");
			return GWind::North; // safety precaution
		}
		std::string CWind(GWind _Wind) {
			switch (_Wind) {
			case GWind::North: return "North";
			case GWind::South: return "South";
			case GWind::East: return "East";
			case GWind::West: return "West";
			}
		}
	}
}