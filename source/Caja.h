#pragma once
#pragma once
#include "raylib.h"
#include <box2d.h>
#include <string>
#include <vector>

//para usar la constante definida en main
extern const float SCALE;

using namespace std;
class Caja
{
private:
    b2Body* body;
    float width;
    float height;
    Color color;

public:
    Caja(b2World& world, float x, float y, float w, float h, Color c, float angleDeg);

    void Draw();
};