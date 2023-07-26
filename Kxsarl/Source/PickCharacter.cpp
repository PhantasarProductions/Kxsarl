// Lic:
// The Legend of the Kxsarl
// Pick a character to play the game with
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
// Version: 23.07.26
// EndLic

#include <SlyvStream.hpp>
#include <SlyvDir.hpp>
#include <SlyvTime.hpp>
#include <SlyvGINIE.hpp>

#include <JCR6_Core.hpp>

#include "AllHeaders.hpp"

using namespace std;
using namespace Slyvina;
using namespace Units;

namespace Kxsarl {
	static UGINIE CharIndex{nullptr};

	const char* Header{
		"# This is the index file for characers\n"
		"# It's generated in order to make sure\n"
		"# Loading all character data goes faster\n"
		"# In order to make sure it's up to date it may\n"
		"# automatically reindex every now and then\n"
		"# If you need to enforce that process, simply delete this file\n"
		"# The game will take care of the rest automatically\n\n\n"
	};

	std::string IndexFile() {
		return SaveCharDir()+"/CharacterIndex.ini";
	}
	void Char_Indexer(bool force) {
		auto Okay{ force };	
		
		if (FileExists(IndexFile())) {
			CharIndex = LoadUGINIE(IndexFile());
		} else {
			CharIndex = ParseUGINIE("[Creation]\nDate=" + CurrentDate() + "\nTime=" + CurrentTime()+"\n[Force]\nMax=255");
			Okay = true;
		}
		auto
			OldDate{ CharIndex->Value("Creation", "Date") },
			OldTime{ CharIndex->Value("Creation", "Time") };
		auto
			MaxTimes{ CharIndex->IntValue("Force","Max") },
			Times{ CharIndex->IntValue("Force","Times") },
			Year{ CharIndex->IntValue("Force","Year") };
		Okay = Okay || Year != CurrentYear();
		Okay = Okay || Times > MaxTimes;
		auto CDirs{ FileList(SaveCharDir(),DirWant::Directories) };
		for (auto D : *CDirs) {
			if (Prefixed(Upper(D), "CHAR_")) {
				Okay = Okay || (!CharIndex->HasCat("CHAR::" + D));
				Okay = Okay || (!CharIndex->BoolValue("CHAR::" + D, "Index"));
			}
		}
		QCol->Doing("Indexing", "Characters", "\t");
		if (!Okay) {
			QCol->Red("Skipped! Appears to be no need for it!\n");
			CharIndex->Value("Force", "Times", Times+1);
			SaveString(TrSPrintF("%s\n%s",Header, CharIndex->UnParse().c_str()), IndexFile());
			return;
		}
		QCol->Green("Go!\n");
		SysStatus("Indexing");
		CharIndex = ParseUGINIE("[Creation]\nDate=" + OldDate + "\nTime=" + OldTime);
		CharIndex->Value("Force", "Max", MaxTimes);
		CharIndex->Value("Force", "Times", 0);
		CharIndex->Value("Force", "Year", CurrentYear());
		for (auto D : *CDirs) {
			if (Prefixed(Upper(D), "CHAR_")) {
				string DTag{ "CHAR::" + D };
				auto GeneralFile{ SaveCharDir() + "/" + D + "General.jcr" };
				if (FileExists(GeneralFile)) {
					QCol->Doing("Indexing:", D);
					auto GF = JCR6::JCR6_Dir(GeneralFile);
					auto GG = LoadUGINIE(GF->GetString("Base.ini"));
					CharIndex->Value(DTag, "Name", GG->Value("Gen", "Name"));
					CharIndex->Value(DTag, "Class", GG->Value("Gen", "Class"));
					CharIndex->Value(DTag, "Gender", GG->Value("Gen", "Gender"));
					switch (GG->IntValue("SKill", "Skill")) {
					case 1: CharIndex->Value(DTag, "Skill", "Easy"); break;
					case 2: CharIndex->Value(DTag, "Skill", "Normal"); break;
					case 3: CharIndex->Value(DTag, "Skill", "Hard"); break;
					default: CharIndex->Value(DTag, "Skill", "???"); break;
					}
					CharIndex->Add("CharList", "CharList", D);
				} else {
					QCol->Error("No general file in character dir: " + D);
				}
			}
		}
		SaveString(TrSPrintF("%s\n%s", Header, CharIndex->UnParse().c_str()), IndexFile());
	}

}