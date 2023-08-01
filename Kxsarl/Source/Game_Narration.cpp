// Lic:
// The Legend Of the Kxsarl
// Narration
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
// Version: 23.08.01
// EndLic

// Tag for debugging purposes. 
// Having to watch this every time something goes wrong in something following up, is gonna get frustrating VERY SOON!

// Must both be on #undef in release
#undef SKIP_NARRATION
#undef KXSARL_DEBUG_NARRATION

#include <TQSE.hpp>
#include <TQSA.hpp>

#include "AllHeaders.hpp"

using namespace Slyvina;
using namespace TQSG;
using namespace TQSA;
using namespace TQSE;

namespace Kxsarl {  
	namespace Game {
		struct BrokenLine {
			VecString Lines{ nullptr };
			int32 StartY{ 0 };
			int32 H{ 0 };
			TAudio Voice{ nullptr };
		};

		static BrokenLine BreakLine(std::string L, _____TIMAGEFONT* F, std::string Nar, size_t idx) {
			static auto W{ ScreenWidth()-20 };
			static auto H{ ScreenHeight() };
			static auto CY{ H / 2 };
			auto Words{ Split(L,' ') };
			auto ret{ NewVecString() };
			std::string NewLine{""};
			for (auto Word : *Words) {
				if (NewLine.size() && F->Width(NewLine + " " + Word) >= W) {
					ret->push_back(NewLine);
					NewLine = "";
				}
				if (Word != "") {
					if (NewLine.size()) NewLine += " ";
					NewLine += Word;
				}
			}
			if (NewLine.size()) ret->push_back(NewLine);
			TAudio Voice{ nullptr };
			auto VoiceFile{ TrSPrintF("Game/%s/Narration/%s/Voice%d.ogg", GameID.c_str(), Nar.c_str(), idx + 1) };
			if (MRes()->EntryExists(VoiceFile)) Voice = LoadAudio(MRes(), VoiceFile);
			auto TH{ F->Height("The quick brown fox jumps over the lazy dog") };
#ifdef KXSARL_DEBUG_NARRATION
			QCol->Doing("Narration line", idx + 1);
			QCol->Doing("=> Lines", ret->size());
			QCol->Doing("=> Text Height", TH);
			QCol->Doing("=> Centre Y", CY);
			QCol->Doing("=> Text Start", CY - ((((int)ret->size()) * TH) / 2));
			QCol->Doing("=> Voice", boolstring(Voice!=nullptr));
#endif
			return { ret,CY - ((((int)ret->size()) * TH) / 2),TH,Voice };
		}
		
		void StartNarration(std::string NarrationEvent, bool(*ReturnFlow)(), std::string ReturnEventParameters) {
#ifndef SKIP_NARRATION
			SysStatus("Loading");
			auto NarrationFile{ TrSPrintF("Game/%s/Narration/%s/%s",GameID.c_str(),NarrationEvent.c_str(),CFG_Lang().c_str()) };
			if (!MRes()->EntryExists(NarrationFile)) { Crash("Narration text not found!", { { "Entry",NarrationFile } }); return; }
			auto Lines{ MRes()->GetLines(NarrationFile) };
			QCol->Doing("Narration", NarrationEvent);
			QCol->Doing("=> Lines", (int)Lines->size());
			auto Background{ LoadUImage(MRes(),TrSPrintF("Game/%s/Narration/%s/Background.png",GameID.c_str(),NarrationEvent.c_str())) };
			if (!Background) Crash("Error loading narration background", { {"Game",GameID},{"Narration",NarrationEvent } });
			size_t Line{ 0 };
			auto Font{ LoadUImageFont(MRes(),TrSPrintF("Game/%s/Narration/%s/Font.jfbf",GameID.c_str(),NarrationEvent.c_str())) };
			auto StrL{ BreakLine((*Lines)[Line],Font.get(),NarrationEvent,0) };
			int VoiceChannel{ -1 }; if (StrL.Voice) VoiceChannel = StrL.Voice->Play();
			auto MusicFile{ TrSPrintF("Game/%s/Narration/%s/Music",GameID.c_str(),NarrationEvent.c_str()) };
			static std::vector<std::string> MusicExt = { ".ogg",".mp3" };
			for (auto ext : MusicExt) { if (MRes()->EntryExists(MusicFile + ext)) { Music(MusicFile + ext); break; } }
			while (true) {
				Poll();
				if (KeyHit(SDLK_ESCAPE)) break;
				bool VoiceEnd{ StrL.Voice && (!Mix_Playing(VoiceChannel)) };
				if (KeyHit(SDLK_SPACE) || KeyHit(SDLK_RETURN) || VoiceEnd) {
					if (++Line >= Lines->size()) break;
					StrL = BreakLine((*Lines)[Line], Font.get(), NarrationEvent, Line);
					VoiceChannel = -1; if (StrL.Voice) VoiceChannel = StrL.Voice->Play();
				}
				Cls(); 
				SetColor(255, 255, 255);
				Background->StretchDraw(0, 0, ScreenWidth(), ScreenHeight());
				for (size_t i = 0; i < StrL.Lines->size(); ++i) {
					Font->Text((*StrL.Lines)[i], ScreenWidth() / 2, StrL.StartY + (StrL.H * i), Align::Center);
				}
				Flip();
			}
#endif // !SKIP_NARRATION
			if (ReturnFlow) GoFlow(ReturnFlow);
		}
	} 
}