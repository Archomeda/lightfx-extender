using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace LightFXViewer
{
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct LightFXExtenderLinkFile
    {
        public uint Version;
        public uint Tick;

        public uint IsInitialized;
        public int Timing;

        public uint NumDevices;
        public AlienFXFrameworkDeviceArray Devices;
    }

    public struct AlienFXFrameworkDeviceArray
    {
        public AlienFXFrameworkDevice Device1;
        public AlienFXFrameworkDevice Device2;
        public AlienFXFrameworkDevice Device3;
        public AlienFXFrameworkDevice Device4;
        public AlienFXFrameworkDevice Device5;
        public AlienFXFrameworkDevice Device6;
        public AlienFXFrameworkDevice Device7;
        public AlienFXFrameworkDevice Device8;

        public AlienFXFrameworkDevice this[int index]
        {
            get
            {
                switch (index)
                {
                    case 0: return this.Device1;
                    case 1: return this.Device2;
                    case 2: return this.Device3;
                    case 3: return this.Device4;
                    case 4: return this.Device5;
                    case 5: return this.Device6;
                    case 6: return this.Device7;
                    case 7: return this.Device8;
                    default: throw new IndexOutOfRangeException();
                }
            }
        }

        public int Length
        {
            get { return 8; }
        }
    }

    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public unsafe struct AlienFXFrameworkDevice
    {
        public fixed sbyte Name[32];
        public AlienFXFrameworkDeviceType DeviceType;
        public uint NumLights;
        public AlienFXFrameworkLightsArray Lights;
    }

    public struct AlienFXFrameworkLightsArray
    {
        public AlienFXFrameworkLight Light1;
        public AlienFXFrameworkLight Light2;
        public AlienFXFrameworkLight Light3;
        public AlienFXFrameworkLight Light4;
        public AlienFXFrameworkLight Light5;
        public AlienFXFrameworkLight Light6;
        public AlienFXFrameworkLight Light7;
        public AlienFXFrameworkLight Light8;
        public AlienFXFrameworkLight Light9;
        public AlienFXFrameworkLight Light10;
        public AlienFXFrameworkLight Light11;
        public AlienFXFrameworkLight Light12;
        public AlienFXFrameworkLight Light13;
        public AlienFXFrameworkLight Light14;
        public AlienFXFrameworkLight Light15;
        public AlienFXFrameworkLight Light16;

        public AlienFXFrameworkLight this[int index]
        {
            get
            {
                switch (index)
                {
                    case 0: return this.Light1;
                    case 1: return this.Light2;
                    case 2: return this.Light3;
                    case 3: return this.Light4;
                    case 4: return this.Light5;
                    case 5: return this.Light6;
                    case 6: return this.Light7;
                    case 7: return this.Light8;
                    case 8: return this.Light9;
                    case 9: return this.Light10;
                    case 10: return this.Light11;
                    case 11: return this.Light12;
                    case 12: return this.Light13;
                    case 13: return this.Light14;
                    case 14: return this.Light15;
                    case 15: return this.Light16;
                    default: throw new IndexOutOfRangeException();
                }
            }
        }

        public int Length
        {
            get { return 16; }
        }
    }

    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public unsafe struct AlienFXFrameworkLight
    {
        public fixed sbyte Name[32];
        public AlienFXFrameworkLightPosition Position;
        public AlienFXFrameworkActionType ActionType;
        public AlienFXFrameworkLightColor PrimaryColor;
        public AlienFXFrameworkLightColor SecondaryColor;
    }


    public enum AlienFXFrameworkActionType
    {
        Morph = 1,
        Pulse,
        Color
    }

    public enum AlienFXFrameworkDeviceType
    {
        Unknown = 0,
        Notebook,
        Desktop,
        Server,
        Display,
        Mouse,
        Keyboard,
        Gamepad,
        Speaker,
        Other = 0xFF
    }


    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct AlienFXFrameworkLightPosition
    {
        public byte X;
        public byte Y;
        public byte Z;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct AlienFXFrameworkLightColor
    {
        public byte Red;
        public byte Green;
        public byte Blue;
        public byte Brightness;
    }
}
