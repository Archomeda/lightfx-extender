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

        private bool isActive = false;
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

        private void CheckLoop()
        {
            this.isActive = true;

            while (!this.stopRequested)
            {
                AlienFXFrameworkLinkFile linkFile = this.ReadMemoryMappedFile();

                if (linkFile.Tick != this.lastTick)
                {
                    this.Invoke(new Action(() =>
                    {
                        this.labelVersion.Text = linkFile.Version.ToString();
                        this.labelTick.Text = linkFile.Tick.ToString();
                        this.labelIsInitialized.Text = linkFile.IsInitialized.ToString();
                        this.labelTiming.Text = linkFile.Timing.ToString();

                        this.labelPosition.Text = string.Format("{0},{1},{2}", linkFile.Light.Position.X, linkFile.Light.Position.Y, linkFile.Light.Position.Z);
                        this.labelActionType.Text = linkFile.Light.ActionType.ToString();
                        this.labelPrimaryColor.Text = string.Format("{0},{1},{2},{3}", linkFile.Light.PrimaryColor.Red, linkFile.Light.PrimaryColor.Green, linkFile.Light.PrimaryColor.Blue, linkFile.Light.PrimaryColor.Brightness);
                        this.labelSecondaryColor.Text = string.Format("{0},{1},{2},{3}", linkFile.Light.SecondaryColor.Red, linkFile.Light.SecondaryColor.Green, linkFile.Light.SecondaryColor.Blue, linkFile.Light.SecondaryColor.Brightness);

                        this.panelPrimaryColor.BackColor = Color.FromArgb(linkFile.Light.PrimaryColor.Brightness, linkFile.Light.PrimaryColor.Red, linkFile.Light.PrimaryColor.Green, linkFile.Light.PrimaryColor.Blue);
                        this.panelSecondaryColor.BackColor = Color.FromArgb(linkFile.Light.SecondaryColor.Brightness, linkFile.Light.SecondaryColor.Red, linkFile.Light.SecondaryColor.Green, linkFile.Light.SecondaryColor.Blue);
                    }));
                    this.lastTick = linkFile.Tick;
                }

                Thread.Sleep(1);
            }

            this.isActive = false;
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
