#include "eightball.h"

#if EIGHTBALL
#include "rtcMem.h"

#define EB_TABLE_LEFT 10
#define EB_TABLE_RIGHT 190
#define EB_TABLE_TOP 30
#define EB_TABLE_BOTTOM 188

#define EB_BALL_RADIUS 10
#define EB_BALLS 6

#define EB_POCKET_DRAW_R 13
#define EB_POCKET_R 15     // Capture: ball center to pocket center
#define EB_POCKET_MOUTH 18 // No-cushion (so no bounce) zone around a pocket
#define EB_POCKETS 6

#define EB_FRICTION 0.98f
#define EB_CUSHION_BOUNCE 0.8f
#define EB_STOP_SPEED 0.2f
#define EB_MAX_SPEED 9.0f

// Animation stuff
#define EB_TICKS_START 4
#define EB_TICKS_INC 2
#define EB_TICKS_MAX 60

#define EB_TILT_MAX_DEG 20.0f
#define EB_TILT_MIN_DEG 4.0f // Line hidden
#define EB_AIM_LINE_MIN 12
#define EB_AIM_LINE_MAX 56
#define EB_AIM_QUANT_DEG 3 // So it doesn't write itself constantly

// Center of the balls pyramid
#define EB_RACK_X 100
#define EB_RACK_Y 109
#define EB_RACK_SPACING 22

#define EB_FONT getFont("eightball/UbuntuMono10")

struct ebBall
{
    float x;
    float y;
    float vx;
    float vy;
    bool inPocket;
};

enum ebState
{
    EB_SELECT,
    EB_AIM,
    EB_MOVE,
    EB_CLEAR
};

static ebBall *ebBalls = nullptr;
static int *ebPocketX = nullptr;
static int *ebPocketY = nullptr;
static int ebSelected = 0;
static int ebPotted = 0;
static int ebPushes = 0;
static int ebMoveFrames = 0; // Frames roll started
static ebState ebSt = EB_SELECT;
static float ebAimAngle = 0;
static float ebAimForce = 0;
static int ebLastAimAngle = -9999;
static int ebLastAimForce = -1;

static void ebRack()
{
    const int ox[EB_BALLS] = {
        -EB_RACK_SPACING,
        0, 0,
        EB_RACK_SPACING, EB_RACK_SPACING, EB_RACK_SPACING};
    const int oy[EB_BALLS] = {
        0,
        -EB_RACK_SPACING / 2, EB_RACK_SPACING / 2,
        -EB_RACK_SPACING, 0, EB_RACK_SPACING};
    for (int i = 0; i < EB_BALLS; i++)
    {
        ebBalls[i].x = EB_RACK_X + ox[i];
        ebBalls[i].y = EB_RACK_Y + oy[i];
        ebBalls[i].vx = 0;
        ebBalls[i].vy = 0;
        ebBalls[i].inPocket = false;
    }
    ebPotted = 0;
    ebPushes = 0;
    ebSelected = 0;
}

static String ebStatusText()
{
    if (ebSt == EB_SELECT)
    {
        return "Pot " + String(ebPotted) + "/" + String(EB_BALLS) + " - Pick ball";
    }
    if (ebSt == EB_AIM)
    {
        return "Tilt aim, Menu push";
    }
    if (ebSt == EB_MOVE)
    {
        return "Rolling...";
    }
    return "Pot " + String(EB_BALLS) + "/" + String(EB_BALLS);
}

static void ebDrawStatus(String text)
{
    dis->fillRect(0, 0, 200, EB_TABLE_TOP - 4, SCWhite);
    int16_t x, y;
    uint16_t w, h;
    getTextBounds(text, &x, &y, &w, &h, 0, 0);
    dis->setTextColor(SCBlack, SCWhite);
    dis->setCursor((200 - w) / 2 - x, (EB_TABLE_TOP - 4 - h) / 2 - y);
    dis->print(text);
}

