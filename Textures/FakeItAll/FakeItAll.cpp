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
// Version: 23.07.28
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

#define ConfigFile ExtractDir(a[0]) + "/FakeItAll.ini"
#define TexDir "/Projects/Applications/Slyvina/Apps/Kxsarl/Textures/"

using namespace std;
using namespace Slyvina;
using namespace Units;

UGINIE Config;
inline string TD(string k) { return string(TexDir) + k; }

int main(int ac, char** a) {
	QCol->Green("Fake it all\n");
	QCol->Doing("Coded by", "Jeroen P. Broks");
	QCol->Doing("Running from", CurrentDir());
	QCol->Doing("Texture dir", TexDir);
	if (FileExists(ConfigFile)) {
		QCol->Doing("Loading", ConfigFile);
		Config = LoadUGINIE(ConfigFile, ConfigFile, "# Just some bloody config!");
	} else {
		QCol->Doing("Creating", ConfigFile);
		Config = ParseUGINIE("[Creation]\nDate=" + CurrentDate() + "\nTime=" + CurrentTime(),ConfigFile,"# New bloody config day! Cool, huh?");
	}
	QCol->Doing("Scanning", "Artists");
	auto artists{ *FileList(TD("src")) };

}