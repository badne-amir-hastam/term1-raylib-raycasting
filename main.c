#include "directories\include\raylib.h"
#include <stdio.h>
#include <math.h>

#define AMPLIFIRE 50
#define MAP_WIDTH 20
#define MAP_HEIGHT 20
#define TILE_SIZE 1
#define PLAYER_R 0.25 * AMPLIFIRE

typedef struct
{
    Vector2 pos;
    Vector2 dir;
    Vector2 plane;
    Color color;
}Player;

int world_map[MAP_HEIGHT][MAP_WIDTH];

void drawMap();
void playerMove(Player *p, float delta);
void playerRot(Player *p, float delta);

int main()
{
    for (int i = 0; i < MAP_HEIGHT; i++)
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            if (!i || !j || i + 1 == MAP_HEIGHT || j + 1 == MAP_WIDTH)
                world_map[i][j] = 1;
            else
                world_map[i][j] = 0;
        }
    InitWindow(MAP_WIDTH * AMPLIFIRE, MAP_HEIGHT * AMPLIFIRE, "map");
    SetTargetFPS(60);
    Player player = {{MAP_HEIGHT * AMPLIFIRE / 2.0, MAP_HEIGHT * AMPLIFIRE / 2.0}, {0, -1}, {1, 0}};
    while (!WindowShouldClose())
    {
        float delta = GetFrameTime(), speed = 1;
        playerMove(&player, delta);
        playerRot(&player, delta);
        BeginDrawing();
        drawMap();
        DrawCircle(player.pos.x, player.pos.y, PLAYER_R, RED);                                                                       // player circle
        DrawLine(player.pos.x, player.pos.y, player.pos.x + player.dir.x /2 * AMPLIFIRE, player.pos.y + player.dir.y / 2 * AMPLIFIRE, RED); // draw direction
        EndDrawing();
    }
}

void drawMap()
{
    ClearBackground(LIGHTGRAY);
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            DrawLine(x * AMPLIFIRE, y * AMPLIFIRE, x * AMPLIFIRE, (y + TILE_SIZE) * AMPLIFIRE, BLACK);
            DrawLine(x * AMPLIFIRE, y * AMPLIFIRE, (x + TILE_SIZE) * AMPLIFIRE, y * AMPLIFIRE, BLACK);
            Color s = LIGHTGRAY;
            if (world_map[y][x])
                s = DARKGRAY;
            DrawRectangle((x)*AMPLIFIRE, (y)*AMPLIFIRE, TILE_SIZE * AMPLIFIRE, TILE_SIZE * AMPLIFIRE, s);
        }
    }
}

void playerMove(Player *p, float delta)
{
    float MoveSpeed = 5 * delta * AMPLIFIRE;
    Vector2 targetPos = p->pos;
    if (IsKeyDown(KEY_W))
    {
        targetPos.x += MoveSpeed * p->dir.x;
        targetPos.y += MoveSpeed * p->dir.y;
    }
    if (IsKeyDown(KEY_S))
    {
        targetPos.x -= MoveSpeed * p->dir.x;
        targetPos.y -= MoveSpeed * p->dir.y;
    }
    if (IsKeyDown(KEY_D))
    {
        targetPos.x -= MoveSpeed * p->dir.y;
        targetPos.y += MoveSpeed * p->dir.x;
    }
    if (IsKeyDown(KEY_A))
    {
        targetPos.x += MoveSpeed * p->dir.y;
        targetPos.y -= MoveSpeed * p->dir.x;
    }
    int fUp = 1, fRt = 1;//check for wall sliding
    if (targetPos.x < p->pos.x)
        fUp = -1;

    if (targetPos.y < p->pos.y)
        fRt = -1;

    if (targetPos.x * targetPos.y > 0)//checks if our character is in the map
    {
        if (!world_map[(int)(p->pos.y / AMPLIFIRE)][(int)((targetPos.x + (PLAYER_R - 1) * fUp) / AMPLIFIRE)])
            p->pos.x = targetPos.x;

        if (!world_map[(int)((targetPos.y + (PLAYER_R - 1) * fRt) / AMPLIFIRE)][(int)(p->pos.x / AMPLIFIRE)])
            p->pos.y = targetPos.y;
    } //-- we add -1 bc on -x andd -y movment, when we change cordition to int, it stops sooner with out touching the wall --
}

void playerRot(Player *p, float delta)
{
    double rotSpeed = 4 * delta;
    if (IsKeyDown(KEY_LEFT))
    {
        float oldx = p->dir.x;
        p->dir.x = p->dir.x * cos(-rotSpeed) - p->dir.y * sin(-rotSpeed);
        p->dir.y = oldx * sin(-rotSpeed) + p->dir.y * cos(-rotSpeed);

        float oldPlaneX = p->plane.x;
        p->plane.x = p->plane.x * cos(-rotSpeed) - p->plane.y * sin(-rotSpeed);
        p->plane.y = oldPlaneX * sin(-rotSpeed) + p->plane.y * cos(-rotSpeed);
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        float oldx = p->dir.x;
        p->dir.x = p->dir.x * cos(rotSpeed) - p->dir.y * sin(rotSpeed);
        p->dir.y = oldx * sin(rotSpeed) + p->dir.y * cos(rotSpeed);

        
        float oldPlaneX = p->plane.x;
        p->plane.x = p->plane.x * cos(rotSpeed) - p->plane.y * sin(rotSpeed);
        p->plane.y = oldPlaneX * sin(rotSpeed) + p->plane.y * cos(rotSpeed);
    }
}