#include "maze.h"

#if MAZE
#include "rtcMem.h"

#define MAZE_SCREEN_W 200
#define MAZE_SCREEN_H 200
#define MAZE_CELLS 10
#define MAZE_CELL_SIZE (MAZE_SCREEN_W / MAZE_CELLS)
#define MAZE_WALL_THICK 2
#define MAZE_PLAYER_RADIUS 2
#define MAZE_STATUS_H 10
#define MAZE_PLAY_TOP MAZE_STATUS_H
#define MAZE_PLAY_H (MAZE_SCREEN_H - MAZE_STATUS_H)
#define MAZE_PLAY_CELL_SIZE (MAZE_PLAY_H / MAZE_CELLS)

#define MAZE_WALL_N 0x1
#define MAZE_WALL_E 0x2
#define MAZE_WALL_S 0x4
#define MAZE_WALL_W 0x8

#define MAZE_SLEEP_DIFF 13

uint8_t **mazeGrid = nullptr;
bool **mazeVisited = nullptr;

int mazePlayerX = 0;
int mazePlayerY = 0;
int mazeLastPlayerX = 0;
int mazeLastPlayerY = 0;
bool mazeWon = false;
bool mazeFailed = false;
uint32_t mazeStartMs = 0;
uint32_t mazeFinishMs = 0;

int mazeVeryLastX = 0;
int mazeVeryLastY = 0;
uint8_t mazeLoopCount = 0;

int mazeCellPxX(int cx)
{
    return cx * MAZE_PLAY_CELL_SIZE;
}

int mazeCellPxY(int cy)
{
    return MAZE_PLAY_TOP + cy * MAZE_PLAY_CELL_SIZE;
}

void mazeCarve(int cx, int cy)
{
    mazeVisited[cx][cy] = true;

    int order[4] = {0, 1, 2, 3};
    for (int i = 3; i > 0; i--)
    {
        int j = betterRandom(0, i + 1);
        int tmp = order[i];
        order[i] = order[j];
        order[j] = tmp;
    }

    for (int i = 0; i < 4; i++)
    {
        int dir = order[i];
        int nx = cx;
        int ny = cy;
        uint8_t wallHere = 0;
        uint8_t wallThere = 0;

        if (dir == 0)
        {
            ny -= 1;
            wallHere = MAZE_WALL_N;
            wallThere = MAZE_WALL_S;
        }
        else if (dir == 1)
        {
            nx += 1;
            wallHere = MAZE_WALL_E;
            wallThere = MAZE_WALL_W;
        }
        else if (dir == 2)
        {
            ny += 1;
            wallHere = MAZE_WALL_S;
            wallThere = MAZE_WALL_N;
        }
        else
        {
            nx -= 1;
            wallHere = MAZE_WALL_W;
            wallThere = MAZE_WALL_E;
        }

        if (nx < 0 || nx >= MAZE_CELLS || ny < 0 || ny >= MAZE_CELLS)
        {
            continue;
        }
        if (mazeVisited[nx][ny] == true)
        {
            continue;
        }

        mazeGrid[cx][cy] &= ~wallHere;
        mazeGrid[nx][ny] &= ~wallThere;
        mazeCarve(nx, ny);
    }
}

void mazeGenerate()
{
    for (int x = 0; x < MAZE_CELLS; x++)
    {
        for (int y = 0; y < MAZE_CELLS; y++)
        {
            mazeGrid[x][y] = MAZE_WALL_N | MAZE_WALL_E | MAZE_WALL_S | MAZE_WALL_W;
            mazeVisited[x][y] = false;
        }
    }
    mazeCarve(0, 0);
}

void mazeDrawStatus()
{
    dis->fillRect(0, 0, MAZE_SCREEN_W, MAZE_STATUS_H, SCWhite);
    dis->setCursor(1, 8);
    setFont(getFont("dogicapixel4"));
    dis->setTextColor(SCBlack);
    if (mazeWon == true)
    {
        uint32_t sec = (mazeFinishMs - mazeStartMs) / 1000;
        dis->print("Won! ");
        dis->print(String(sec).c_str());
        dis->print("s - Click to restart");
    }
    else if (mazeFailed == true)
    {
        dis->print("Failed! - Click to restart");
    }
    else
    {
        dis->print("MAZE - Reach top right");
    }
    resetSleepDelay(SLEEP_EVERY_MS);
}

