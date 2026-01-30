#include "directories\include\raylib.h"
#include <stdio.h>
#include <math.h>

#define MAP_WIDTH 20
#define MAP_HEIGHT 20
#define TILE_SIZE 64
#define SCREEN_WIDTH MAP_WIDTH * TILE_SIZE 
#define SCREEN_HEIGHT MAP_HEIGHT * TILE_SIZE

typedef struct
{
    Vector2 pos;
    Vector2 dir;
    Vector2 plane;
} Player;

int world_map[MAP_HEIGHT][MAP_WIDTH];

void drawMap();
void playerMove(Player *p, float delta);
void playerRot(Player *p, float delta);
void RayCasting(Player *p);
int main()
{

    for (int i = 0; i <MAP_HEIGHT; i++)
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            if (!i || !j || i + 1 == MAP_WIDTH || j + 1 == MAP_HEIGHT)
                world_map[i][j] = 1;
            else
                world_map[i][j] = 0;
        }
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
    SetTargetFPS(60);
    Player player = {{MAP_WIDTH / 2.0, MAP_HEIGHT/ 2.0}, {1, 0}, {0, 0.66}};

    while (!WindowShouldClose())
    {

        float delta = GetFrameTime();
        playerMove(&player, delta);
        playerRot(&player, delta);
        BeginDrawing();

        ClearBackground(LIGHTGRAY);

        drawMap();
        DrawCircle(player.pos.x * TILE_SIZE, player.pos.y * TILE_SIZE, TILE_SIZE / 4, RED);                                                                  // player circle
        DrawLine(player.pos.x * TILE_SIZE, player.pos.y * TILE_SIZE, player.pos.x * TILE_SIZE + player.dir.x * TILE_SIZE, player.pos.y * TILE_SIZE + player.dir.y * TILE_SIZE, RED); // draw direction
        EndDrawing();
    }
}

void drawMap()
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            if (world_map[y][x] == 0)
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, LIGHTGRAY);
            else
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKGRAY);
            DrawRectangleLines(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLACK); // outline
        }
    }
}

void playerMove(Player *p, float delta)
{
    float MoveSpeed = delta * 2;
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

    int fUp = 1, fRt = 1;
    if (targetPos.x < p->pos.x)
        fUp = -1;

    if (targetPos.y < p->pos.y)
        fRt = -1;

    if(world_map[(int)(targetPos.y)][(int)p->pos.x] == 0)
        p->pos.y = targetPos.y;
    if(world_map[(int)p->pos.y][(int)(targetPos.x)] == 0)
        p->pos.x = targetPos.x;
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

void RayCasting(Player *p){
    Vector2 ray; 
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        double cameraX = 2.0 * x / SCREEN_WIDTH - 1;
        double rayDirX = p->dir.x + p->plane.x * cameraX;
        double rayDirY = p->dir.y + p->plane.y * cameraX;
        int mapX = (int)p->pos.x;
        int mapY = (int)p->pos.y;
        
    }
    
}