static void ebDrawTable()
{
    // Pockets, drawn first so the parts outside the table will be removed
    for (int p = 0; p < EB_POCKETS; p++)
    {
        dis->fillCircle(ebPocketX[p], ebPocketY[p], EB_POCKET_DRAW_R, SCBlack);
    }
    // Clip the pockets to the table interior
    dis->fillRect(0, 0, EB_TABLE_LEFT + 2, 200, SCWhite);
    dis->fillRect(EB_TABLE_RIGHT - 1, 0, 200, 200, SCWhite);
    dis->fillRect(0, 0, 200, EB_TABLE_TOP + 2, SCWhite);
    dis->fillRect(0, EB_TABLE_BOTTOM - 1, 200, 200 - EB_TABLE_BOTTOM + 1, SCWhite);
    // Table frame
    dis->drawRect(EB_TABLE_LEFT, EB_TABLE_TOP, EB_TABLE_RIGHT - EB_TABLE_LEFT, EB_TABLE_BOTTOM - EB_TABLE_TOP, SCBlack);
    dis->drawRect(EB_TABLE_LEFT + 1, EB_TABLE_TOP + 1, EB_TABLE_RIGHT - EB_TABLE_LEFT - 2, EB_TABLE_BOTTOM - EB_TABLE_TOP - 2, SCBlack);
}

static void ebDrawBall(int i, bool selected)
{
    ebBall *b = &ebBalls[i];
    if (b->inPocket)
    {
        return;
    }
    int x = (int)b->x;
    int y = (int)b->y;
    if (selected)
    {
        dis->fillCircle(x, y, EB_BALL_RADIUS + 4, SCWhite);
        dis->drawCircle(x, y, EB_BALL_RADIUS + 3, SCBlack);
        dis->drawCircle(x, y, EB_BALL_RADIUS + 2, SCBlack);
    }
    dis->fillCircle(x, y, EB_BALL_RADIUS, SCBlack);
    dis->fillCircle(x, y, EB_BALL_RADIUS / 2, SCWhite);
}

static void ebDrawAimLine()
{
    ebBall *b = &ebBalls[ebSelected];
    if (b->inPocket || ebAimForce <= 0)
    {
        return;
    }
    int len = EB_AIM_LINE_MIN + (int)(ebAimForce * (EB_AIM_LINE_MAX - EB_AIM_LINE_MIN));
    float dx = cosf(ebAimAngle);
    float dy = sinf(ebAimAngle);
    int startOff = EB_BALL_RADIUS + 4;
    int x = (int)b->x;
    int y = (int)b->y;

    for (int o = -2; o <= 2; o++)
    {
        dis->drawLine(x + (int)(dx * startOff) + (int)(-dy * o), y + (int)(dy * startOff) + (int)(dx * o),
                      x + (int)(dx * (startOff + len)) + (int)(-dy * o), y + (int)(dy * (startOff + len)) + (int)(dx * o),
                      SCWhite);
    }
    for (int o = -1; o <= 1; o++)
    {
        dis->drawLine(x + (int)(dx * startOff) + (int)(-dy * o), y + (int)(dy * startOff) + (int)(dx * o),
                      x + (int)(dx * (startOff + len)) + (int)(-dy * o), y + (int)(dy * (startOff + len)) + (int)(dx * o),
                      SCBlack);
    }
}

static void ebDrawAll()
{
    dis->fillScreen(SCWhite);
    ebDrawTable();
    ebDrawStatus(ebStatusText());
    for (int i = 0; i < EB_BALLS; i++)
    {
        ebDrawBall(i, i == ebSelected && (ebSt == EB_SELECT || ebSt == EB_AIM));
    }
    if (ebSt == EB_AIM)
    {
        ebDrawAimLine();
    }
}

static void ebDrawClear()
{
    dis->fillScreen(SCWhite);
    ebDrawTable();
    ebDrawStatus(ebStatusText());
    int16_t x, y;
    uint16_t w, h;
    dis->setTextColor(SCBlack, SCWhite);
    String s1 = "Table clear!";
    getTextBounds(s1, &x, &y, &w, &h, 0, 0);
    dis->setCursor((200 - w) / 2 - x, 55 + (40 - h) / 2 - y);
    dis->print(s1);
    String s1b = "In " + String(ebPushes) + " pushes";
    getTextBounds(s1b, &x, &y, &w, &h, 0, 0);
    dis->setCursor((200 - w) / 2 - x, 90 + (40 - h) / 2 - y);
    dis->print(s1b);
    String s2 = "Tap for new rack";
    getTextBounds(s2, &x, &y, &w, &h, 0, 0);
    dis->setCursor((200 - w) / 2 - x, 125 + (40 - h) / 2 - y);
    dis->print(s2);
}