void mazeDrawWalls()
{
    int mazeW = MAZE_CELLS * MAZE_PLAY_CELL_SIZE;
    int mazeBottom = MAZE_PLAY_TOP + MAZE_CELLS * MAZE_PLAY_CELL_SIZE;

    for (int cx = 0; cx < MAZE_CELLS; cx++)
    {
        for (int cy = 0; cy < MAZE_CELLS; cy++)
        {
            int px = mazeCellPxX(cx);
            int py = mazeCellPxY(cy);
            uint8_t w = mazeGrid[cx][cy];

            if (w & MAZE_WALL_N)
            {
                int x1 = px - MAZE_WALL_THICK;
                int x2 = px + MAZE_PLAY_CELL_SIZE + MAZE_WALL_THICK;
                if (x1 < 0)
                    x1 = 0;
                if (x2 > mazeW)
                    x2 = mazeW;
                dis->fillRect(x1, py, x2 - x1, MAZE_WALL_THICK, SCBlack);
            }
            if (w & MAZE_WALL_W)
            {
                int y1 = py - MAZE_WALL_THICK;
                int y2 = py + MAZE_PLAY_CELL_SIZE + MAZE_WALL_THICK;
                if (y1 < MAZE_PLAY_TOP)
                    y1 = MAZE_PLAY_TOP;
                if (y2 > mazeBottom)
                    y2 = mazeBottom;
                dis->fillRect(px, y1, MAZE_WALL_THICK, y2 - y1, SCBlack);
            }
            if (w & MAZE_WALL_S)
            {
                int x1 = px - MAZE_WALL_THICK;
                int x2 = px + MAZE_PLAY_CELL_SIZE + MAZE_WALL_THICK;
                if (x1 < 0)
                    x1 = 0;
                if (x2 > mazeW)
                    x2 = mazeW;
                dis->fillRect(x1, py + MAZE_PLAY_CELL_SIZE - MAZE_WALL_THICK, x2 - x1, MAZE_WALL_THICK, SCBlack);
            }
            if (w & MAZE_WALL_E)
            {
                int y1 = py - MAZE_WALL_THICK;
                int y2 = py + MAZE_PLAY_CELL_SIZE + MAZE_WALL_THICK;
                if (y1 < MAZE_PLAY_TOP)
                    y1 = MAZE_PLAY_TOP;
                if (y2 > mazeBottom)
                    y2 = mazeBottom;
                dis->fillRect(px + MAZE_PLAY_CELL_SIZE - MAZE_WALL_THICK, y1, MAZE_WALL_THICK, y2 - y1, SCBlack);
            }
        }
    }
}

void mazeDrawGoal()
{
    int gx = mazeCellPxX(MAZE_CELLS - 1) + MAZE_WALL_THICK;
    int gy = mazeCellPxY(0) + MAZE_WALL_THICK;
    int sz = MAZE_PLAY_CELL_SIZE - 2 * MAZE_WALL_THICK;
    for (int i = 0; i < sz; i += 3)
    {
        dis->drawLine(gx + i, gy, gx, gy + i, SCBlack);
        dis->drawLine(gx + sz, gy + i, gx + i, gy + sz, SCBlack);
    }
}

int mazeClampX(int x)
{
    if (x < MAZE_WALL_THICK + MAZE_PLAYER_RADIUS)
        return MAZE_WALL_THICK + MAZE_PLAYER_RADIUS;
    int maxX = MAZE_CELLS * MAZE_PLAY_CELL_SIZE - MAZE_WALL_THICK - MAZE_PLAYER_RADIUS - 1;
    if (x > maxX)
        return maxX;
    return x;
}

