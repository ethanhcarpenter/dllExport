#include <string>
#include <cstring>
#define DLL_EXPORT __declspec(dllexport)

class Dog {
private:
    std::string name;
public:
    void setName(const std::string& inputName) {
        name = inputName;
    }

    const char* getName() {
        return name.c_str();
    }
};

extern "C" {
    DLL_EXPORT Dog* createDog() {
        return new Dog();
    }

    DLL_EXPORT void setName(Dog* dog, const char* name) {
        dog->setName(name);
    }

    DLL_EXPORT const char* getName(Dog* dog) {
        return dog->getName();
    }

    DLL_EXPORT void deleteDog(Dog* dog) {
        delete dog;
    }
}
