#pragma region Code Environment Setup

#include <iostream>
#include <sstream>
#include <map>
#include <cmath>
#include <ctime>
#include <chrono>
//#include<matplotlibcpp.h>
#include <string>
#include <vector>
#include <array>
#include <random>
#include <fstream>


//constant variable for pi 
#ifndef PI
#define PI 3.14159265358979323846
#endif
#ifndef DEBUG
#define DEBUG false
#endif 
#define DLL_EXPORT __declspec(dllexport)


using namespace std;
using namespace chrono;
//namespace plt = matplotlibcpp;
#pragma endregion

#pragma region Data Structures



//custom struct to represent co ordinates 
struct PositionTuple {
    float x = INFINITY, y = INFINITY, z = INFINITY;
};

//custom data structure to store and manipulate co ordinates
class Vector {
private:
    PositionTuple positionTuple;
public:
    //empty constructor for null Vectors
    Vector() {}
    Vector(float ax, float ay, float az) {
        positionTuple.x = ax;
        positionTuple.y = ay;
        positionTuple.z = az;
    }
    //return the value on a specified axis
    float getValueOnAxis(char axis) {
        if (axis == 'x') return positionTuple.x;
        else if (axis == 'y') return positionTuple.y;
        else if (axis == 'z') return positionTuple.z;
        return 0;
    }
    //set the value on a specified axis
    void setValueOnAxis(char axis, float value) {
        if (axis == 'x') positionTuple.x = value;
        else if (axis == 'y') positionTuple.y = value;
        else if (axis == 'z') positionTuple.z = value;
    }
    //add or subtract to the current value of a specified axis
    void moveValueOnAxis(char axis, float value) {
        if (axis == 'x') positionTuple.x += +value;
        else if (axis == 'y') positionTuple.y += value;
        else if (axis == 'z') positionTuple.z += value;
    }
    //add 2 vectors
    static Vector add(Vector v1, Vector v2) {
        return Vector{
            (v1.getValueOnAxis('y') == INFINITY) ? INFINITY : v1.getValueOnAxis('x') + v2.getValueOnAxis('x'),
            (v1.getValueOnAxis('y') == INFINITY) ? INFINITY : v1.getValueOnAxis('y') + v2.getValueOnAxis('y'),
            (v1.getValueOnAxis('y') == INFINITY) ? INFINITY : v1.getValueOnAxis('z') + v2.getValueOnAxis('z'),
        };
    }
    //subtract 2 vectors
    static Vector subtract(Vector v1, Vector v2) {
        return Vector{
            (v1.getValueOnAxis('y') == INFINITY) ? INFINITY : v1.getValueOnAxis('x') - v2.getValueOnAxis('x'),
            (v1.getValueOnAxis('y') == INFINITY) ? INFINITY : v1.getValueOnAxis('y') - v2.getValueOnAxis('y'),
            (v1.getValueOnAxis('y') == INFINITY) ? INFINITY : v1.getValueOnAxis('z') - v2.getValueOnAxis('z'),
        };
    }
    //dot product of 2 vectors x and z only
    static float  dot2D(Vector v1, Vector v2) {
        return v1.getValueOnAxis('x') * v2.getValueOnAxis('x') + v1.getValueOnAxis('z') * v2.getValueOnAxis('z');
    }
    static Vector randomise(int minX, int minZ, int maxX, int maxZ) {
        Vector a = Vector(Vector::random(minX, maxX), INFINITY, Vector::random(minZ, maxZ));
        cout << a.stringRepresentation() << endl;
        return a;
    }
    //global random number generator
    static mt19937 gen;
    static float random(float start = 0, float end = 0) {
        uniform_real_distribution<> dist(start, end);
        return dist(gen);
    }
    //debug: return a string of each of the values of the axis
    string stringRepresentation() {
        ostringstream oss;
        oss << "x: " << positionTuple.x
            << ", y: " << positionTuple.y
            << ", z: " << positionTuple.z;
        return oss.str();
    }
};

//initialize the static random number generator globally
mt19937 Vector::gen(static_cast<unsigned int>(time(0)));
#pragma endregion

#pragma region Objects
//base object class that any physical "thing" inherits from
class Object {
public:
    // Total range (-360 to 360)
    static constexpr int RANGE = 720;
    static std::array<float, RANGE> sinValues;
    static std::array<float, RANGE> cosValues;