int mazeClampY(int y)
{
    if (y < MAZE_PLAY_TOP + MAZE_WALL_THICK + MAZE_PLAYER_RADIUS)
        return MAZE_PLAY_TOP + MAZE_WALL_THICK + MAZE_PLAYER_RADIUS;
    int maxY = MAZE_PLAY_TOP + MAZE_CELLS * MAZE_PLAY_CELL_SIZE - MAZE_WALL_THICK - MAZE_PLAYER_RADIUS - 1;
    if (y > maxY)
        return maxY;
    return y;
}

bool mazeBlockedX(int fromX, int toX, int y)
{
    if (toX == fromX)
    {
        return false;
    }
    int cy = (y - MAZE_PLAY_TOP) / MAZE_PLAY_CELL_SIZE;
    if (cy < 0 || cy >= MAZE_CELLS)
    {
        return true;
    }

    if (toX > fromX)
    {
        int cxFrom = (fromX + MAZE_PLAYER_RADIUS) / MAZE_PLAY_CELL_SIZE;
        int cxTo = (toX + MAZE_PLAYER_RADIUS) / MAZE_PLAY_CELL_SIZE;
        for (int cx = cxFrom; cx < cxTo; cx++)
        {
            if (cx < 0 || cx >= MAZE_CELLS)
            {
                return true;
            }
            if (mazeGrid[cx][cy] & MAZE_WALL_E)
            {
                return true;
            }
        }
    }
    else
    {
        int cxFrom = (fromX - MAZE_PLAYER_RADIUS) / MAZE_PLAY_CELL_SIZE;
        int cxTo = (toX - MAZE_PLAYER_RADIUS) / MAZE_PLAY_CELL_SIZE;
        for (int cx = cxFrom; cx > cxTo; cx--)
        {
            if (cx < 0 || cx >= MAZE_CELLS)
            {
                return true;
            }
            if (mazeGrid[cx][cy] & MAZE_WALL_W)
            {
                return true;
            }
        }
    }
    return false;
}

bool mazeBlockedY(int fromY, int toY, int x)
{
    if (toY == fromY)
    {
        return false;
    }
    int cx = x / MAZE_PLAY_CELL_SIZE;
    if (cx < 0 || cx >= MAZE_CELLS)
    {
        return true;
    }

    if (toY > fromY)
    {
        int cyFrom = (fromY - MAZE_PLAY_TOP + MAZE_PLAYER_RADIUS) / MAZE_PLAY_CELL_SIZE;
        int cyTo = (toY - MAZE_PLAY_TOP + MAZE_PLAYER_RADIUS) / MAZE_PLAY_CELL_SIZE;
        for (int cy = cyFrom; cy < cyTo; cy++)
        {
            if (cy < 0 || cy >= MAZE_CELLS)
            {
                return true;
            }
            if (mazeGrid[cx][cy] & MAZE_WALL_S)
            {
                return true;
            }
        }
    }
    else
    {
        int cyFrom = (fromY - MAZE_PLAY_TOP - MAZE_PLAYER_RADIUS) / MAZE_PLAY_CELL_SIZE;
        int cyTo = (toY - MAZE_PLAY_TOP - MAZE_PLAYER_RADIUS) / MAZE_PLAY_CELL_SIZE;
        for (int cy = cyFrom; cy > cyTo; cy--)
        {
            if (cy < 0 || cy >= MAZE_CELLS)
            {
                return true;
            }
            if (mazeGrid[cx][cy] & MAZE_WALL_N)
            {
                return true;
            }
        }
    }
    return false;
}

void mazeRedrawAll()
{
    dis->fillScreen(SCWhite);
    mazeDrawWalls();
    mazeDrawStatus();
    mazeDrawGoal();
    dis->fillCircle(mazePlayerX, mazePlayerY, MAZE_PLAYER_RADIUS, SCBlack);
}

