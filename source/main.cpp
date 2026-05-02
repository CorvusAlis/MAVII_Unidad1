#ifndef NDEBUG
#include <vld.h>
#endif

#include "raylib.h"
#include <box2d.h>
#include <vector>

#include "Caja.h"

using namespace std;

void DrawPreviewCaja(float x, float y, float w, float h, float angleDeg);

//escala para convertir pixeles/metros
//metros a píxeles : *SCALE
//píxeles a metros : / SCALE
const float SCALE = 30.0f;


int main(void)
{
    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Las cajas de Mavix");
    SetTargetFPS(60);

    //bg
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

    //creo un vector de cajas para rellenar cuando spawneen
    vector<Caja> boxes;

    //rotacion con flechas - 90 grados por segundo
    float spawnAngle = 0.0f;

    while (!WindowShouldClose())
    {

        if (IsKeyDown(KEY_RIGHT))
        {
            spawnAngle += 90.0f * GetFrameTime();
        }

        if (IsKeyDown(KEY_LEFT))
        {
            spawnAngle -= 90.0f * GetFrameTime();
        }

        DrawPreviewCaja(60.0f, 60.0f, 50.0f, 30.0f, spawnAngle);

        //creo cajas con la tecla espacio - aca solo se crea la logica de la fisica en el vector
        if (IsKeyPressed(KEY_SPACE))
        {
            boxes.emplace_back(
                world,
                GetRandomValue(200, 800),   //genera las cajas en un lugar aleatorio en la parte superior de la pantalla
                50.0f,
                50.0f,
                30.0f,
                Fade(SKYBLUE, 0.95f),
                spawnAngle
            );
        }

        //SIMULACION
        // Avanzar simulación - sincronizado con el juego a 60fps (definido mas arriba)
        world.Step(1.0f / 60.0f, 8, 3);

        BeginDrawing();
        ClearBackground(fondo);

        // Suelo visual - rectangulo que no tiene fisicas, es solo una imagen
        DrawRectangle(0, screenHeight - 60, screenWidth, 40, sueloColor);

        //dibujo de las cajas
        for (auto& caja : boxes)
        {
            caja.Draw();
        }

        DrawText("<-Preview - Spawnea cajas con ESPACIO - Giralas con las FLECHAS", 100, 30, 24, textoPrincipal);


        EndDrawing();
    }

    CloseWindow();
    return 0;
}

//PREVIEW DE CAJA
//muestro como se va a generar la siguiente caja
void DrawPreviewCaja(float x, float y, float w, float h, float angleDeg)
{
    Rectangle rect = {
        x - w / 2.0f,
        y - h / 2.0f,
        w,
        h
    };

    Vector2 origin = { w / 2.0f, h / 2.0f };

    DrawRectanglePro(rect, origin, angleDeg, Fade(SKYBLUE, 0.4f));
}