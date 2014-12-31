using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace AlienFXViewer
{
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public unsafe struct AlienFXFrameworkLinkFile
    {
        public uint Version;
        public uint Tick;

        public uint IsInitialized;
        public int Timing;

        public AlienFXFrameworkLight Light;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct AlienFXFrameworkLight
    {
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
