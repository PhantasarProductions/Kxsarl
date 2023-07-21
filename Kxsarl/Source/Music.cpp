#include <TQSA.hpp>
#include "../Headers/Error.hpp"
#include "../Headers/Music.hpp"
#include "../Headers/UseJCR6.hpp"

using namespace Slyvina;
using namespace TQSA;

namespace Kxsarl {
    bool HasMusic() { return MRes()->EntryExists("Music/Present"); }
    std::string MusicPresent() { if (HasMusic()) return "Present"; return "Absent"; } // Dirty, but works.


    void Music(std::string File) {
        static std::string LastFile{"*"};
        static TAudio __MusicBuf[2];
        static bool __CMusicBuf{ false };
        static int __Channel{ 0 };
        QCol->Doing("Music", File);
        if (!HasMusic()) {
            QCol->Red("Ignored! "); QCol->Pink("No music present in this installation\m"); return;
        }
        Trans2Upper(File);
        if (File == LastFile) {
            QCol->Red("Ignored! "); QCol->Pink("Same as before. No need to reload!\m"); return;
            return;
        }
        if (!MRes()->EntryExists(File)) Crash("Music not found", { {"Music File",File} });
        QCol->Yellow("= Loading\n");
        __MusicBuf[__CMusicBuf] = LoadAudio(MRes(),File);
        if (!__MusicBuf[__CMusicBuf]) Crash("Loading music failed", { {"Music File",File},{"SDL",Mix_GetError()},{"JCR6",JCR6::Last()->ErrorMessage},{"TQSA",AudioError()} });
        if (LastFile != "*") {
            Mix_HaltChannel(__Channel);
        }
        QCol->Yellow("= Playing\n");
        __Channel = __MusicBuf[__CMusicBuf]->Play(-1);
        QCol->Doing("= Channel", __Channel);
        __CMusicBuf != __CMusicBuf;
        LastFile = File;
    }
}

