using System;
using System.Runtime.InteropServices;

namespace dllExport {
    public static class SimDLL {
        private const string DllName = "otherC/myDLL/SimulationAttempt1/sim.dll";
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr createConfinement(float length, float width, string metric, int TPS);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void deleteConfinement(IntPtr confinement);
    }
}
