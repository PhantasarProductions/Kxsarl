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
// Version: 23.07.19
// EndLic

#undef KXSARL_NOCATCH

#include <SlyvQCol.hpp>

#include <TQSG.hpp>

#include "../Headers/Error.hpp"
#include "../Headers/UseJCR6.hpp"
#include "../Headers/Version.hpp"
#include "../Headers/Config.hpp"
#include "../Headers/Flow.hpp"

using namespace Slyvina;
using namespace Units;
using namespace TQSG;
using namespace Kxsarl;

void EnterGFXMode() {
	QCol->Doing("Entering", "Graphics Mode");
	if (CFG_Windowed()) {
		Graphics(CFG_Width(), CFG_Height(), "The Legend of the Kxsarl");
	} else {
		Graphics("The Legend of the Kxsarl");
	}
	SetAltScreen(1900, 1000);
}

int main(int ac, char** a) {
#ifndef KXSARL_NOCATCH
	try {
#endif
		Executable = ChReplace(a[0], '\\', '/');
		QCol->LCyan("The Legend of the Kxsarl\n");
		QCol->Doing("Version", Version());
		QCol->Doing("Coded by", "Jeroen P. Broks");
		QCol->LMagenta("(c) " + CYear() + " Jeroen P. Broks; ");
		QCol->LGreen("Licensed under the terms of the GPL3\n\n");
		QCol->Doing("Launched file", Executable);
		QCol->Doing("Game dir", ExeDir());
		QCol->Doing("Desktop Size", TrSPrintF("%d x %d", DesktopWidth(), DesktopHeight()));
		QCol->Doing("Game screen", CFG_ScreenModeString());

		QCol->LMagenta(MRes()->GetString("Text/Welcome.txt") + " \n");
		FlowInt();
		EnterGFXMode();
		Run();

		// Actual code comes later!
		QCol->Reset(); std::cout << "\n\n\n";
		return 0;
#ifndef KXSARL_NOCATCH
	} catch (std::exception e) {
		Crash(TrSPrintF("Exception thrown: %s", e.what()));
		return 255; // Backup. Should not be required as "Crash" should already end all operations.
	}
#endif
}