static void ebPot(ebBall *b)
{
    b->inPocket = true;
    b->vx = 0;
    b->vy = 0;
    b->x = 0;
    b->y = 0;
    ebPotted++;
}

static bool ebAnyMoving()
{
    for (int i = 0; i < EB_BALLS; i++)
    {
        ebBall *b = &ebBalls[i];
        if (b->inPocket)
        {
            continue;
        }
        if (b->vx != 0 || b->vy != 0)
        {
            return true;
        }
    }
    return false;
}

static void ebPhysicsTick()
{
    for (int i = 0; i < EB_BALLS; i++)
    {
        ebBall *b = &ebBalls[i];
        if (b->inPocket)
        {
            continue;
        }
        b->x += b->vx;
        b->y += b->vy;
        b->vx *= EB_FRICTION;
        b->vy *= EB_FRICTION;
        if (b->vx * b->vx + b->vy * b->vy < EB_STOP_SPEED * EB_STOP_SPEED)
        {
            b->vx = 0;
            b->vy = 0;
        }
    }

    // Ball to ball collisions
    for (int i = 0; i < EB_BALLS; i++)
    {
        for (int j = i + 1; j < EB_BALLS; j++)
        {
            ebBall *a = &ebBalls[i];
            ebBall *b = &ebBalls[j];
            if (a->inPocket || b->inPocket)
            {
                continue;
            }
            float dx = b->x - a->x;
            float dy = b->y - a->y;
            float d2 = dx * dx + dy * dy;
            float minD = 2.0f * EB_BALL_RADIUS;
            if (d2 >= minD * minD || d2 < 0.0001f)
            {
                continue;
            }
            float d = sqrtf(d2);
            float nx = dx / d;
            float ny = dy / d;
            // Separate the overlap
            float overlap = (minD - d) / 2.0f;
            a->x -= nx * overlap;
            a->y -= ny * overlap;
            b->x += nx * overlap;
            b->y += ny * overlap;
            // Elastic collision, equal masses (swap velocity along the normal)
            float va = a->vx * nx + a->vy * ny;
            float vb = b->vx * nx + b->vy * ny;
            if (va - vb > 0) // Only if they are moving towards each other
            {
                a->vx += (vb - va) * nx;
                a->vy += (vb - va) * ny;
                b->vx += (va - vb) * nx;
                b->vy += (va - vb) * ny;
            }
        }
    }

    // Pockets and cushions
    for (int i = 0; i < EB_BALLS; i++)
    {
        ebBall *b = &ebBalls[i];
        if (b->inPocket)
        {
            continue;
        }

        // Pocket capture, and remember if the ball is in a pocket mouth
        bool nearPocket = false;
        for (int p = 0; p < EB_POCKETS; p++)
        {
            float dx = b->x - ebPocketX[p];
            float dy = b->y - ebPocketY[p];
            float d2 = dx * dx + dy * dy;
            if (d2 < EB_POCKET_R * EB_POCKET_R)
            {
                ebPot(b);
                break;
            }
            if (d2 < EB_POCKET_MOUTH * EB_POCKET_MOUTH)
            {
                nearPocket = true;
            }
        }
        if (b->inPocket)
        {
            continue;
        }

        bool pastLeft = b->x < EB_TABLE_LEFT + EB_BALL_RADIUS;
        bool pastRight = b->x > EB_TABLE_RIGHT - EB_BALL_RADIUS;
        bool pastTop = b->y < EB_TABLE_TOP + EB_BALL_RADIUS;
        bool pastBottom = b->y > EB_TABLE_BOTTOM - EB_BALL_RADIUS;

        if (pastLeft || pastRight || pastTop || pastBottom)
        {
            // A ball can only cross the cushion line inside a pocket mouth
            if (nearPocket)
            {
                // It went in the hole
                ebPot(b);
            }
            else
            {
                // Safety net, push it back onto the table
                if (pastLeft)
                {
                    b->x = EB_TABLE_LEFT + EB_BALL_RADIUS;
                    b->vx = fabsf(b->vx) * EB_CUSHION_BOUNCE;
                }
                if (pastRight)
                {
                    b->x = EB_TABLE_RIGHT - EB_BALL_RADIUS;
                    b->vx = -fabsf(b->vx) * EB_CUSHION_BOUNCE;
                }
                if (pastTop)
                {
                    b->y = EB_TABLE_TOP + EB_BALL_RADIUS;
                    b->vy = fabsf(b->vy) * EB_CUSHION_BOUNCE;
                }
                if (pastBottom)
                {
                    b->y = EB_TABLE_BOTTOM - EB_BALL_RADIUS;
                    b->vy = -fabsf(b->vy) * EB_CUSHION_BOUNCE;
                }
            }
            continue;
        }

        // On the table, cushion reflection, skipped inside a pocket mouth
        if (nearPocket)
        {
            continue;
        }
        if (b->x <= EB_TABLE_LEFT + EB_BALL_RADIUS)
        {
            b->x = EB_TABLE_LEFT + EB_BALL_RADIUS;
            b->vx = -b->vx * EB_CUSHION_BOUNCE;
        }
        else if (b->x >= EB_TABLE_RIGHT - EB_BALL_RADIUS)
        {
            b->x = EB_TABLE_RIGHT - EB_BALL_RADIUS;
            b->vx = -b->vx * EB_CUSHION_BOUNCE;
        }
        if (b->y <= EB_TABLE_TOP + EB_BALL_RADIUS)
        {
            b->y = EB_TABLE_TOP + EB_BALL_RADIUS;
            b->vy = -b->vy * EB_CUSHION_BOUNCE;
        }
        else if (b->y >= EB_TABLE_BOTTOM - EB_BALL_RADIUS)
        {
            b->y = EB_TABLE_BOTTOM - EB_BALL_RADIUS;
            b->vy = -b->vy * EB_CUSHION_BOUNCE;
        }
    }
}

