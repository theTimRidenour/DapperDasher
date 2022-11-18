#include "raylib.h"

int main()
{
    const int WIN_WIDTH = 512;
    const int WIN_HEIGHT = 380;

    InitWindow(WIN_WIDTH, WIN_HEIGHT, "Dapper Dasher");

    // scarfy
    int velocity = 0;

    // animation frame
    int frame = 0;
    int nebFrame = 0;
    const float updateTime = 1.0/12.0;
    float runningTime = 0.0;

    // textures
        // santa
        Texture2D santa = LoadTexture("textures/santa.png");
        Rectangle santaRec{0.0, 0.0, santa.width/11, santa.height/2};
        Vector2 santaPos{(WIN_WIDTH/2) - (santaRec.width/2), WIN_HEIGHT - santaRec.height};

        // scarfy
        Texture2D scarfy = LoadTexture("textures/scarfy.png");
        Rectangle scarfyRec{0.0, 0.0, scarfy.width/6, scarfy.height};
        Vector2 scarfyPos{(WIN_WIDTH/2) - (scarfyRec.width/2), WIN_HEIGHT - scarfyRec.height};

        // nedula
        Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
        Rectangle nebRec{0.0, 0.0, nebula.width/8, nebula.height/8};
        Vector2 nebPos{WIN_WIDTH - nebRec.width, WIN_HEIGHT - nebRec.height};

    // nebula X velocity (pixels/second)
    int nebVel{-200};

    // jumping
    const int gravity = 1'250; // meters per second
    const int jumpVel = -600;
    bool jumping = false;
    // bool doubleJump = false;

    // active player (0 = scarfy, 1 = santa)
    int activePlayer{0};

    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(WHITE);
        const float dT{ GetFrameTime() };
        runningTime += dT;

        // update animation frames
        if ( runningTime >= updateTime) {

            if (activePlayer == 0) { scarfyRec.x = frame * scarfyRec.width; }
            if (activePlayer == 1 && !jumping) {
                santaRec.x = frame * santaRec.width;
                santaRec.y = 0;
            } else if (activePlayer == 1 && jumping) {
                if (velocity < 0) {
                    santaRec.x = 0;
                } else {
                    santaRec.x = santaRec.width;
                }
                santaRec.y = santaRec.height;
            }

            if (nebFrame >= 56) {
                nebRec.y = 7 * nebRec.height;
                nebRec.x = (nebFrame - 56) * nebRec.width;
            } else if (nebFrame >= 48) {
                nebRec.y = 6 * nebRec.height;
                nebRec.x = (nebFrame - 48) * nebRec.width;
            } else if (nebFrame >= 40) {
                nebRec.y = 5 * nebRec.height;
                nebRec.x = (nebFrame - 40) * nebRec.width;
            } else if (nebFrame >= 32) {
                nebRec.y = 4 * nebRec.height;
                nebRec.x = (nebFrame - 32) * nebRec.width;
            } else if (nebFrame >= 24) {
                nebRec.y = 3 * nebRec.height;
                nebRec.x = (nebFrame - 24) * nebRec.width;
            } else if (nebFrame >= 16) {
                nebRec.y = 2 * nebRec.height;
                nebRec.x = (nebFrame - 16) * nebRec.width;
            } else if (nebFrame >= 8) {
                nebRec.y = nebRec.height;
                nebRec.x = (nebFrame - 8) * nebRec.width;
            } else {
                nebRec.y = 0;
                nebRec.x = nebFrame * nebRec.width;
            }

            if (!jumping) { frame++; }

            // frame update for scarfy
            if (frame > 5 && activePlayer == 0) { frame = 0; }

            // frame update for santa
            if (frame > 10 && activePlayer == 1) { frame = 0; }

            // frame update for nebula
            nebFrame++;
            if (nebFrame > 60) { nebFrame = 0; }

            runningTime = 0.0;
        }

        // update nebula position
        nebPos.x += nebVel * dT;
        if (nebPos.x < -nebRec.width) { nebPos.x = WIN_WIDTH; }

        // update scarfy's position
        if (activePlayer == 0) { scarfyPos.y += velocity * dT; }

        // update santa's position
        if (activePlayer == 1) { santaPos.y += velocity * dT; }

        // jumping
        if (jumping) {
            if ((scarfyPos.y >= WIN_HEIGHT - scarfyRec.height && activePlayer == 0) ||
                  santaPos.y >= WIN_HEIGHT - santaRec.height && activePlayer == 1) {
                velocity = 0;
                if (activePlayer == 0) { scarfyPos.y = WIN_HEIGHT - scarfyRec.height; }
                if (activePlayer == 1) { santaPos.y = WIN_HEIGHT - santaRec.height; }
                jumping = false;
                // doubleJump = false;
            } else {
                velocity += gravity * dT;
                /* if (IsKeyPressed(KEY_SPACE) && !doubleJump) {
                    velocity = jumpVel;
                    doubleJump = true;
                } */
            }
        } else {
            if (IsKeyPressed(KEY_SPACE)) {
                jumping = true;
                velocity = jumpVel;
            }
        }

        // draw characters
        if (activePlayer == 0) { DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE); }
        if (activePlayer == 1) { DrawTextureRec(santa, santaRec, santaPos, WHITE);}
        DrawTextureRec(nebula, nebRec, nebPos, WHITE);

        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(santa);
    UnloadTexture(nebula);
    CloseWindow();
}