// Lic:
// The Legend of the Kxsarl
// Transfer Main Window (code)
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
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using TrickyUnits;

namespace Kxsarl_Transfer {
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window {
        public MainWindow() {
            InitializeComponent();
            KxsarlTransfer.GetChars(LB_TransferrableChars);
            MayExport();
        }

        private string StorageMethod {
            get {
                if (Storage.SelectedItem == null) return "";
                return Storage.SelectedItem.ToString().Replace("System.Windows.Controls.ComboBoxItem:","").Trim();
            }
        }
        
        private void MayExport() {
            //Confirm.Annoy(StorageMethod); // debug!
            if (Act_Export == null) Confirm.Annoy("Act_Export is null");
            if (TB_ExportTo == null) Confirm.Annoy("ExportTo Texbox is null");
            Act_Export.IsEnabled = TB_ExportTo.Text != "" && StorageMethod!="";            
        }

        private void Browse_ExportTo_Click(object sender, RoutedEventArgs e) {
            var WantFile = FFS.RequestFile(true);
            if (WantFile != "") TB_ExportTo.Text = WantFile;
            MayExport();
        }

        private void Storage_SelectionChanged(object sender, SelectionChangedEventArgs e) {
            MayExport();
        }

        private void Act_Export_Click(object sender, RoutedEventArgs e) {
            KxsarlTransfer.Pack(TB_ExportTo.Text, StorageMethod);
        }

        private void TB_ExportTo_TextChanged(object sender, TextChangedEventArgs e) => MayExport();
    }
}