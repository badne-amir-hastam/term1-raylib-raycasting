#include "directories/include/raylib.h"
#include <stdio.h>
#include <math.h>

#define MAP_WIDTH 20
#define MAP_HEIGHT 20
#define TILE_SIZE 20
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

typedef struct
{
    Vector2 pos;
    Vector2 dir;
    Vector2 plane;
} Player;

typedef enum
{
    STATE_PLAY_MODE,
    STATE_EDIT_MODE
} GameState;

int world_map[MAP_HEIGHT][MAP_WIDTH];

void drawMap();
void playerMove(Player *p, float delta);
void playerRot(Player *p, float delta);
void RayCasting(Player *p);
void loadMap(char *filename, Player *p);
void saveMap(char *fileName, Player *p);

int main()
{
    // init map
    for (int i = 0; i < MAP_HEIGHT; i++)
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            if (!i || !j || i + 1 == MAP_WIDTH || j + 1 == MAP_HEIGHT)
                world_map[i][j] = -1;
            else
                world_map[i][j] = 0;
        }

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
    SetTargetFPS(60);
    Player player = {{MAP_WIDTH / 2.0, MAP_HEIGHT / 2.0}, {1, 0}, {0, 0.66}};
    GameState state = STATE_PLAY_MODE;
    int w = GetMonitorWidth(0);
    int h = GetMonitorHeight(0);
    int posX, posY;
    while (!WindowShouldClose())
    {

        float delta = GetFrameTime();
        playerMove(&player, delta);
        playerRot(&player, delta);
        if (IsKeyPressed(KEY_M))
        {
            if (state == STATE_PLAY_MODE)
            {
                state = STATE_EDIT_MODE;
                SetWindowSize(TILE_SIZE * MAP_WIDTH, TILE_SIZE * MAP_HEIGHT);
                posX = (w - MAP_WIDTH * TILE_SIZE) / 2;
                posY = (h - MAP_HEIGHT * TILE_SIZE) / 2;
                SetWindowPosition(posX, posY);
            }
            else
            {
                state = STATE_PLAY_MODE;
                SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
                posX = (w - SCREEN_WIDTH) / 2;
                posY = (h - SCREEN_HEIGHT) / 2;
                SetWindowPosition(posX, posY);
            }
        }
        BeginDrawing();
        if (state == STATE_PLAY_MODE)
            RayCasting(&player);
        else
        {
            drawMap();
            DrawCircle(player.pos.x * TILE_SIZE, player.pos.y * TILE_SIZE, TILE_SIZE / 4, RED);
            DrawLine(player.pos.x * TILE_SIZE, player.pos.y * TILE_SIZE, (player.pos.x + player.dir.x * 0.8) * TILE_SIZE, (player.pos.y + player.dir.y * 0.8) * TILE_SIZE, RED); // draw direction
            int f = 0, status;
            Vector2 mouse;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                mouse = GetMousePosition();
                status = 1;
                if (mouse.x < TILE_SIZE * MAP_WIDTH && mouse.x > 0 && mouse.y < TILE_SIZE * MAP_HEIGHT && mouse.y > 0)
                    f = 1;
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                mouse = GetMousePosition();
                status = 2;
                if (mouse.x < TILE_SIZE * MAP_WIDTH && mouse.x > 0 && mouse.y < TILE_SIZE * MAP_HEIGHT && mouse.y > 0)
                    f = 1;
            }
            if (f)
            {
                int mouseTileX = (int)(mouse.x / TILE_SIZE);
                int mouseTileY = (int)(mouse.y / TILE_SIZE);
                if (!((mouseTileX == (int)player.pos.x && mouseTileY == (int)player.pos.y) || world_map[mouseTileY][mouseTileX] == -1))
                {
                    if (status == 1)
                    {
                        world_map[mouseTileY][mouseTileX] = 1;
                    }
                    else
                    {
                        world_map[mouseTileY][mouseTileX] = 0;
                    }
                }
            }
        }

        EndDrawing();
        if (IsKeyPressed(KEY_F5))
            saveMap("MAP.txt",&player);
        if (IsKeyPressed(KEY_F9))
            loadMap("MAP.txt",&player);
    }
}

void drawMap()
{
    ClearBackground(LIGHTGRAY);
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            if (world_map[y][x] == 0)
                DrawRectangle(+x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, LIGHTGRAY);
            else if (world_map[y][x] == 1)
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKGRAY);
            else
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, RED);
            DrawRectangleLines(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLACK); // outline
        }
    }
}

void playerMove(Player *p, float delta)
{
    float MoveSpeed = delta * 5;
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

    if (world_map[(int)(targetPos.y)][(int)p->pos.x] == 0)
        p->pos.y = targetPos.y;
    if (world_map[(int)p->pos.y][(int)(targetPos.x)] == 0)
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

void RayCasting(Player *p)
{

    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        double cameraX = 2.0 * x / SCREEN_WIDTH - 1;
        double rayDirX = p->dir.x + p->plane.x * cameraX;
        double rayDirY = p->dir.y + p->plane.y * cameraX;
        int mapX = (int)p->pos.x;
        int mapY = (int)p->pos.y;

        double deltaDistX = (rayDirX != 0 ? fabs(1.0 / rayDirX) : 1e10);
        double deltaDistY = (rayDirY != 0 ? fabs(1.0 / rayDirY) : 1e10);
        double sideDistX, sideDistY;
        int stepX, stepY;

        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (p->pos.x - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - p->pos.x) * deltaDistX;
        }
        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (p->pos.y - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - p->pos.y) * deltaDistY;
        }

        // dda
        int side, wallType;
        while (1)
        {
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
            if (world_map[mapY][mapX] == 1)
            {
                wallType = 1;
                break;
            }
            if (world_map[mapY][mapX] == -1)
            {
                wallType = -1;
                break;
            }
        }
        double perpWallDist;
        if (side)
            perpWallDist = sideDistY - deltaDistY;
        else
            perpWallDist = sideDistX - deltaDistX;
        // draw

        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0)
            drawStart = 0;
        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT)
            drawEnd = SCREEN_HEIGHT - 1;

        Color color = (wallType == 1) ? GRAY : RED;
        if (side == 1)
        {
            color = (wallType == 1) ? LIGHTGRAY : MAROON;
        }
        DrawLine(x, 0, x, drawStart, VIOLET);      // ceiling
        DrawLine(x, drawStart, x, drawEnd, color); // wall
        DrawLine(x, drawEnd, x, SCREEN_HEIGHT, DARKGRAY);
    }
}

void saveMap(char *fileName, Player *p)
{
    FILE *file = fopen(fileName, "w");
    if (!file)
    {
        printf("ERROR");
        return;
    }
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            fprintf(file, "%d ", world_map[y][x]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "%f %f", p->pos.x, p->pos.y);
    fclose(file);
    printf("Map saved successfuly in: %s\n", fileName);
}

void loadMap(char *filename, Player *p)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("ERROR");
        return;
    }

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            fscanf(file, "%d", &world_map[y][x]);
        }
    }
    fscanf(file, " %f %f", &p->pos.x, &p->pos.y);
    world_map[(int)p->pos.y][(int)p->pos.x] = 0;

    fclose(file);
    printf("Map loaded successfuly from: %s\n", filename);
}