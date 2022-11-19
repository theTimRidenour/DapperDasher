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
    Texture2D charSheet;
    AnimData data;
};

int main()
{
    const int WINDOW_DIMENSIONS[2]{512, 380}; // Width, Height

    InitWindow(WINDOW_DIMENSIONS[0], WINDOW_DIMENSIONS[1], "Dapper Dasher");

    int velocity = 0;

    // textures
        player players[2];
        players[0].charSheet = {LoadTexture("textures/scarfy.png")};
        players[0].data = {
            {0.0, 0.0, (float)players[0].charSheet.width/6, (float)players[0].charSheet.height}, // rec
            {(float)((WINDOW_DIMENSIONS[0]/2) - (players[0].charSheet.width/12)), (float)(WINDOW_DIMENSIONS[1] - players[0].charSheet.height)}, // pos
            0, 1.0/12.0, 0.0 // frame, updateTime, runningTime
        };

        players[1].charSheet = {LoadTexture("textures/santa.png")};
        players[1].data = {
            {0.0, 0.0, (float)players[1].charSheet.width/11, (float)players[1].charSheet.height/2}, // rec
            {(float)((WINDOW_DIMENSIONS[0]/2) - (players[1].charSheet.width/22)), (float)(WINDOW_DIMENSIONS[1] - (players[1].charSheet.height/2))}, // pos
            0, 1.0/12.0, 0.0 // frame, updateTime, runningTime
        };

        // nedula texture
        Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

        // nebula template data
        AnimData nebDataTemp {
            {0.0, 0.0, (float)nebula.width/8, (float)nebula.height/8}, // rec
            {(float)WINDOW_DIMENSIONS[0], (float)(WINDOW_DIMENSIONS[1] - (nebula.height/8))}, // pos
            0, 1.0/12.0, 0.0 // frame, updateTime, runningTime
        };

        AnimData nebulae[2]{nebDataTemp, nebDataTemp};
            nebulae[1].pos.x += 300;
            nebulae[1].updateTime = 1.0/16.0;

    // nebula X velocity (pixels/second)
    int nebVel{-200};

    // jumping
    const int gravity = 1'250; // meters per second
    const int jumpVel = -600;
    bool jumping = false;
    bool doubleJump = false;

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
        for (int i = 0; i < 2; i++) { nebulae[i].runningTime += dT; }

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

        // update animation frames for nebulae
        for (int i = 0; i < 2; i++) {
            if (nebulae[i].runningTime >= nebulae[i].updateTime) {
                if (nebulae[i].frame >= 56) {
                    nebulae[i].rec.y = 7 * nebulae[i].rec.height;
                    nebulae[i].rec.x = (nebulae[i].frame - 56) * nebulae[i].rec.width;
                } else if (nebulae[i].frame >= 48) {
                    nebulae[i].rec.y = 6 * nebulae[i].rec.height;
                    nebulae[i].rec.x = (nebulae[i].frame - 48) * nebulae[i].rec.width;
                } else if (nebulae[i].frame >= 40) {
                    nebulae[i].rec.y = 5 * nebulae[i].rec.height;
                    nebulae[i].rec.x = (nebulae[i].frame - 40) * nebulae[i].rec.width;
                } else if (nebulae[i].frame >= 32) {
                    nebulae[i].rec.y = 4 * nebulae[i].rec.height;
                    nebulae[i].rec.x = (nebulae[i].frame - 32) * nebulae[i].rec.width;
                } else if (nebulae[i].frame >= 24) {
                    nebulae[i].rec.y = 3 * nebulae[i].rec.height;
                    nebulae[i].rec.x = (nebulae[i].frame - 24) * nebulae[i].rec.width;
                } else if (nebulae[i].frame >= 16) {
                    nebulae[i].rec.y = 2 * nebulae[i].rec.height;
                    nebulae[i].rec.x = (nebulae[i].frame - 16) * nebulae[i].rec.width;
                } else if (nebulae[i].frame >= 8) {
                    nebulae[i].rec.y = nebulae[i].rec.height;
                    nebulae[i].rec.x = (nebulae[i].frame - 8) * nebulae[i].rec.width;
                } else {
                    nebulae[i].rec.y = 0;
                    nebulae[i].rec.x = nebulae[i].frame * nebulae[i].rec.width;
                }

                // frame update for nebulae
                nebulae[i].frame++;
                if (nebulae[i].frame > 60) { nebulae[i].frame = 0; }
                nebulae[i].runningTime = 0.0;
            }

            // update nebulae position
            nebulae[i].pos.x += nebVel *dT;
            if (nebulae[i].pos.x < -nebulae[i].rec.width) { nebulae[i].pos.x = WINDOW_DIMENSIONS[0]; }
        }

        // update player's position
        players[activePlayer].data.pos.y += velocity * dT;

        // jumping
        if (jumping) {
            if (players[activePlayer].data.pos.y >= WINDOW_DIMENSIONS[1] - players[activePlayer].data.rec.height) {
                velocity = 0;
                players[activePlayer].data.pos.y = WINDOW_DIMENSIONS[1] - players[activePlayer].data.rec.height;
                jumping = false;
                doubleJump = false;
            } else {
                velocity += gravity * dT;
                if (activePlayer == 1) {
                    if (IsKeyPressed(KEY_SPACE) && !doubleJump) {
                        velocity = jumpVel;
                        doubleJump = true;
                    }
                }
            }
        } else {
            if (IsKeyPressed(KEY_SPACE)) {
                jumping = true;
                velocity = jumpVel;
            }
        }

        // draw characters
        DrawTextureRec(players[activePlayer].charSheet,
                       players[activePlayer].data.rec, 
                       players[activePlayer].data.pos, 
                       WHITE);
        
        // draw nebulae
        for (int i = 0; i < 2; i++) { DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE); }
        
        EndDrawing();
    }
    for (int i=0; i < 2; i++) { UnloadTexture(players[i].charSheet); }
    UnloadTexture(nebula);
    CloseWindow();
}