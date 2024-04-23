#include "conway.h"

// Stolen from https://github.com/delhoume/ssd1306_adafruit_game_of_life/blob/master/ssd1306_adafruit_game_of_life.ino

#define CONWAY_APP_WIDTH 200
#define CONWAY_APP_HEIGHT 200

// width must be multiple of 8
// 8.6 iterations / second
uint8_t conwayAppGrid[CONWAY_APP_WIDTH / 8 * CONWAY_APP_HEIGHT];
uint8_t conwayAppNewGrid[CONWAY_APP_WIDTH / 8 * CONWAY_APP_HEIGHT];

static const uint8_t
    GFXsetBit[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01},
    GFXclrBit[] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};

inline void setPixel(uint8_t *ptr, uint8_t x, uint8_t y, uint8_t color, uint8_t conwayWidth)
{
    uint16_t idx = (x + y * conwayWidth) / 8;
    if (color == 1)
    {
        //     ptr[idx] |= (0x80 >> (x & 7));
        // >> is slow on AVR (Adafruit)
        ptr[idx] |= pgm_read_byte(&GFXsetBit[x & 7]);
    }
    else
    {
        //     ptr[idx] &= ~(0x80 >> (x & 7));
        ptr[idx] &= pgm_read_byte(&GFXclrBit[x & 7]);
    }
}

inline uint8_t getPixel(uint8_t *ptr, uint8_t x, uint8_t y, uint8_t conwayWidth)
{
    uint16_t idx = (x + y * conwayWidth) / 8;
    //  uint8_t value = ptr[idx] & (0x80 >> (x & 7));
    uint8_t value = ptr[idx] & pgm_read_byte(&GFXsetBit[x & 7]);
    return value == 0 ? 0 : 1;
}

void drawGrid(uint8_t *conwayGrid, uint8_t conwayHeight, uint8_t conwayWidth, uint8_t xOffset, uint8_t yOffset)
{
    for (uint8_t y = 1; y < (conwayHeight - 1); y++)
    {
        for (uint8_t x = 1; x < (conwayWidth - 1); x++)
        {
            uint8_t color = getPixel(conwayGrid, x, y, conwayWidth);
            if (color == 1)
            {
                display.drawPixel(x + xOffset, y + yOffset, GxEPD_BLACK);
            }
        }
    }
}

void initConwayGrid(uint8_t *conwayGrid, uint8_t randomPerc, uint8_t conwayHeight, uint8_t conwayWidth)
{
    for (uint8_t y = 0; y < conwayHeight; y++)
    {
        for (uint8_t x = 0; x < conwayWidth; x++)
        {
            if (x == 0 || x == (conwayWidth - 1) || y == 0 || y == (conwayHeight - 1))
            {
                setPixel(conwayGrid, x, y, 0, conwayWidth);
            }
            else
            {
                bool pixelColor = false;
                uint8_t randomChoosed = random(0, 101);
                if (randomChoosed <= randomPerc)
                {
                    pixelColor = true;
                }
                setPixel(conwayGrid, x, y, pixelColor, conwayWidth);
            }
        }
    }
}

inline uint8_t getNumberOfNeighbors(uint8_t x, uint8_t y, uint8_t *conwayGrid, uint8_t conwayWidth)
{
    return getPixel(conwayGrid, x - 1, y, conwayWidth) +
           getPixel(conwayGrid, x + 1, y, conwayWidth) +
           getPixel(conwayGrid, x - 1, y - 1, conwayWidth) +
           getPixel(conwayGrid, x, y - 1, conwayWidth) +
           getPixel(conwayGrid, x + 1, y - 1, conwayWidth) +
           getPixel(conwayGrid, x - 1, y + 1, conwayWidth) +
           getPixel(conwayGrid, x, y + 1, conwayWidth) +
           getPixel(conwayGrid, x + 1, y + 1, conwayWidth);
}

void computeNewGeneration(uint8_t *currentGrid, uint8_t *newGrid, uint8_t conwayHeight, uint8_t conwayWidth)
{
    for (uint8_t y = 1; y < (conwayHeight - 1); y++)
    {
        for (uint8_t x = 1; x < (conwayWidth - 1); x++)
        {
            uint8_t neighbors = getNumberOfNeighbors(x, y, currentGrid, conwayWidth);
            uint8_t current = getPixel(currentGrid, x, y, conwayWidth);
            if (current == 1 && (neighbors == 2 || neighbors == 3))
            {
                setPixel(newGrid, x, y, 1, conwayWidth);
            }
            else if (current == 1)
                setPixel(newGrid, x, y, 0, conwayWidth);
            if (current == 0 && (neighbors == 3))
            {
                setPixel(newGrid, x, y, 1, conwayWidth);
            }
            else if (current == 0)
                setPixel(newGrid, x, y, 0, conwayWidth);
        }
    }
    memcpy(currentGrid, newGrid, conwayWidth * conwayHeight / 8);
}

// disUp(true) is needed here i quess
void initConway()
{
    display.fillScreen(GxEPD_WHITE);
    initConwayGrid(conwayAppGrid, 42, CONWAY_APP_HEIGHT, CONWAY_APP_WIDTH);
    drawGrid(conwayAppGrid, CONWAY_APP_HEIGHT, CONWAY_APP_WIDTH, 0 , 0);
    delay(200);
}

void loopConway()
{
    computeNewGeneration(conwayAppGrid, conwayAppNewGrid, CONWAY_APP_HEIGHT, CONWAY_APP_WIDTH);
    drawGrid(conwayAppGrid, CONWAY_APP_HEIGHT, CONWAY_APP_WIDTH, 0 , 0);
}