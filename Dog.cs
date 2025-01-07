using System;
using System.Runtime.InteropServices;


namespace dllExport {
    public static class DogDll {
        private const string DllName = "otherC/myDLL/Dog/dog.dll";

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr createDog();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void deleteDog(IntPtr dog);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void setName(IntPtr dog, string name);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr getName(IntPtr dog);
    }
}
