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
inline string TD(string k) { return string(TexDir) + k; }

const char* PS1_Convert{
	//	Base
	"# Generated Script!\n\n"
	"function __ConvWall($artist,$tex,$texo,$col,$notes) {\n"
	//	"\tMD " TexDir "Priv.Temp/Out.Tex\n"
	"\techo \"Converting $tex by $artist (trans: $col)\"\n"
	//	"\techo \"" TexDir "/Fake3D/Fake3D 'Kxsarl," TexDir "src/$artist/Walls/$tex,$artist," TexDir "Priv.Temp/Out.Tex,$col,1900,1000," TexDir"Priv.Temp/Ready'\"\n" // debug only
	"\t" TexDir "/Fake3D/Fake3D \"Kxsarl," TexDir "src/$artist/Walls/$tex,$artist," TexDir "Priv.Temp/Out.Tex,$col,1700,720," TexDir"/Priv.Temp/Ready\"\n"
	"\tSleep 5\necho 'Waiting 4 minutes'\n"
	"\tsleep 240\n"
	"echo 'Converting to PNG and cutting transparent parts'\n"
	"__Trans " TexDir "Priv.Temp/Out.Tex $col\n"
	"pushd\n"
	"cd " TexDir "Priv.Temp/Out.Tex\n"
	"echo 'Disposing bitmaps'\n" 
	"RM *.bmp\n"
	"echo 'Packaging textures'\n"
	"njcr add -doj -cm zlib -fc zlib  -author \"$artist\" -notes \"$notes\" \"" TexDir "tgt/$texo.tex\"\n"
	"popd\n"
	"}\n\n"
};

int main(int ac, char** a) {
	bool force{ false };
	string output{ PS1_Convert };
	byte Mix[4]{ 1, 0, 1, 2 };
	output += "function __Trans($dir,$col) {\n";
	for (char i1 = -3; i1 <= 3; ++i1)
		for (byte i2 = Mix[abs(i1)]; i2 <= 5; ++i2)
			output += TrSPrintF("magick $dir/Pic.%d.%d.bmp -transparent \"#$col\" $dir/Pic.%d.%d.png\n", i1, i2, i1, i2);
	output += "}\n\n";
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
	bool anymodifications{ false };
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
			vector<string> Modified{};
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
					anymodifications = true;
					Modified.push_back(wall);
				}
			}
			for (auto modifiedwall : Modified) {
				output += TrSPrintF("__ConvWall \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"\n", artist.c_str(), modifiedwall.c_str(), StripExt(modifiedwall).c_str(), Ask(ATag, "WALLCOLOR::" + modifiedwall, "What color to use for transparency? ", "00ffff").c_str(), wnotes.c_str());
			}
		}
	}

	if (anymodifications) {
		QCol->Doing("Saving", "Convert script");
		SaveString(TexDir "/Priv.Temp/RunConvert.ps1", output);
		QCol->Doing("Running", "Convert script");
		system("pwsh \"" TexDir "Priv.Temp/RunConvert.ps1\"");
	} else {
		QCol->Green("No modifications found.\n");
	}
	QCol->Reset();
}