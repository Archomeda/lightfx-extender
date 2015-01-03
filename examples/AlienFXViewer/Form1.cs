using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.MemoryMappedFiles;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AlienFXViewer
{
    public partial class Form1 : Form
    {
        private MemoryMappedFile memoryMappedFile;
        private MemoryMappedViewAccessor accessor;
        private AlienFXFrameworkLinkFile linkFile;

        private bool stopRequested = false;

        private uint lastTick = 0;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.memoryMappedFile = MemoryMappedFile.CreateOrOpen("AlienFXFrameworkLink", Marshal.SizeOf(typeof(AlienFXFrameworkLinkFile)), MemoryMappedFileAccess.ReadWrite);
            this.accessor = this.memoryMappedFile.CreateViewAccessor();

            new Thread(CheckLoop).Start();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.stopRequested = true;
        }

        private void comboBoxDevice_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.UpdateInfo();
        }

        private unsafe void UpdateInfo()
        {
            if (this.comboBoxDevice.Items.Count == 0)
            {
                for (int i = 0; i < linkFile.Devices.Length; i++)
                {
                    var device = linkFile.Devices[i];
                    this.comboBoxDevice.Items.Add(string.Format("{0} ({1}, {2} lights)", new string(device.Name), device.DeviceType, device.NumLights));
                }
            }

            if (this.comboBoxDevice.SelectedIndex < 0)
            {
                this.comboBoxDevice.SelectedIndex = 0;
            }

            this.labelVersion.Text = linkFile.Version.ToString();
            this.labelTick.Text = linkFile.Tick.ToString();
            this.labelIsInitialized.Text = linkFile.IsInitialized.ToString();
            this.labelTiming.Text = linkFile.Timing.ToString();
            this.labelNumDevices.Text = linkFile.NumDevices.ToString();

            var lights = linkFile.Devices[this.comboBoxDevice.SelectedIndex].Lights;

            // Workaround for CS1708
            var light0 = lights[0];
            var light1 = lights[1];
            var light2 = lights[2];
            var light3 = lights[3];

            this.labelInfo1.Text = string.Format("{0}\r\n{1},{2},{3}/{4}", new string(light0.Name), lights[0].Position.X, lights[0].Position.Y, lights[0].Position.Z, lights[0].ActionType);
            this.labelInfo2.Text = string.Format("{0}\r\n{1},{2},{3}/{4}", new string(light1.Name), lights[1].Position.X, lights[1].Position.Y, lights[1].Position.Z, lights[1].ActionType);
            this.labelInfo3.Text = string.Format("{0}\r\n{1},{2},{3}/{4}", new string(light2.Name), lights[2].Position.X, lights[2].Position.Y, lights[2].Position.Z, lights[2].ActionType);
            this.labelInfo4.Text = string.Format("{0}\r\n{1},{2},{3}/{4}", new string(light3.Name), lights[3].Position.X, lights[3].Position.Y, lights[3].Position.Z, lights[3].ActionType);

            this.panelPrimaryColor1.BackColor = Color.FromArgb(lights[0].PrimaryColor.Brightness, lights[0].PrimaryColor.Red, lights[0].PrimaryColor.Green, lights[0].PrimaryColor.Blue);
            this.panelPrimaryColor2.BackColor = Color.FromArgb(lights[1].PrimaryColor.Brightness, lights[1].PrimaryColor.Red, lights[1].PrimaryColor.Green, lights[1].PrimaryColor.Blue);
            this.panelPrimaryColor3.BackColor = Color.FromArgb(lights[2].PrimaryColor.Brightness, lights[2].PrimaryColor.Red, lights[2].PrimaryColor.Green, lights[2].PrimaryColor.Blue);
            this.panelPrimaryColor4.BackColor = Color.FromArgb(lights[3].PrimaryColor.Brightness, lights[3].PrimaryColor.Red, lights[3].PrimaryColor.Green, lights[3].PrimaryColor.Blue);
            this.panelSecondaryColor1.BackColor = Color.FromArgb(lights[0].SecondaryColor.Brightness, lights[0].SecondaryColor.Red, lights[0].SecondaryColor.Green, lights[0].SecondaryColor.Blue);
            this.panelSecondaryColor2.BackColor = Color.FromArgb(lights[1].SecondaryColor.Brightness, lights[1].SecondaryColor.Red, lights[1].SecondaryColor.Green, lights[1].SecondaryColor.Blue);
            this.panelSecondaryColor3.BackColor = Color.FromArgb(lights[2].SecondaryColor.Brightness, lights[2].SecondaryColor.Red, lights[2].SecondaryColor.Green, lights[2].SecondaryColor.Blue);
            this.panelSecondaryColor4.BackColor = Color.FromArgb(lights[3].SecondaryColor.Brightness, lights[3].SecondaryColor.Red, lights[3].SecondaryColor.Green, lights[3].SecondaryColor.Blue);

            this.labelPrimaryColor1.Text = string.Format("{0},{1},{2},{3}", lights[0].PrimaryColor.Red, lights[0].PrimaryColor.Green, lights[0].PrimaryColor.Blue, lights[0].PrimaryColor.Brightness);
            this.labelPrimaryColor2.Text = string.Format("{0},{1},{2},{3}", lights[1].PrimaryColor.Red, lights[1].PrimaryColor.Green, lights[1].PrimaryColor.Blue, lights[1].PrimaryColor.Brightness);
            this.labelPrimaryColor3.Text = string.Format("{0},{1},{2},{3}", lights[2].PrimaryColor.Red, lights[2].PrimaryColor.Green, lights[2].PrimaryColor.Blue, lights[2].PrimaryColor.Brightness);
            this.labelPrimaryColor4.Text = string.Format("{0},{1},{2},{3}", lights[3].PrimaryColor.Red, lights[3].PrimaryColor.Green, lights[3].PrimaryColor.Blue, lights[3].PrimaryColor.Brightness);
            this.labelSecondaryColor1.Text = string.Format("{0},{1},{2},{3}", lights[0].SecondaryColor.Red, lights[0].SecondaryColor.Green, lights[0].SecondaryColor.Blue, lights[0].SecondaryColor.Brightness);
            this.labelSecondaryColor2.Text = string.Format("{0},{1},{2},{3}", lights[1].SecondaryColor.Red, lights[1].SecondaryColor.Green, lights[1].SecondaryColor.Blue, lights[1].SecondaryColor.Brightness);
            this.labelSecondaryColor3.Text = string.Format("{0},{1},{2},{3}", lights[2].SecondaryColor.Red, lights[2].SecondaryColor.Green, lights[2].SecondaryColor.Blue, lights[2].SecondaryColor.Brightness);
            this.labelSecondaryColor4.Text = string.Format("{0},{1},{2},{3}", lights[3].SecondaryColor.Red, lights[3].SecondaryColor.Green, lights[3].SecondaryColor.Blue, lights[3].SecondaryColor.Brightness);
        }

        private void CheckLoop()
        {
            while (!this.stopRequested)
            {
                linkFile = this.ReadMemoryMappedFile();

                if (linkFile.Tick != this.lastTick)
                {
                    this.Invoke(new Action(() => this.UpdateInfo()));
                    this.lastTick = linkFile.Tick;
                }

                Thread.Sleep(1);
            }

            this.accessor.Dispose();
            this.memoryMappedFile.Dispose();
        }

        private AlienFXFrameworkLinkFile ReadMemoryMappedFile()
        {
            AlienFXFrameworkLinkFile linkFile;
            this.accessor.Read(0, out linkFile);
            return linkFile;
        }


    }
}
