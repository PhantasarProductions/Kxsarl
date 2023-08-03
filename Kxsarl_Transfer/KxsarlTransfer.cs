// Lic:
// The Legend Of The Kxsarl
// Transferring itself and configuration in general
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
// Version: 23.08.03
// EndLic
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using TrickyUnits;
using UseJCR6;

namespace Kxsarl_Transfer {
	static internal class KxsarlTransfer {

		static GINIE GlobalConfig = null;

		static string GlobalConfigFile => Dirry.C("$AppSupport$/Kxsarl/Config/Config.ini");
		static string SavedCharsDir => Dirry.AD(GlobalConfig["DIRECTORY", "SAVEDCHARS"]);
		static string[] SavedChars = null;
		static List<string> TransChars = null;

        #region Export
        static public void GetChars(ListBox LB) { 
			LB.Items.Clear();
			TransChars = new List<string>();
			foreach(var CHID in SavedChars) {
				var CHFile = SavedCharsDir + "/" + CHID + "/General.jcr";
				if (File.Exists(CHFile)) {
					var JDir = JCR6.Dir(CHFile);
					var Base = GINIE.FromSource(JDir.LoadString("Base.ini")); if (Base["GEN", "NAME"] == "") Base["GEN", "NAME"] = "<???>";
					var Item = $"{Base["GEN","NAME"]}\t{Base["GEN","SEX"]} {Base["GEN","CLASS"]}";
					if (Base["DEATH","PERMADEATH"].ToUpper()!="TRUE" && Base["DEATH", "SUPERPERMADEATH"].ToUpper() != "TRUE") {
						TransChars.Add(CHID);                        
						LB.Items.Add(Item);
					}
				}
			}
		}

		static public void Pack(string outfile,string storage) {
			storage = storage.Trim();
			if (storage == "None") storage = "Store";
			var OJ = new TJCRCreate(outfile, storage);
			var OJB=new TJCRCreateBlock(OJ,storage);
			if (JCR6.JERROR != "") {
				Confirm.Annoy(JCR6.JERROR, "Error!", System.Windows.Forms.MessageBoxIcon.Error);
				return;
			}
			OJB.AddString("[ID]\nTransfer=KXSARL", "ID.ini");
			var charlist = new StringBuilder();
			foreach(var CH in TransChars) {
				var CHBL = new TJCRCreateBlock(OJ, storage);
				var CHFL = FileList.GetTree(SavedCharsDir + "/" + CH);
				var CHJR = JCR6.Dir(SavedCharsDir + "/" + CH + "/General.jcr");
				var CHDT = GINIE.FromSource(CHJR.LoadString("Base.ini"));
				CHBL.AddString("Check!", "Check/" + CH);
				if (CHDT["GEN", "NAME"] == "") CHDT["GEN", "NAME"] = "<???>";
				charlist.Append($"{CHDT["GEN", "NAME"]}\t{CHDT["GEN", "SEX"]} {CHDT["GEN", "CLASS"]}\n");
				foreach (var CHFile in CHFL) {
					CHBL.AddFile(SavedCharsDir + "/" + CH + "/" + CHFile, "Characters/" + CH + "/" + CHFile);
				}
				CHBL.Close();
			}
			OJB.AddString(charlist, "Chars",storage);
			OJ.Close();
			if (JCR6.JERROR != "")
				Confirm.Annoy($"{JCR6.JCATCH.Message}\nEntry: {JCR6.JCATCH.entry}", "Something went wrong", System.Windows.Forms.MessageBoxIcon.Error);
			else
				Confirm.Annoy("Transfer data saved as " + outfile + "; Storage(" + storage + ")");
		}
        #endregion

        #region Import
		public static bool RecognizeTrans(string file,bool notify=true, ListBox LB = null) {
			var R = "NE"; if (!File.Exists(file)) goto Mislukt; // I really HATE the "goto" command, but in this case it was the safest and easiest way to go!
			R = "NRBJ"; if (JCR6.Recognize(file) == "NONE") goto Mislukt;
			R = "INVID"; var RTJ = JCR6.Dir(file);
            var Check = GINIE.FromSource(RTJ.LoadString("ID.ini"));
			if (Check["ID", "Transfer"] != "KXSARL") goto Mislukt;
			if (LB != null) {
				LB.Items.Clear();
				var Chars = RTJ.ReadLines("Chars", true);
				foreach(var Char in Chars) LB.Items.Add(Char);
			}
			return true;
		Mislukt:
			if (notify) Confirm.Annoy($"File \"{file}\" does either not exist or has not been recognized as a transfer file\n({R})", "Error", System.Windows.Forms.MessageBoxIcon.Error) ;
			return false;
		}
        #endregion


        #region Init
        static KxsarlTransfer() {
			JCR6_zlib.Init();
			JCR6_lzma.Init();
			Dirry.InitAltDrives();
			//Confirm.Annoy(Dirry.C("$AppSupport$/Config/Config.ini"));
			GlobalConfig = GINIE.FromFile(GlobalConfigFile);
			//Confirm.Annoy(SavedCharsDir); // debug
			SavedChars = FileList.GetDir(SavedCharsDir, 2);
		}
        #endregion
    }
}