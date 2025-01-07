using System;
using System.Runtime.InteropServices;
using dllExport;

class Program {
    static void Dog() {
        IntPtr dog = DogDll.createDog();
        DogDll.setName(dog, "richard");
        IntPtr namePtr = DogDll.getName(dog);
        string name = Marshal.PtrToStringAnsi(namePtr);

        Console.WriteLine($"Dog's name is: {name}");

        DogDll.deleteDog(dog);
    }
    static void Sim() {
        try {
            IntPtr confinement = SimDLL.createConfinement(1000, 1000, "cm", 100);
            if (confinement == IntPtr.Zero) {
                Console.WriteLine("Failed to create confinement.");
            }
            else {
                Console.WriteLine("Confinement created successfully.");
                SimDLL.deleteConfinement(confinement);  // Cleanup
            }
        } catch (Exception ex) {
            Console.WriteLine($"Error in Sim: {ex.Message}");
        }
    }

    static void Main() {
        Dog();
        Sim();
    }
}
