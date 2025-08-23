#include "jumper.h"

#if JUMPER

#define SCREEN_W 200
#define SCREEN_H 200
#define JUMPER_SIZE 15
#define OBSTACLE_SIZE 7
#define STEP_X 20
#define STEP_Y 20
#define NUM_OBSTACLES 5

struct xy
{
    int x;
    int y;
};

xy jumper;
xy obstacles[NUM_OBSTACLES];
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
        dis->fillRect(obstacles[i].x, obstacles[i].y, STEP_X, OBSTACLE_SIZE, SCBlack);
    }
}

bool checkCollision()
{
    for (int i = 0; i < NUM_OBSTACLES; i++)
    {
        if (jumper.y == obstacles[i].y && jumper.x < obstacles[i].x + STEP_X && jumper.x + JUMPER_SIZE > obstacles[i].x)
        {
            return true;
        }
    }
    return false;
}

void moveObstacles()
{
    for (int i = 0; i < NUM_OBSTACLES; i++)
    {
        obstacles[i].x += (i % 2 == 0 ? STEP_X : -STEP_X);
        if (obstacles[i].x < 0)
            obstacles[i].x = SCREEN_W - STEP_X;
        if (obstacles[i].x > SCREEN_W - STEP_X)
            obstacles[i].x = 0;
    }
}

void initJumper()
{
    jumper.x = (SCREEN_W - JUMPER_SIZE) / 2; // centered horizontally
    jumper.y = SCREEN_H - STEP_Y;
    for (int i = 0; i < NUM_OBSTACLES; i++)
    {
        obstacles[i].x = (i * 30) % SCREEN_W;
        obstacles[i].y = (i + 1) * STEP_Y * 2;
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
