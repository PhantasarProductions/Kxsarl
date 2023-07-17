// Lic:
// The Legend of the Ksxarl
// Main start up
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
// Version: 23.07.17
// EndLic

#include <SlyvQCol.hpp>

#include "../Headers/Version.hpp"

using namespace Slyvina;
using namespace Units;
using namespace Kxsarl;

int main(int ac, char** a) {
	QCol->LCyan("The Legend of the Kxsarl\n");
	QCol->Doing("Version", Version());
	QCol->Doing("Coded by", "Jeroen P. Broks");
	QCol->LMagenta("(c) " + CYear() + " Jeroen P. Broks; ");
	QCol->LGreen("Licensed under the terms of the GPL3\n");
	// Actual code comes later!
	QCol->Reset();
	return 0;
}