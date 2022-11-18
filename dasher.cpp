#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

struct player
{
    Texture2D name;
    AnimData data;
};

int main()
{
    const int WINDOW_DIMENSIONS[2]{512, 380}; // Width, Height

    InitWindow(WINDOW_DIMENSIONS[0], WINDOW_DIMENSIONS[1], "Dapper Dasher");

    int velocity = 0;

    // textures
        // santa
        Texture2D santa = LoadTexture("textures/santa.png");
        AnimData santaData {
            {0.0, 0.0, (float)santa.width/11, (float)santa.height/2}, // rec
            {(float)((WINDOW_DIMENSIONS[0]/2) - (santa.width/22)), (float)(WINDOW_DIMENSIONS[1] - (santa.height/2))}, // pos
            0, 1.0/12.0, 0.0 // frame, updateTime, runningTime
        };

        // scarfy
        Texture2D scarfy = LoadTexture("textures/scarfy.png");
        AnimData scarfyData {
            {0.0, 0.0, (float)scarfy.width/6, (float)scarfy.height}, // rec
            {(float)((WINDOW_DIMENSIONS[0]/2) - (scarfy.width/12)), (float)(WINDOW_DIMENSIONS[1] - scarfy.height)}, // pos
            0, 1.0/12.0, 0.0 // frame, updateTime, runningTime
        };

        player players[2]{{scarfy, scarfyData}, {santa, santaData}};

        // nedula 1
        Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
        AnimData neb1Data {
            {0.0, 0.0, (float)nebula.width/8, (float)nebula.height/8}, // rec
            {(float)WINDOW_DIMENSIONS[0], (float)(WINDOW_DIMENSIONS[1] - (nebula.height/8))}, // pos
            0, 1.0/12.0, 0.0 // frame, updateTime, runningTime
        };

        // nebula 2
        AnimData neb2Data {
            neb1Data.rec, {(float)WINDOW_DIMENSIONS[0] + 300, neb1Data.pos.y}, // rec, pos
            0, 1.0/16.0, 0.0 // frame, updateTime, runningTime
        };

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

        // switch player
        if (IsKeyPressed(KEY_ONE)) { activePlayer = 0; }
        if (IsKeyPressed(KEY_TWO)) { activePlayer = 1; }

        ClearBackground(WHITE);
        const float dT{ GetFrameTime() };
        players[activePlayer].data.runningTime += dT;
        neb1Data.runningTime += dT;
        neb2Data.runningTime += dT;

        // update animation frames for characters
        if (players[activePlayer].data.runningTime >= players[activePlayer].data.updateTime) {
            players[activePlayer].data.rec.x = 
                players[activePlayer].data.frame * players[activePlayer].data.rec.width;
            if (activePlayer == 1 && !jumping) {
                players[1].data.rec.y = 0;
            } else if (activePlayer == 1 && jumping) {
                if (velocity < 0) {
                    players[1].data.rec.x = 0;
                } else {
                    players[1].data.rec.x = players[1].data.rec.width;
                }
                players[1].data.rec.y = players[1].data.rec.height;
            }
            if (!jumping) { players[activePlayer].data.frame++; }

            // frame update for player
            if ((activePlayer == 0 && players[activePlayer].data.frame > 5) ||
                (activePlayer == 1 && players[activePlayer].data.frame >10)) {
                    players[activePlayer].data.frame = 0;
                }
            
            players[activePlayer].data.runningTime = 0.0;
        }

        // update animation frames from nebula 1
        if (neb1Data.runningTime >= neb1Data.updateTime) {
            if (neb1Data.frame >= 56) {
                neb1Data.rec.y = 7 * neb1Data.rec.height;
                neb1Data.rec.x = (neb1Data.frame - 56) * neb1Data.rec.width;
            } else if (neb1Data.frame >= 48) {
                neb1Data.rec.y = 6 * neb1Data.rec.height;
                neb1Data.rec.x = (neb1Data.frame - 48) * neb1Data.rec.width;
            } else if (neb1Data.frame >= 40) {
                neb1Data.rec.y = 5 * neb1Data.rec.height;
                neb1Data.rec.x = (neb1Data.frame - 40) * neb1Data.rec.width;
            } else if (neb1Data.frame >= 32) {
                neb1Data.rec.y = 4 * neb1Data.rec.height;
                neb1Data.rec.x = (neb1Data.frame - 32) * neb1Data.rec.width;
            } else if (neb1Data.frame >= 24) {
                neb1Data.rec.y = 3 * neb1Data.rec.height;
                neb1Data.rec.x = (neb1Data.frame - 24) * neb1Data.rec.width;
            } else if (neb1Data.frame >= 16) {
                neb1Data.rec.y = 2 * neb1Data.rec.height;
                neb1Data.rec.x = (neb1Data.frame - 16) * neb1Data.rec.width;
            } else if (neb1Data.frame >= 8) {
                neb1Data.rec.y = neb1Data.rec.height;
                neb1Data.rec.x = (neb1Data.frame - 8) * neb1Data.rec.width;
            } else {
                neb1Data.rec.y = 0;
                neb1Data.rec.x = neb1Data.frame * neb1Data.rec.width;
            }

            // frame update for nebula
            neb1Data.frame++;
            if (neb1Data.frame > 60) { neb1Data.frame = 0; }
            neb1Data.runningTime = 0.0;
        }

        // update animation frames from nebula 2
        if (neb2Data.runningTime >= neb2Data.updateTime) {
            if (neb2Data.frame >= 56) {
                neb2Data.rec.y = 7 * neb2Data.rec.height;
                neb2Data.rec.x = (neb2Data.frame - 56) * neb2Data.rec.width;
            } else if (neb2Data.frame >= 48) {
                neb2Data.rec.y = 6 * neb2Data.rec.height;
                neb2Data.rec.x = (neb2Data.frame - 48) * neb2Data.rec.width;
            } else if (neb2Data.frame >= 40) {
                neb2Data.rec.y = 5 * neb2Data.rec.height;
                neb2Data.rec.x = (neb2Data.frame - 40) * neb2Data.rec.width;
            } else if (neb2Data.frame >= 32) {
                neb2Data.rec.y = 4 * neb2Data.rec.height;
                neb2Data.rec.x = (neb2Data.frame - 32) * neb2Data.rec.width;
            } else if (neb2Data.frame >= 24) {
                neb2Data.rec.y = 3 * neb2Data.rec.height;
                neb2Data.rec.x = (neb2Data.frame - 24) * neb2Data.rec.width;
            } else if (neb2Data.frame >= 16) {
                neb2Data.rec.y = 2 * neb2Data.rec.height;
                neb2Data.rec.x = (neb2Data.frame - 16) * neb2Data.rec.width;
            } else if (neb2Data.frame >= 8) {
                neb2Data.rec.y = neb2Data.rec.height;
                neb2Data.rec.x = (neb2Data.frame - 8) * neb2Data.rec.width;
            } else {
                neb2Data.rec.y = 0;
                neb2Data.rec.x = neb2Data.frame * neb2Data.rec.width;
            }

            // frame update for nebula 2
            neb2Data.frame++;
            if (neb2Data.frame > 60) { neb2Data.frame = 0; }
            neb2Data.runningTime = 0.0;
        }

        // update nebula 1 position
        neb1Data.pos.x += nebVel * dT;
        if (neb1Data.pos.x < -neb1Data.rec.width) { neb1Data.pos.x = WINDOW_DIMENSIONS[0]; }

        // update nebula 2 position
        neb2Data.pos.x += nebVel * dT;
        if(neb2Data.pos.x < -neb2Data.rec.width) { neb2Data.pos.x = WINDOW_DIMENSIONS[0]; }

        // update player's position
        players[activePlayer].data.pos.y += velocity * dT;

        // jumping
        if (jumping) {
            if (players[activePlayer].data.pos.y >= WINDOW_DIMENSIONS[1] - players[activePlayer].data.rec.height) {
                velocity = 0;
                players[activePlayer].data.pos.y = WINDOW_DIMENSIONS[1] - players[activePlayer].data.rec.height;
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
        DrawTextureRec(players[activePlayer].name,
                       players[activePlayer].data.rec, 
                       players[activePlayer].data.pos, 
                       WHITE);
        
        // draw nebulas
        DrawTextureRec(nebula, neb1Data.rec, neb1Data.pos, WHITE);
        DrawTextureRec(nebula, neb2Data.rec, neb2Data.pos, RED);

        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(santa);
    UnloadTexture(players[0].name);
    UnloadTexture(players[1].name);
    UnloadTexture(nebula);
    CloseWindow();
}