    //precompute trig values
    static void initialiseTrigValues() {
        for (int i = -360; i < 360; ++i) {
            // Shift range [-360, 360] to [0, 720]
            int index = i + 360;
            sinValues[index] = sin(i * PI / 180.0);
            cosValues[index] = cos(i * PI / 180.0);
        }
    }
private:
    //where the object is
    Vector position;

public:
    Object(Vector startingPosition)
        : position(startingPosition) {
    }
    //rellocate the object to a new position
    void move(Vector newPosition) {
        position = newPosition;
    }

    //add ior subtract from a specified axis
    void moveInDirection(char axis, float amount) {
        position.moveValueOnAxis(axis, amount);
    }
    //return the position vector
    Vector getPosition() const {
        return position;
    }
    //return the value on a specified axis
    float getPositionOnAxis(char axis) {
        return position.getValueOnAxis(axis);
    }


};


//initialise trig values
std::array<float, Object::RANGE> Object::sinValues;
std::array<float, Object::RANGE> Object::cosValues;

//represents a simple organism that can move
class SimpleLife : public Object {
private:
    const float movementPerTick;
    const float mass;
    const float meetRadius;
    const float TPS;
    vector<tuple<int, Vector>> tickPositions;
    vector<Vector> justPositions;

public:
    SimpleLife(float avgSpeed, Vector startingPosition, float mass, float TPS)
        : Object(startingPosition), mass(mass), meetRadius(meetRadius), TPS(TPS), movementPerTick(avgSpeed / TPS) {
        //get the average movement per tick
        tickPositions.push_back({ 0 ,startingPosition });
    }
    //return meet radius
    float getMeetRadius() {
        return meetRadius;
    }
    //return the generated path
    vector<tuple<int, Vector>>* getPath() {
        return &tickPositions;
    }
    //return just the x values of the path
    vector<float> xPath() {
        vector<float> x;
        for (auto& point : justPositions) {
            x.push_back(point.getValueOnAxis('x'));
        }
        return x;
    }
    //return just the y values of the path
    vector<float> yPath() {
        vector<float> y;
        for (auto& point : justPositions) {
            y.push_back(point.getValueOnAxis('y'));
        }
        return y;
    }
    //return just the z values of the path
    vector<float> zPath() {
        vector<float> z;
        for (auto& point : justPositions) {
            z.push_back(point.getValueOnAxis('z'));
        }
        return z;
    }
    //randomly generate a walk
    int createRandomPath(int tick, float width, float length,int lastTurn) {
        //inital turn from the center
        if (tick == 0) {lastTurn = Vector::random(-360, 360); }
        while (true) {
            //limit degree of motion to create realism
            int randomDeviation = Vector::random(-10, 10);
            //add or subtract to create new angle as all angles are bearings and ensure angle in range
            lastTurn = (lastTurn + randomDeviation) % 360;
            //get trig values of the angle
            float cosLastTurn = cosValues[lastTurn + 360];
            float sinLastTurn = sinValues[lastTurn + 360];
            //CAH adjacent=hypotenuse*cos(theta) cahnge in x = move per tick*cos(lastTurn)
            float moveX = (movementPerTick * cosLastTurn);
            //SOH opposite=hypotenuse*cossintheta) cahnge in y = move per tick*sin(lastTurn)
            float moveZ = (movementPerTick * sinLastTurn);

            //get new x and y co ordinates
            float newX = getPositionOnAxis('x') + moveX;
            float newZ = getPositionOnAxis('z') + moveZ;

            //check if new co ordintaes are within the bounds of the space
            if (abs(newX) <= width / 2 && abs(newZ) <= length / 2) {
                //move to new position
                moveInDirection('x', moveX);
                moveInDirection('z', moveZ);
                //add the new position and the tick to the list of paths
                tickPositions.push_back({ tick, Vector{ getPositionOnAxis('x'), getPositionOnAxis('y'), getPositionOnAxis('z') } });
                justPositions.emplace_back(Vector{ getPositionOnAxis('x'), getPositionOnAxis('y'), getPositionOnAxis('z') });

                return lastTurn;
            }
        
        }
    }
    //reset all values and reutrn to a specified position
    void resetLifeForm(Vector startingPosition) {
        move(startingPosition);
        tickPositions.clear();
        tickPositions.push_back({ 0 ,startingPosition });
    }
    //debug: returns a string of the movement per tick and amount of points in the path
    string getDetails() {
        ostringstream oss;
        oss << "MPT: " << movementPerTick << ", Points: " << tickPositions.size();
        return oss.str();
    }
};