// Ticks per frame grow from EB_TICKS_START by EB_TICKS_INC per frame, capped at EB_TICKS_MAX
static void ebStepMove()
{
    int ticks = EB_TICKS_START + ebMoveFrames * EB_TICKS_INC;
    if (ticks > EB_TICKS_MAX)
    {
        ticks = EB_TICKS_MAX;
    }
    ebMoveFrames++;
    for (int t = 0; t < ticks; t++)
    {
        ebPhysicsTick();
    }
}

static void ebMoveEnd()
{
    for (int i = 0; i < EB_BALLS; i++)
    {
        ebBalls[i].vx = 0;
        ebBalls[i].vy = 0;
    }
    if (ebPotted >= EB_BALLS)
    {
        ebSt = EB_CLEAR;
        ebDrawClear();
    }
    else
    {
        ebSt = EB_SELECT;
        for (int i = 0; i < EB_BALLS; i++)
        {
            if (ebBalls[i].inPocket == false)
            {
                ebSelected = i;
                break;
            }
        }
        ebDrawAll();
    }
    dUChange = true;
}

static void ebCycleSelection(int dir)
{
    int start = ebSelected;
    for (int i = 0; i < EB_BALLS; i++)
    {
        ebSelected = (ebSelected + dir + EB_BALLS) % EB_BALLS;
        if (ebBalls[ebSelected].inPocket == false && ebSelected != start)
        {
            break;
        }
    }
}

static void ebAimUpdate()
{
    Accel acc;
    if (rM.SBMA.getAccel(&acc) == false)
    {
        return;
    }
    float degX = getAxisDegrees(acc.x, acc.y, acc.z);
    float degY = getAxisDegrees(acc.y, acc.x, acc.z);
    float mag = sqrtf(degX * degX + degY * degY);

    ebAimAngle = atan2f(-degY, -degX);
    ebAimForce = 0;
    if (mag >= EB_TILT_MIN_DEG)
    {
        ebAimForce = mag / EB_TILT_MAX_DEG;
        if (ebAimForce > 1.0f)
        {
            ebAimForce = 1.0f;
        }
    }

    int qAngle = (int)(ebAimAngle * 57.29577f / EB_AIM_QUANT_DEG) * EB_AIM_QUANT_DEG;
    int qForce = (int)(ebAimForce * 100.0f / 5.0f + 0.5f) * 5;
    if (qAngle != ebLastAimAngle || qForce != ebLastAimForce)
    {
        ebLastAimAngle = qAngle;
        ebLastAimForce = qForce;
        ebDrawAll();
        dUChange = true;
    }
}

