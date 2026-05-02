#include "Caja.h"

//constructor
Caja::Caja(b2World& world, float x, float y, float w, float h, Color c)
    : width(w), height(h), color(c)
{
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(x / SCALE, y / SCALE);

    b2PolygonShape shape;
    shape.SetAsBox((w / 2.0f) / SCALE, (h / 2.0f) / SCALE);

    body = world.CreateBody(&def);

    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = 1.0f;
    fixture.friction = 0.4f;
    fixture.restitution = 0.2f;

    body->CreateFixture(&fixture);
}

void Caja::Draw()
{
    b2Vec2 pos = body->GetPosition();
    float angle = body->GetAngle() * RAD2DEG;

    Rectangle rect = {
        (pos.x * SCALE) - width / 2.0f,
        (pos.y * SCALE) - height / 2.0f,
        width,
        height
    };

    Vector2 origin = { width / 2.0f, height / 2.0f };

    DrawRectanglePro(rect, origin, angle, color);
    //DrawRectangleLinesEx(rect, 2, DARKBLUE); // debug
}