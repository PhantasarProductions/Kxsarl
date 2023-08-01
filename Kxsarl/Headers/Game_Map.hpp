// Lic:
// The Legend of the Kxsarl
// Game Map Class (header)
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
// Version: 23.08.01
// EndLic

#pragma once
#include <string>
#include <SuperTed_Core.hpp>
#include <SlyvLinkedList.hpp>

namespace Kxsarl {
	namespace Game {
		enum class GWind { North, East, South, West };
		struct __PlayerPos { int X{ 0 }, Y{ 0 }; GWind Wind{ GWind::North }; };

		class GMap;
		class GActor;

		class GMap {
		private:
		public:
			// Fields
			Slyvina::SuperTed::Teddy Map{nullptr};
			std::unique_ptr<Slyvina::Units::TList<GActor>> Actors{nullptr };
			__PlayerPos Player{ 0,0,GWind::North };
			std::string LoadedMap{""};
			std::string CurrentRoom{""};			

			// Positon functions
			void GetForward(int tiles, int& x, int& y);
			void GoForward(int tiles = 1);
			void GetSideward(int tiles, int& x, int& y);
			void GoSideward(int tiles = 1);
			void TurnLeft();
			void TurnRight();

			// Load
			void Load(std::string);
			void GoTo(std::string _Map, std::string _Room, int x, int y, GWind wind = GWind::North);
			void GoTo(std::string _Map, std::string _Room, std::string spot = "Start");
			GMap();
		};

		class GActor {
		private:
			GMap* _parent;
		public:
			int X{ 0 }, Y{ 0 };
		};

		extern GMap Map;
	}
}