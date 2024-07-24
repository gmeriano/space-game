#include <raylib.h>
#include "game.h"
#include "constants.h"
#include <math.h>

int main()
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My first RAYLIB program!");

    Texture tx = LoadTexture("./res/background/spaceBackground.png");
    Texture fire = LoadTexture("./res/flame.png");
    fire.height = GetScreenHeight()/20;
    fire.width = GetScreenWidth();

    Vector2 offset = { 0,0 };

    Game game = Game(SCREEN_WIDTH, SCREEN_HEIGHT);
    game.gameState = HOME;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        offset.y -= GetFrameTime() * 100;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexturePro(tx
                ,(Rectangle) { offset.x, offset.y, (float)GetScreenWidth(), (float)GetScreenHeight() }
                ,(Rectangle) {  0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT }
                ,(Vector2) { 0, 0 }
                , 0
                , WHITE);  
        DrawTexture(fire, 0, GetScreenHeight()*19/20, WHITE);

        game.update();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}