void initEightball()
{
    free(ebBalls);
    free(ebPocketX);
    free(ebPocketY);
    ebBalls = (ebBall *)malloc(sizeof(ebBall) * EB_BALLS);
    ebPocketX = (int *)malloc(sizeof(int) * EB_POCKETS);
    ebPocketY = (int *)malloc(sizeof(int) * EB_POCKETS);
    ebPocketX[0] = EB_TABLE_LEFT;
    ebPocketX[1] = EB_TABLE_RIGHT;
    ebPocketX[2] = EB_TABLE_LEFT;
    ebPocketX[3] = EB_TABLE_RIGHT;
    ebPocketX[4] = (EB_TABLE_LEFT + EB_TABLE_RIGHT) / 2;
    ebPocketX[5] = (EB_TABLE_LEFT + EB_TABLE_RIGHT) / 2;
    ebPocketY[0] = EB_TABLE_TOP;
    ebPocketY[1] = EB_TABLE_TOP;
    ebPocketY[2] = EB_TABLE_BOTTOM;
    ebPocketY[3] = EB_TABLE_BOTTOM;
    ebPocketY[4] = EB_TABLE_TOP;
    ebPocketY[5] = EB_TABLE_BOTTOM;

    setFont(EB_FONT);
    setTextSize(1);
    dis->setTextColor(SCBlack, SCWhite);
    dis->setTextWrap(false);
    initAcc();
    ebRack();
    ebSt = EB_SELECT;
    ebLastAimAngle = -9999;
    ebLastAimForce = -1;
    ebDrawAll();
    disUp(true);
}

void loopEightball()
{
    resetSleepDelay();
    buttonState btn = useButton();

    switch (ebSt)
    {
    case EB_SELECT:
    {
        if (btn == Up || btn == LongUp)
        {
            ebCycleSelection(1);
            ebDrawAll();
            dUChange = true;
        }
        else if (btn == Down || btn == LongDown)
        {
            ebCycleSelection(-1);
            ebDrawAll();
            dUChange = true;
        }
        else if (btn == Menu)
        {
            ebSt = EB_AIM;
            ebAimForce = 0;
            ebLastAimAngle = -9999;
            ebLastAimForce = -1;
            ebDrawAll();
            dUChange = true;
        }
        else if (btn == LongMenu)
        {
            ebRack();
            ebDrawAll();
            dUChange = true;
        }
        break;
    }
    case EB_AIM:
    {
        if (btn == Menu && ebAimForce > 0)
        {
            float speed = ebAimForce * EB_MAX_SPEED;

            ebBalls[ebSelected].vx = -cosf(ebAimAngle) * speed;
            ebBalls[ebSelected].vy = -sinf(ebAimAngle) * speed;
            ebPushes++;
            ebMoveFrames = 0;
            ebSt = EB_MOVE;
        }
        else if (btn == LongMenu)
        {
            ebRack();
            ebSt = EB_SELECT;
            ebDrawAll();
            dUChange = true;
        }
        else
        {
            ebAimUpdate();
        }
        break;
    }
    case EB_MOVE:
    {
        ebStepMove();
        if (ebAnyMoving() == false)
        {
            ebMoveEnd();
        }
        else
        {
            ebDrawAll();
            // So it can't flicker a full refresh mid-roll
            disUp(true, true);
        }
        return;
    }
    case EB_CLEAR:
    {
        if (btn != None)
        {
            initEightball();
            return;
        }
        break;
    }
    }
    disUp();
}

void exitEightball()
{
    free(ebBalls);
    free(ebPocketX);
    free(ebPocketY);
    ebBalls = nullptr;
    ebPocketX = nullptr;
    ebPocketY = nullptr;
}

#endif
