#include "jumper.h"

#if JUMPER

#define SCREEN_W 200
#define SCREEN_H 200
#define JUMPER_SIZE 15
#define OBSTACLE_SIZE 5
#define STEP_X 20
#define STEP_Y 20
#define NUM_OBSTACLES 10
#define MINIMUM_OBSTACLES 3

struct xy
{
    int x;
    int y;
};

struct obstacle
{
    int x;
    int y;
    int speed;
};

xy jumper;
obstacle obstacles[NUM_OBSTACLES];
bool lostJumper = false;
int jumperPoints;

void drawJumper()
{
    dis->fillRect(jumper.x, jumper.y, JUMPER_SIZE, JUMPER_SIZE, SCBlack);
}

void drawObstacles()
{
    for (int i = 0; i < NUM_OBSTACLES; i++)
    {
        if (obstacles[i].y >= 0) // only draw if lane exists
        {
            dis->fillRect(obstacles[i].x, obstacles[i].y, STEP_X, OBSTACLE_SIZE, SCBlack);
        }
    }
}

bool checkCollision()
{
    for (int i = 0; i < NUM_OBSTACLES; i++)
    {
        if (obstacles[i].y >= 0)
        {
            if (jumper.x < obstacles[i].x + STEP_X &&
                jumper.x + JUMPER_SIZE > obstacles[i].x &&
                jumper.y < obstacles[i].y + OBSTACLE_SIZE &&
                jumper.y + JUMPER_SIZE > obstacles[i].y)
            {
                return true;
            }
        }
    }
    return false;
}

void moveObstacles()
{
    for (int i = 0; i < NUM_OBSTACLES; i++)
    {
        if (obstacles[i].y < 0)
            continue; // skip disabled lanes

        obstacles[i].x += obstacles[i].speed;
        if (obstacles[i].x < 0)
            obstacles[i].x = SCREEN_W - STEP_X;
        if (obstacles[i].x > SCREEN_W - STEP_X)
            obstacles[i].x = 0;
    }
}

void initJumper()
{
    jumper.x = (SCREEN_W - JUMPER_SIZE) / 2;
    jumper.y = SCREEN_H - STEP_Y;

    int laneHeight = STEP_Y;                // spacing between lanes
    int maxLanes = (SCREEN_H / laneHeight) - 1; // number of possible lanes
    debugLog("Max lanes: " + String(maxLanes));
    int c = 0;

    for (int i = 0; i < NUM_OBSTACLES; i++)
    {
        bool hasCar = (betterRandom(0, 100) < 80);
        hasCar = true;
        if (hasCar && i < maxLanes)
        {
            c++;
            obstacles[i].x = betterRandom(0, SCREEN_W - STEP_X);
            obstacles[i].y = i * laneHeight;

            int dir = (betterRandom(0, 2) == 0) ? -1 : 1;
            obstacles[i].speed = dir * (betterRandom(1, 7) * (STEP_X / 3));
        }
        else
        {
            obstacles[i].y = -1; // empty lane
        }
    }

    if (c < MINIMUM_OBSTACLES)
    {
        initJumper();
        return;
    }

    jumperPoints = 0;
    lostJumper = false;
}

void loopJumper()
{
    buttonState btn = useButton();

    if (!lostJumper)
    {
        if (btn == Up)
        {
            jumper.y -= STEP_Y;
            jumperPoints++;
        }
        if (btn == Down)
        {
            jumper.y += STEP_Y;
        }
        if (btn == Menu)
        {
            jumper.y -= STEP_Y;
            jumperPoints++;
        }
        if (btn == LongUp)
        {
            jumper.x -= STEP_X;
        }
        if (btn == LongDown)
        {
            jumper.x += STEP_X;
        }
        if (btn == LongMenu)
        {
            jumper.y += STEP_Y;
        }

        if (jumper.x < 0)
            jumper.x = 0;
        if (jumper.x > SCREEN_W - JUMPER_SIZE)
            jumper.x = SCREEN_W - JUMPER_SIZE;
        if (jumper.y < 0)
        {
            jumperPoints += 10;
            initJumper();
            return;
        }
        if (jumper.y > SCREEN_H - JUMPER_SIZE)
            jumper.y = SCREEN_H - JUMPER_SIZE;

        dis->fillScreen(SCWhite);
        drawJumper();
        moveObstacles();
        drawObstacles();
        if (checkCollision())
        {
            lostJumper = true;
            String msg = "You lost! Score: " + String(jumperPoints);
            writeTextCenterReplaceBack(msg, SCREEN_H / 2);
        }
        dUChange = true;
        debugLog("Updating jumper...");
    }
    else
    {
        if (btn != None)
            initJumper();
    }

    resetSleepDelay();
    disUp();
}

#endif