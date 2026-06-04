#include "dice.h"

#if DICE
#include "rtcMem.h"

int currentDiceType = 0;
int lastRolls[3] = {0, 0, 0};
int rollResult = 0;
bool rolling = false;
uint32_t rollStartTime = 0;
int16_t angleX = 0, angleY = 0, angleZ = 0;

void setDiceType(int type)
{
    currentDiceType = type;
    rollResult = 0;
    rolling = false;
}

void switchD2()
{
    setDiceType(0);
    switchDiceApp();
}
void switchD6()
{
    setDiceType(1);
    switchDiceApp();
}
void switchD10()
{
    setDiceType(2);
    switchDiceApp();
}
void switchD20()
{
    setDiceType(3);
    switchDiceApp();
}
void switchD100()
{
    setDiceType(4);
    switchDiceApp();
}

void initDiceMenu()
{
    int count = -1;
    entryMenu buttons[5];

    count = count + 1;
    buttons[count] = {"D2", &emptyImgPack, switchD2};
    count = count + 1;
    buttons[count] = {"D6", &emptyImgPack, switchD6};
    count = count + 1;
    buttons[count] = {"D10", &emptyImgPack, switchD10};
    count = count + 1;
    buttons[count] = {"D20", &emptyImgPack, switchD20};
    count = count + 1;
    buttons[count] = {"D100", &emptyImgPack, switchD100};

    count = count + 1;
    initMenu(buttons, count, MENU_DICE, 1);
}

struct Shape
{
    int numVertices;
    int numEdges;
    int16_t (*vertices)[3];
    uint8_t (*edges)[2];
};

void initDiceApp()
{
    rolling = false;
    rollResult = 0;
    angleX = angleY = angleZ = 0;
    lastRolls[0] = 0;
    lastRolls[1] = 0;
    lastRolls[2] = 0;
    dUChange = true;
}

#define PHI_VAL 162

