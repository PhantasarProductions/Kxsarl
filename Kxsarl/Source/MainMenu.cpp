#include <SlyvLinkedList.hpp>

#include "../Headers/MainMenu.hpp"

using namespace std;
using namespace Slyvina;
using namespace Units;

namespace Kxsarl {
	static unique_ptr<TList<MainMenuItem>> MenuItems{TList<MainMenuItem>::CreateUnique() };

	static bool AlwaysAllowed(MainMenuItem*) { return true; }

	MainMenuItem::MainMenuItem(std::string _Caption, void (*_Action)(MainMenuItem*) = nullptr, bool (*_Allow)(MainMenuItem*) = nullptr) {
		Caption = _Caption;
		Action = _Action;
		if (_Allow)
			Allow = _Allow;
		else
			Allow = AlwaysAllowed;
		(*MenuItems) += this;
	}

	void MainMenuItem::InitMenuItems() {
		new MainMenuItem("Character Creation");
		new MainMenuItem("Start Game");
		new MainMenuItem("Jukebox");
		new MainMenuItem("Report bugs");
		new MainMenuItem("System configuration");
		new MainMenuItem("Transfer");
		new MainMenuItem("Exit");
	}
}