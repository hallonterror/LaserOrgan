using System;
using System.Windows;
using System.Windows.Controls;
using System.IO.Ports;
using System.Windows.Controls.Primitives;
using System.Collections.Generic;

namespace SimpleMIDIKeyboard
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        SerialPort serial;
        const Int32 MaxButtons = 6;
        Int32 CurrentButtons = 0;

        public MainWindow()
        {
            InitializeComponent();

            SetupSerial("COM15", 115200);
            CreateButtonLayout();
        }
    

        private void CreateButtonLayout()
        {
            Int32 vertical = 11;
            Int32 horizontal = 12;
            String[] note = new String[] { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

            for (Int32 r = 0; r < vertical; ++r)
            {
                for (Int32 c = 0; c < horizontal; ++c)
                {
                    if (r * horizontal + c > 127)
                        break;

                    ToggleButton button = new ToggleButton();
                    //Button button = new Button();
                    button.Content = note[c] + "(" + Convert.ToString(r - 1) + ")";
                    button.Checked += ButtonDown;
                    button.Unchecked += ButtonUp;
                    Grid.SetColumn(button, c);
                    Grid.SetRow(button, r);
                    ButtonGrid.Children.Add(button);
                }
            }
        }

        private void SetupSerial(String port, int baud)
        {
            try
            {
                serial = new SerialPort(port, baud);
                serial.DataReceived += Serial_DataReceived;
                serial.Open();
            }
            catch(Exception ex)
            {
                InfoList.Items.Add("Failed to open serial port. Reason: ");
                InfoList.Items.Add(ex.Message.ToString());
                InfoList.ScrollIntoView(InfoList.Items[InfoList.Items.Count - 1]);
            }
        }

        private void Serial_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            Application.Current.Dispatcher.Invoke(new Action(() =>
            {
                char[] content = new char[serial.BytesToRead];
                serial.Read(content, 0, serial.BytesToRead);
                InfoList.Items.Add(new string(content));
                InfoList.ScrollIntoView(InfoList.Items[InfoList.Items.Count - 1]);
            }));
        }

        private void ButtonDown(object sender, RoutedEventArgs e)
        {
            ToggleButton button = (ToggleButton)sender;
            if (CurrentButtons >= MaxButtons)
            {
                button.IsChecked = false;
                CurrentButtons++;  // Work around as the toogle off even decrements this (fix later)
                e.Handled = true;
                return;
            }

            int c = Grid.GetColumn(button);
            int r = Grid.GetRow(button);

            byte status = 0x90;                 // Just a valid status
            byte pitch = (byte)(r * 12 + c);    // Get the correct midi tone
            byte velocity = 127;                // A really strong note

            try
            {
                serial.Write(new byte[] { status, pitch, velocity }, 0, 3);
            }
            catch (Exception ex)
            {
                InfoList.Items.Add("Failed to write to serial port. Reason: ");
                InfoList.Items.Add(ex.Message.ToString());
                InfoList.ScrollIntoView(InfoList.Items[InfoList.Items.Count - 1]);
            }
            CurrentButtons++;
        }

        private void ButtonUp(object sender, RoutedEventArgs e)
        {
            ToggleButton button = (ToggleButton)sender;
            int c = Grid.GetColumn(button);
            int r = Grid.GetRow(button);

            byte status = 0x80;                 // Off status
            byte pitch = (byte)(r * 12 + c);    // Get the correct midi tone
            byte velocity = 0;                // A really strong note
            try
            {
                serial.Write(new byte[] { status, pitch, velocity }, 0, 3);
            }
            catch (Exception ex)
            {
                InfoList.Items.Add("Failed to write to serial port. Reason: ");
                InfoList.Items.Add(ex.Message.ToString());
                InfoList.ScrollIntoView(InfoList.Items[InfoList.Items.Count - 1]);
            }
            CurrentButtons--;
        }
    }
}
