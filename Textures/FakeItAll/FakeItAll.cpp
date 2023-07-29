// Lic:
// Fake It All
// .
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
// Version: 23.07.29
// EndLic

// Please note that FakeItAll (and the Blitz Basic program Fake 3D)
// Are NOT part of the game distribution, but merely some tools meant for
// development purposes during the projects design stages only. 

// I added it to this repository for examination purposes, but that's all.
// You can also expect a kind of "dirty" way to access directories and stuff
// as a result.

#include <SlyvGINIE.hpp>
#include <SlyvDir.hpp>
#include <SlyvQCol.hpp>
#include <SlyvStream.hpp>
#include <SlyvTime.hpp>
#include <SlyvAsk.hpp>

#define ConfigFile ExtractDir(a[0]) + "/FakeItAll.ini"
#define ModifyFile ExtractDir(a[0]) + "/FakeItAll.ModifyLog.ini"
#define TexDir "/Projects/Applications/Slyvina/Apps/Kxsarl/Textures/"
#define AssetJQLiniFile "/Projects/Applications/Slyvina/Apps/Kxsarl/Assets/Dir2JQL.ini"

using namespace std;
using namespace Slyvina;
using namespace Units;

GINIE AssetConfig{ nullptr };
GINIE Config{ nullptr };
GINIE ModifyLog{ nullptr };
vector<string> Modified{};
inline string TD(string k) { return string(TexDir) + k; }

int main(int ac, char** a) {
	bool force{ false };
	for (int i = 1; i < ac; i++) force = force || Upper(a[i]) == "-FORCE";
	QCol->Green("Fake it all\n");
	QCol->Doing("Coded by", "Jeroen P. Broks");
	QCol->Doing("Running from", CurrentDir());
	QCol->Doing("Texture dir", TexDir);
	if (FileExists(ConfigFile)) {
		QCol->Doing("Loading", ConfigFile);
		Config = LoadGINIE(ConfigFile, ConfigFile, "Just some bloody config! #");
	} else {
		QCol->Doing("Creating", ConfigFile);
		Config = ParseGINIE("[Creation]\nDate=" + CurrentDate() + "\nTime=" + CurrentTime(), ConfigFile, "New bloody config day! Cool, huh? #");
	}
	if (FileExists(ModifyFile)) {
		QCol->Doing("Loading", ModifyFile);
		ModifyLog = LoadGINIE(ModifyFile, ModifyFile, "Just some logs to detect modifications config! #");
	} else {
		QCol->Doing("Creating", ModifyFile);
		ModifyLog = ParseGINIE("[Creation]\nDate=" + CurrentDate() + "\nTime=" + CurrentTime(), ModifyFile, "New bloody config day! Cool, huh? #");
	}
	QCol->Doing("Loading", AssetJQLiniFile); AssetConfig = LoadGINIE(AssetJQLiniFile);
	QCol->Doing("Scanning", "Artists");
	auto artists{ *FileList(TD("src"),DirWant::Directories) };
	AskGINIE = Config;
	for (auto artist : artists) {
		auto ATag{ "Artist::" + artist };
		if (Yes(ATag, "Add", "Add '" + artist + "' as texture artist")) {
			auto MDTag{ "MultiDir::" + artist };
			if (AssetConfig->Value(MDTag, "Include") == "Y") {
				if (Yes(ATag, "JQLImport", "JQL project data found about '" + artist + "'. Import that")) {
					Config->Value(ATag, "Author", AssetConfig->Value(MDTag, "Author"));
					Config->Value(ATag, "Notes", AssetConfig->Value(MDTag, "Notes"));
				}
			}
			auto wartist = Ask(ATag, "Author", "Author name for '" + artist + "': ", artist);
			auto wnotes = Ask(ATag, "Notes", "Notes for '" + artist + "': ");
			auto basedir{ TD("src/" + artist) };
			auto walldir{ basedir + "/walls" };
			auto walls{ *GetTree(walldir) };
			QCol->Doing("Scanning artist", wartist);
			vector<string> Reasons{};
			for (auto wall : walls) {
				auto fullwall(walldir + "/" + wall);
				if (force) Reasons.push_back("Forced");
				if (FileSize(fullwall) != ModifyLog->IntValue(fullwall, "Size")) Reasons.push_back("Size mismatch");
				if (FileTimeStamp(fullwall) != ModifyLog->IntValue(fullwall, "Time")) Reasons.push_back("Time mismatch");
				if (Reasons.size()) {
					QCol->Doing("Scheduled", wall);
					for (auto Reason : Reasons) {
						QCol->LMagenta("* ");
						QCol->LGreen(Reason);
						QCol->Reset();
						cout << endl;
					}
					Modified.push_back(fullwall);
				}
			}
		}
	}
}