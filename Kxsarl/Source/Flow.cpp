// Lic:
// The Legend of The Kxsarl
// Flow manager
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
#pragma once

#include <SlyvString.hpp>

#include <TQSE.hpp>
#include <TQSG.hpp>

#include "../Headers/Debug.hpp"
#include "../Headers/Error.hpp"
#include "../Headers/Flow.hpp"
#include "../Headers/Config.hpp"

#include "../Headers/MainMenu.hpp"

using namespace std;
using namespace Slyvina;
using namespace Units;

namespace Kxsarl {
	static map<string, FlowFunction> FlowRegister{};
	static map<string, FlowArrive> FlowArriveRegister{};
	FlowFunction CurrentFlow{ nullptr };

	void RegFlow(std::string k, FlowFunction f,FlowArrive a) {
		Trans2Upper(k);
		FlowRegister[k] = f;
		CSay(TrSPrintF("Flow run function %08x has been registered on %s", (uint64)f, k.c_str()));
		if (a) {
			FlowArriveRegister[k] = a;
			CSay(TrSPrintF("Flow arrive function %08x has been registered on %s", (uint64)a, k.c_str()));
		}
	}
	void GoFlow(std::string k) {
		Trans2Upper(k);
		if (!FlowRegister.count(k)) Crash("Flow \"" + k + "\" doesn't exist!");
		CSay("Going to flow: " + k);
		if (FlowArriveRegister.count(k))
			GoFlow(FlowRegister[k], FlowArriveRegister[k]);
		else
			GoFlow(FlowRegister[k]);
	}

	void GoFlow(FlowFunction f,FlowArrive a) {
#ifdef KXSARL_DEBUG
		CSay(TrSPrintF("Going to flow #%08x", (uint64)f));
#endif
		if (a) a();
		CurrentFlow = f;
	}
	void FlowInt() {
		QCol->Doing("Init", "Flow functions");
		RegFlow("MainMenu", Flow_MainMenu,Arrive_MainMenu);
		RegFlow("Config", Flow_Config, Arrive_Config);
	}

	void Run() {
		QCol->Doing("Starting", "The Flow");
		GoFlow(Flow_MainMenu,Arrive_MainMenu);
		do {
			TQSG::Flip();
			TQSG::Cls();
			TQSE::Poll();
			if (TQSE::AppTerminate()) {
				QCol->Doing("Received", "User request to terminate the application");
				if (TQSE::Yes("You've requested to quit the game!\nPlease note that any unsaved progress will be lost!\n\nDo you still wish to quit?")) break;
			}
			if (!CurrentFlow) Crash("Nullflow (Internal error. Please report!)");
		} while (CurrentFlow());
		QCol->Doing("Ending", "The Flow");
	}
}