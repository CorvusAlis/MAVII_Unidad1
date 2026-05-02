#ifndef NDEBUG
#include <vld.h>
#endif

#include "raylib.h"
#include <box2d.h>
#include <vector>

using namespace std;

//escala para convertir pixeles/metros
//metros a píxeles : *SCALE
//píxeles a metros : / SCALE
const float SCALE = 30.0f;

struct PhysicsBox
{
    b2Body* body;
    float width;
    float height;
    Color color;
};

int main(void)
{
    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Las cajas de Mavix");
    SetTargetFPS(60);

    Color fondo = { 110, 100, 215, 255 };
    Color textoPrincipal = RAYWHITE;
    Color textoSecundario = DARKPURPLE;
    Color sueloColor = Fade(DARKGREEN, 0.7f);

    // Mundo físico - todo se crea DIVIDIENDO el tamaño por scale
    b2Vec2 gravity(0.0f, 9.8f);
    b2World world(gravity);

    // -----------------------------
    // Suelo estático - marca el espacio con el que los elementos interactuan != del suelo visual
    // -----------------------------
    b2BodyDef groundDef;
    groundDef.type = b2_staticBody;
    groundDef.position.Set(
        (screenWidth / 2.0f) / SCALE,
        (screenHeight - 40.0f) / SCALE
    );

    b2PolygonShape groundShape;
    groundShape.SetAsBox(
        (screenWidth / 2.0f) / SCALE,
        20.0f / SCALE
    );

    b2Body* groundBody = world.CreateBody(&groundDef);

    groundBody->CreateFixture(&groundShape, 0.0f);

    /*
    b2Body = objeto lógico (como una entidad)
    b2Shape = la geometría de esa parte
    b2Fixture = “parte física tangible”
    */

    vector<PhysicsBox> boxes;

    // -----------------------------
    // Crear algunas cajas dinámicas
    // -----------------------------
    for (int i = 0; i < 4; i++)
    {
        b2BodyDef boxDef;
        boxDef.type = b2_dynamicBody;
        boxDef.position.Set(
            (300.0f + i * 80.0f) / SCALE,
            (80.0f + i * 20.0f) / SCALE
        );

        b2PolygonShape boxShape;
        boxShape.SetAsBox(
            (25.0f) / SCALE,
            (25.0f) / SCALE
        );

        b2Body* boxBody = world.CreateBody(&boxDef);

        b2FixtureDef boxFixture;
        boxFixture.shape = &boxShape;
        boxFixture.density = 1.0f;
        boxFixture.friction = 0.4f;
        boxFixture.restitution = 0.2f;

        boxBody->CreateFixture(&boxFixture);

        boxes.push_back({ boxBody, 50.0f, 50.0f, Fade(SKYBLUE, 0.95f) });
    }

    while (!WindowShouldClose())
    {
        // Avanzar simulación - sincronizado con el juego a 60fps (definido mas arriba)
        world.Step(1.0f / 60.0f, 8, 3);

        BeginDrawing();
        ClearBackground(fondo);

        // Suelo visual - rectangulo que no tiene fisicas, es solo una imagen
        DrawRectangle(0, screenHeight - 60, screenWidth, 40, sueloColor);

        // Dibujar cajas - aca paso de metros a pixeles MULTIPLICANDO el tamaño por scale
        for (const auto& box : boxes)
        {
            b2Vec2 pos = box.body->GetPosition();
            float angle = box.body->GetAngle() * RAD2DEG;

            Rectangle rect = {
                (pos.x * SCALE) - box.width / 2.0f,
                (pos.y * SCALE) - box.height / 2.0f + 30.0f,
                box.width,
                box.height
            };

            Vector2 origin = { box.width / 2.0f, box.height / 2.0f };
            DrawRectanglePro(rect, origin, angle, box.color);
            //DrawRectangleLinesEx(rect, 2, DARKBLUE);
        }

        DrawRectangle(90, 70, 820, 90, Fade(BLACK, 0.18f));
        DrawText("Bienvenidos a Modelos y Algoritmos para Videojuegos II", 120, 90, 28, textoPrincipal);


        EndDrawing();
    }

    CloseWindow();
    return 0;
}