#include "directories\include\raylib.h"
#include <stdio.h>
#include <math.h>

#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define TILE_SIZE 64
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640

typedef struct
{
    Vector2 pos;
    Vector2 dir;
    Vector2 plane;
    Color color;
} Player;

int world_map[MAP_WIDTH][MAP_HEIGHT];

void drawMap();
void playerMove(Player *p, float delta);
void playerRot(Player *p, float delta);

int main()
{

    Vector2 rayDir;
    float stepX, stepY, sideDistX, sideDistY, deltaDistX, deltaDistY;
    for (int i = 0; i < MAP_WIDTH; i++)
        for (int j = 0; j < MAP_HEIGHT; j++)
        {
            if (!i || !j || i + 1 == MAP_WIDTH || j + 1 == MAP_HEIGHT)
                world_map[i][j] = 1;
            else
                world_map[i][j] = 0;
        }
    // InitWindow(MAP_WIDTH * AMPLIFIRE, MAP_HEIGHT * AMPLIFIRE, "map");
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
    SetTargetFPS(60);
    Player player = {{SCREEN_WIDTH / 2.0, SCREEN_WIDTH / 2.0}, {0, -1}, {1, 0}};

    while (!WindowShouldClose())
    {

        float delta = GetFrameTime(), speed = 1;
        playerMove(&player, delta);
        playerRot(&player, delta);
        BeginDrawing();
        drawMap();
        DrawCircle(player.pos.x, player.pos.y, TILE_SIZE/4, RED);                                                                               // player circle
        DrawLine(player.pos.x, player.pos.y, player.pos.x + player.dir.x * TILE_SIZE, player.pos.y + player.dir.y * TILE_SIZE, RED); // draw direction
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
            if(world_map[x][y] == 0)
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, LIGHTGRAY);
            else
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKGRAY);
            DrawRectangleLines(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLACK);//outline
        }
    }
}

void playerMove(Player *p, float delta)
{
    float MoveSpeed = TILE_SIZE * delta * 2;
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
    int x = (int)(targetPos.x + TILE_SIZE)/64, y  = (int)(targetPos.y + TILE_SIZE)/64;
    if(abs(x - (targetPos.x + TILE_SIZE)/64) > 0.5){
        x++;
    }
    if(abs(y - (targetPos.y + TILE_SIZE)/64) > 0.5){
        y++;
    }

    int fUp = 1, fRt = 1;
    if (targetPos.x < p->pos.x)
        fUp = -1;

    if (targetPos.y < p->pos.y)
        fRt = -1;

    if (targetPos.x * targetPos.y > 0)//checks if our character is in the map
    {
        if (!world_map[(int)((targetPos.x + (TILE_SIZE/4) * fUp) / TILE_SIZE)][(int)(p->pos.y / TILE_SIZE)])
            p->pos.x = targetPos.x;

        if (!world_map[(int)(p->pos.x / TILE_SIZE)][(int)((targetPos.y + (TILE_SIZE / 4) * fRt) / TILE_SIZE)])
            p->pos.y = targetPos.y;
    } 
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

/*              phase 2
 int mapX = floor(player.pos.x), mapY = floor(player.pos.y), res = 0;
        float cameraX = 2 * player.pos.x / SCREEN_WIDTH, side;
        rayDir.x = player.dir.x + player.plane.x * cameraX;
        rayDir.y = player.dir.y + player.plane.y * cameraX;

        if (rayDir.x < 0)
        {
            stepX = -1;
            sideDistX = (player.pos.x - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - player.pos.x) * deltaDistX;
        }

        if (rayDir.y < 0)
        {
            stepY = -1;
            sideDistY = (player.pos.y - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - player.pos.y) * deltaDistY;
        }

        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            // 1. Calculate ray position and direction
            double cameraX = 2 * x / (double)SCREEN_WIDTH - 1;
            double rayDirX = player.dir.x + player.plane.x * cameraX;
            double rayDirY = player.dir.y + player.plane.y * cameraX;
            // 2. Identify which box of the map we're in
            int mapX = (int)player.pos.x;
            int mapY = (int)player.pos.y;
            deltaDistX = abs(1 / rayDir.x);
            deltaDistY = abs(1 / rayDir.y);

            if (sideDistX < sideDistY)
            {
                mapX += stepX;
                sideDistX += deltaDistX;
                side = 0;
            }
            else
            {
                mapY += stepY;
                sideDistY += deltaDistY;
                side = 1;
            }
            if (world_map[mapX][mapY] > 0)
            {
                res = 1;
                break;
            }

            float perpWallDist;
            if (side == 0)
                perpWallDist = (sideDistX - deltaDistX);
            else
                perpWallDist = (sideDistY - deltaDistY);

            int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
            int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
            if (drawStart < 0)
                drawStart = 0;
            int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
            if (drawEnd == SCREEN_HEIGHT)
                drawEnd = SCREEN_HEIGHT - 1;

            Color color = RED;
            if(side == 1){
                color = MAROON;
            }

            DrawLine(x, drawStart, x, drawEnd, color);


        }
*/