void initMaze()
{
    mazeGrid = (uint8_t **)malloc(MAZE_CELLS * sizeof(uint8_t *));
    mazeVisited = (bool **)malloc(MAZE_CELLS * sizeof(bool *));

    for (int i = 0; i < MAZE_CELLS; i++) {
        mazeGrid[i] = (uint8_t *)malloc(MAZE_CELLS * sizeof(uint8_t));
        mazeVisited[i] = (bool *)malloc(MAZE_CELLS * sizeof(bool));
    }

    mazeGenerate();

    mazePlayerX = mazeCellPxX(0) + MAZE_PLAY_CELL_SIZE / 2;
    mazePlayerY = mazeCellPxY(MAZE_CELLS - 1) + MAZE_PLAY_CELL_SIZE / 2;
    mazeLastPlayerX = mazePlayerX;
    mazeLastPlayerY = mazePlayerY;

    mazeWon = false;
    mazeFailed = false;
    mazeStartMs = millisBetter();
    mazeFinishMs = 0;
    mazeVeryLastX = mazePlayerX;
    mazeVeryLastY = mazePlayerY;
    mazeLoopCount = 0;

    initAcc();
    mazeRedrawAll();
    disUp(true);
}

void loopMaze()
{
    buttonState btn = useButton();

    if (mazeWon == true || mazeFailed == true)
    {
        if (btn != None)
        {
            initMaze();
            return;
        }
    }

    if (btn == Menu)
    {
        initMaze();
        return;
    }

    Accel acc;
    if (rM.SBMA.getAccel(&acc) && (mazeWon == false && mazeFailed == false))
    {
        float degX = getAxisDegrees(acc.x, acc.y, acc.z);
        float degY = getAxisDegrees(acc.y, acc.x, acc.z);

        int moveX = (int)(degX / 4.0);
        int moveY = (int)(degY / 4.0);

        int newX = mazeClampX(mazePlayerX + moveX);
        int newY = mazeClampY(mazePlayerY + moveY);

        if (moveX != 0)
        {
            if (mazeBlockedX(mazePlayerX, newX, mazePlayerY) == true)
            {
                mazeFailed = true;
            }
            else
            {
                mazePlayerX = newX;
            }
        }

        if (!mazeFailed && moveY != 0)
        {
            if (mazeBlockedY(mazePlayerY, newY, mazePlayerX) == true)
            {
                mazeFailed = true;
            }
            else
            {
                mazePlayerY = newY;
            }
        }

        if (mazeFailed)
        {
            mazeDrawStatus();
            dUChange = true;
        }

        if (mazePlayerX != mazeLastPlayerX || mazePlayerY != mazeLastPlayerY)
        {
            dis->fillCircle(mazeLastPlayerX, mazeLastPlayerY, MAZE_PLAYER_RADIUS, SCWhite);
            mazeDrawWalls();
            mazeDrawGoal();
            dis->fillCircle(mazePlayerX, mazePlayerY, MAZE_PLAYER_RADIUS, SCBlack);
            mazeLastPlayerX = mazePlayerX;
            mazeLastPlayerY = mazePlayerY;
            dUChange = true;
        }

        int goalCx = (mazePlayerX) / MAZE_PLAY_CELL_SIZE;
        int goalCy = (mazePlayerY - MAZE_PLAY_TOP) / MAZE_PLAY_CELL_SIZE;
        if (goalCx == MAZE_CELLS - 1 && goalCy == 0)
        {
            mazeWon = true;
            mazeFinishMs = millisBetter();
            mazeDrawStatus();
            dUChange = true;
        }

        mazeLoopCount++;
        if (mazeLoopCount % 5 == 0)
        {
            if (abs(mazeVeryLastX - mazePlayerX) > MAZE_SLEEP_DIFF || abs(mazeVeryLastY - mazePlayerY) > MAZE_SLEEP_DIFF)
            {
                resetSleepDelay(SLEEP_EVERY_MS);
                mazeVeryLastX = mazePlayerX;
                mazeVeryLastY = mazePlayerY;
            }
            mazeLoopCount = 0;
        }
    }

    disUp();
}

void exitMaze()
{
    for (int i = 0; i < MAZE_CELLS; i++) {
        free(mazeGrid[i]);
    }
    free(mazeGrid);

    for (int i = 0; i < MAZE_CELLS; i++) {
        free(mazeVisited[i]);
    }
    free(mazeVisited);
    mazeGrid = nullptr;
    mazeVisited = nullptr;
}

#endif
