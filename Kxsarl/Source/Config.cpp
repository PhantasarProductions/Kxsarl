
#include <SlyvStream.hpp>
#include <SlyvGINIE.hpp>
#include <SlyvDirry.hpp>
#include <SlyvQCol.hpp>
#include <SlyvTime.hpp>

#include <TQSG.hpp>

#include "../Headers/Config.hpp"


using namespace Slyvina;
using namespace Units;
using namespace TQSG;

namespace Kxsarl {

	static GINIE _Config{nullptr};

	static GINIE Config() {
		if (!_Config) {
			if (!FileExists(ConfigFile())) {
				QCol->Doing("Creating", ConfigFile());
				SaveString(ConfigFile(), "[Creation]\nDate=" + CurrentDate() + "\nTime=" + CurrentTime());
			}
			QCol->Doing("Loading", ConfigFile());
			_Config = LoadGINIE(ConfigFile(), ConfigFile(), "The Legend Of The Kxarl\n(C) Jeroen P. Broks");
		}
		return _Config;
	}

	inline void DefaultScreen() {
		Config()->NewValue("Screen", "Windowed", "Yes");
		Config()->NewValue("Screen", "Width", std::to_string(DesktopWidth() - 20));
		Config()->NewValue("Screen", "Height", std::to_string(DesktopHeight() - 60));
	}

	std::string ConfigDir() { return Dirry("$AppSupport$/Kxsarl/"); }

	std::string ConfigFile() {
		auto _D{ ConfigDir() + "Config" }; 
		if (!DirectoryExists(_D)) {
			QCol->Doing("Creating dir", _D);
			MakeDir(_D);
		}
		return _D + "/Config.ini";
	}
	int CFG_Width() {
		DefaultScreen();
		return Config()->IntValue("Screen","Width");
	}
	int CFG_Height() {
		DefaultScreen();
		return Config()->IntValue("Screen", "Height");
	}
	bool CFG_Windowed() {
		DefaultScreen();
		return Upper(Config()->Value("Screen", "Windowed")) == "YES";
	}
	std::string CFG_ScreenModeString() {
		if (CFG_Windowed())
			return TrSPrintF("%d x %d (Windowed)", CFG_Width(), CFG_Height());
		else
		return TrSPrintF("%d x %d (FullScreen)", DesktopWidth(),DesktopHeight());
	}
}