class Apple : public Object {
private:
    float radius;

public:
    Apple(Vector startingPosition,float radius)
        : Object(startingPosition), radius(radius) {
    }
};


class Confinement {
private:
    map<string, unique_ptr<class SimpleLife>> lifeForms;
    map<string, unique_ptr<class Apple>> apples;
    const float length;
    const float width;
    const string metric;
    const int TPS;

public:
    Confinement(float length, float width, string metric, int TPS)
        : length(length), width(width), metric(metric), TPS(TPS) {
    }
   //tick loop
    void runRandomPath(float time) {
        auto lifes = getAllLifeForms();
        //initialize lastTurns with 0 for each life form
        vector<int> lastTurns(lifes.size(), 0);
        //loop through tick at a time
        for (int tick = 0; tick < int(TPS * time); tick++) {
            //loop through the lifes and add an extra movement
            for (int i = 0; i < lifes.size(); i++) {
                auto life = lifes[i];
                lastTurns[i] = life->createRandomPath(tick, width, length, lastTurns[i]);
            }
        }
        for (auto life : lifes) {
            cout << life->getPath()->size() << "\n";
        }
    }

    //retrun ticks per second
    int getTPS() {
        return TPS;
    }
    //return length of confinement
    float getLength() {
        return length;
    }
    //return width of confinement
    float getWidth() {
        return width;
    }
    //create a simple life form within the confinement 
    void addSimpleLife(string identifier, float avgSpeed, Vector startingPosition, float mass ) {
        lifeForms[identifier] = make_unique<SimpleLife>(avgSpeed, startingPosition, mass,  getTPS());
    }
    //return a life form with specified identifier
    SimpleLife* getLifeForm(string identifier) {
        return lifeForms[identifier].get();
    }
    //return all life forms
    vector<SimpleLife*> getAllLifeForms() {
        vector<SimpleLife*> allLife;
        for (int i = 0; i < lifeForms.size(); i++) {
            auto it = lifeForms.begin();
            //like a pointer advance it to the ith position
            advance(it, i);
            //add the life to the vector
            allLife.emplace_back(it->second.get());
        }
        return allLife;
    }
    //clea the list of life forms
    void removeAllLife() {
        lifeForms.clear();
    }
};

#pragma endregion



//int main() {
//    Object::initialiseTrigValues();
//    Confinement space(1000, 1000, "m", 100);
//
//    add simple life forms to the confinement space
//    space.addSimpleLife("ethan", 2.0, Vector::randomise(-space.getLength() / 2, -space.getWidth() / 2, space.getLength() / 2, space.getWidth() / 2), 1);
//    space.addSimpleLife("john", 2.0, Vector::randomise(-space.getLength() / 2, -space.getWidth() / 2, space.getLength() / 2, space.getWidth() / 2), 1);
//    space.addSimpleLife("raj", 2.0, Vector::randomise(-space.getLength() / 2, -space.getWidth() / 2, space.getLength() / 2, space.getWidth() / 2), 1);
//    space.addSimpleLife("terry", 2.0, Vector::randomise(-space.getLength() / 2, -space.getWidth() / 2, space.getLength() / 2, space.getWidth() / 2), 1);
//    space.runRandomPath(3600);
//    space.getAllLifeForms()[0]->yPath();
//
//
//
//    return 0;
//}




extern "C" {
    DLL_EXPORT Confinement* createConfinement(float length, float width, const char* metric, int TPS) {
        return new Confinement(length, width, metric, TPS);
    }
    DLL_EXPORT void addSimpleLife(Confinement* confinement, string& identifier, float avgSpeed, float mass) {
        float halfX = -confinement->getLength() / 2;
        float halfZ = -confinement->getWidth() / 2;
        confinement->addSimpleLife(identifier, avgSpeed, Vector::randomise(-halfX,-halfZ,halfX,halfZ), mass);
    }
    DLL_EXPORT void runRandomPath(Confinement* confinement, float time) {
        confinement->runRandomPath(time);
    }
    DLL_EXPORT void deleteConfinement(Confinement* confinement) {
        delete confinement;
    }
}