void loopDiceApp()
{
    if (useButton() == Menu)
    {
        if (!rolling)
        {
            rolling = true;
            rollStartTime = millisBetter();
            int diceRanges[] = {2, 6, 10, 20, 100};
            rollResult = random(1, diceRanges[currentDiceType] + 1);
            dUChange = true;
        }
    }

    if (rolling && millisBetter() - rollStartTime > DICE_ANIMATION_TIME_MS)
    {
        rolling = false;
        lastRolls[2] = lastRolls[1];
        lastRolls[1] = lastRolls[0];
        lastRolls[0] = rollResult;
        dUChange = true;
    }

    if (rolling)
    {
        angleX = (angleX + 12) % 360;
        angleY = (angleY + 18) % 360;
        angleZ = (angleZ + 6) % 360;
        dUChange = true;
    }

    if (dUChange)
    {
        static int16_t d2Verts[16][3] = {
            {100, 0, 10}, {71, 71, 10}, {0, 100, 10}, {-71, 71, 10}, {-100, 0, 10}, {-71, -71, 10}, {0, -100, 10}, {71, -71, 10}, {100, 0, -10}, {71, 71, -10}, {0, 100, -10}, {-71, 71, -10}, {-100, 0, -10}, {-71, -71, -10}, {0, -100, -10}, {71, -71, -10}};
        static uint8_t d2Edges[24][2] = {
            {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 0}, {8, 9}, {9, 10}, {10, 11}, {11, 12}, {12, 13}, {13, 14}, {14, 15}, {15, 8}, {0, 8}, {1, 9}, {2, 10}, {3, 11}, {4, 12}, {5, 13}, {6, 14}, {7, 15}};

        static int16_t d6Verts[8][3] = {
            {-100, -100, -100}, {100, -100, -100}, {100, 100, -100}, {-100, 100, -100}, {-100, -100, 100}, {100, -100, 100}, {100, 100, 100}, {-100, 100, 100}};
        static uint8_t d6Edges[12][2] = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};

        static int16_t d10Verts[12][3] = {
            {0, 0, 120}, {0, 0, -120}, {100, 0, 30}, {31, 95, 30}, {-81, 59, 30}, {-81, -59, 30}, {31, -95, 30}, {81, 59, -30}, {-31, 95, -30}, {-100, 0, -30}, {-31, -95, -30}, {81, -59, -30}};
        static uint8_t d10Edges[20][2] = {
            {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {1, 7}, {1, 8}, {1, 9}, {1, 10}, {1, 11}, {2, 7}, {7, 3}, {3, 8}, {8, 4}, {4, 9}, {9, 5}, {5, 10}, {10, 6}, {6, 11}, {11, 2}};

        static int16_t d30Verts[26][3] = {
            {0, 0, 130}, {0, 0, -130}, {100, 0, 40}, {87, 50, 40}, {50, 87, 40}, {0, 100, 40}, {-50, 87, 40}, {-87, 50, 40}, {-100, 0, 40}, {-87, -50, 40}, {-50, -87, 40}, {0, -100, 40}, {50, -87, 40}, {87, -50, 40}, {97, 26, -40}, {71, 71, -40}, {26, 97, -40}, {-26, 97, -40}, {-71, 71, -40}, {-97, 26, -40}, {-97, -26, -40}, {-71, -71, -40}, {-26, -97, -40}, {26, -97, -40}, {71, -71, -40}, {97, -26, -40}};
        static uint8_t d30Edges[48][2] = {
            {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {1, 14}, {1, 15}, {1, 16}, {1, 17}, {1, 18}, {1, 19}, {1, 20}, {1, 21}, {1, 22}, {1, 23}, {1, 24}, {1, 25}, {2, 14}, {3, 15}, {4, 16}, {5, 17}, {6, 18}, {7, 19}, {8, 20}, {9, 21}, {10, 22}, {11, 23}, {12, 24}, {13, 25}, {2, 25}, {3, 14}, {4, 15}, {5, 16}, {6, 17}, {7, 18}, {8, 19}, {9, 20}, {10, 21}, {11, 22}, {12, 23}, {13, 24}};

        static int16_t d20Verts[12][3] = {
            {0, 100, PHI_VAL}, {0, 100, -PHI_VAL}, {0, -100, PHI_VAL}, {0, -100, -PHI_VAL}, {100, PHI_VAL, 0}, {100, -PHI_VAL, 0}, {-100, PHI_VAL, 0}, {-100, -PHI_VAL, 0}, {PHI_VAL, 0, 100}, {PHI_VAL, 0, -100}, {-PHI_VAL, 0, 100}, {-PHI_VAL, 0, -100}};
        static uint8_t d20Edges[30][2] = {
            {0, 2}, {0, 4}, {0, 6}, {0, 8}, {0, 10}, {1, 3}, {1, 4}, {1, 6}, {1, 9}, {1, 11}, {2, 5}, {2, 7}, {2, 8}, {2, 10}, {3, 5}, {3, 7}, {3, 9}, {3, 11}, {4, 6}, {4, 8}, {4, 9}, {5, 7}, {5, 8}, {5, 9}, {6, 10}, {6, 11}, {7, 10}, {7, 11}, {8, 9}, {10, 11}};

        Shape shapes[] = {
            {16, 24, d2Verts, d2Edges},   // D2
            {8, 12, d6Verts, d6Edges},    // D6
            {12, 20, d10Verts, d10Edges}, // D10
            {12, 30, d20Verts, d20Edges}, // D20
            {26, 48, d30Verts, d30Edges}  // D100 (as D30)
        };

        dis->fillScreen(SCWhite);

        setFont(getFont("UbuntuMono10"));
        for (int i = 0; i < 3; i++)
        {
            if (lastRolls[i] > 0)
            {
                dis->setCursor(5, 15 + i * 15);
                dis->print(lastRolls[i]);
            }
        }

        Shape s = shapes[currentDiceType];
        int centerX = 100;
        int centerY = 80;
        int16_t scale = 0;
        if (currentDiceType == 0) scale = 60;
        else if (currentDiceType == 1) scale = 48;
        else if (currentDiceType == 2) scale = 65;
        else if (currentDiceType == 3) scale = 40;
        else scale = 60;

        int16_t projX[32], projY[32];
        const float rad = 3.14159f / 180.0f;
        float ax = angleX * rad, ay = angleY * rad, az = angleZ * rad;

        for (int i = 0; i < s.numVertices; i++)
        {
            float x = s.vertices[i][0] / 100.0f;
            float y = s.vertices[i][1] / 100.0f;
            float z = s.vertices[i][2] / 100.0f;

            float y1 = y * cos(ax) - z * sin(ax);
            float z1 = y * sin(ax) + z * cos(ax);
            float x2 = x * cos(ay) + z1 * sin(ay);
            float x3 = (x2 * cos(az) - y1 * sin(az)) * scale;
            float y3 = (x2 * sin(az) + y1 * cos(az)) * scale;

            projX[i] = centerX + (int16_t)x3;
            projY[i] = centerY + (int16_t)y3;
        }

        for (int i = 0; i < s.numEdges; i++)
        {
            dis->drawLine(projX[s.edges[i][0]], projY[s.edges[i][0]],
                          projX[s.edges[i][1]], projY[s.edges[i][1]], SCBlack);
        }

        if (!rolling && rollResult > 0)
        {
            setFont(getFont("Mono33"));
            dis->setTextSize(1);
            writeTextCenterReplaceBack(String(rollResult), 200);
        }

        resetSleepDelay();
    }
    disUp();
}

void exitDiceApp()
{
}

#endif
