using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
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

namespace SampleUI_DesktopApp
{
    /// <summary>
    /// ** Interaction logic for MainWindow.xaml
    /// * This code interfaces the UI and allows users to  input desired blink
    /// * duration and upload these directly to the connected Arduino Micro board
    /// * without opening the Arduino IDE
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            int blind_duration;

            InitializeComponent();
        }

        // Button Click Event
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            // ==============================================================
            // TODO: Obtain value from the Textbox and update this into
            //       a separate header file in which the main() can call
            // ==============================================================
            /* On-click: new batch process */
            // Batch file cd's to the correct .ino file that will be uploaded;
            System.Diagnostics.Process proc = new System.Diagnostics.Process();

            // Determines where the batch and working directory is
            // Albert: This initalizes the filename and workdir to the Current Directory
            //         Right now, the bin\\Debug folder is used as default but I'm not sure
            //         if will still be the same folder on Release. Ask Albert for more details.
            proc.StartInfo.FileName = System.IO.Directory.GetCurrentDirectory() + "\\blink\\upload_blink.bat";
            proc.StartInfo.WorkingDirectory = System.IO.Directory.GetCurrentDirectory() + "\\blink";
            proc.Start();
        }

        private void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            
        }
    }
}

//DUMP
/*
System.Diagnostics.ProcessStartInfo proc = new System.Diagnostics.ProcessStartInfo();
// proc.RedirectStandardInput = true;
// Indicate that we want to write to standard input of
// process
//proc.RedirectStandardOutput = true;
//proc.UseShellExecute = false;
proc.FileName = @"cmd.exe";
            proc.Arguments = "/K " + "cd/";
            proc.Arguments = "/K " + "cd C:/Users/Albert/Documents/ELEC/Capstone/Featherboard";
            System.Diagnostics.Process.Start(proc);
            */