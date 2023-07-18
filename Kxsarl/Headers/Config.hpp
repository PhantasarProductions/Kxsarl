
#pragma once
#include <string>

namespace Kxsarl {
	std::string ConfigDir();
	std::string ConfigFile();

	int CFG_Width();
	int CFG_Height();
	bool CFG_Windowed();

	std::string CFG_ScreenModeString();
}