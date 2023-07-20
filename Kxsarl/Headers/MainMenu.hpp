// Lic:
// The Legend Of The Kxsarl
// Main Menu (header)
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
// Version: 23.07.18
// EndLic


#ifdef KXSARL_DEBUG
#include <iostream>
#endif

namespace Kxsarl {

	class MainMenuItem;

	class MainMenuItem {
	private:
		MainMenuItem(std::string _Caption, void (*_Action)(MainMenuItem*) = nullptr, bool (*_Allow)(MainMenuItem*) = nullptr);
	public:
		std::string Caption{ "" };
		void (*Action)(MainMenuItem*) { nullptr };
		bool (*Allow)(MainMenuItem*) { nullptr };

		static void InitMenuItems();
#ifdef KXSARL_DEBUG
		inline ~MainMenuItem() { std::cout << "Destroyed main menu item: " << Caption << "\n"; }
#endif
	};

	void Arrive_MainMenu();
	bool Flow_MainMenu();
}