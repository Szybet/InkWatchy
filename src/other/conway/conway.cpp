#include "conway.h"

// Stolen from https://github.com/delhoume/ssd1306_adafruit_game_of_life/blob/master/ssd1306_adafruit_game_of_life.ino

#define CONWAY_WIDTH 200
#define CONWAY_HEIGHT 200
#define CONWAY_PIXEL_SIZE 1

uint8_t grid[CONWAY_WIDTH / 8 * CONWAY_HEIGHT];
uint8_t newgrid[CONWAY_WIDTH / 8 * CONWAY_HEIGHT];

static const uint8_t PROGMEM
    GFXsetBit[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01},
    GFXclrBit[] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};

inline void setPixel(uint8_t *ptr, uint8_t x, uint8_t y, uint8_t color)
{
    uint16_t idx = (x + y * CONWAY_WIDTH) / 8;
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

inline uint8_t getPixel(uint8_t *ptr, uint8_t x, uint8_t y)
{
    uint16_t idx = (x + y * CONWAY_WIDTH) / 8;
    //  uint8_t value = ptr[idx] & (0x80 >> (x & 7));
    uint8_t value = ptr[idx] & pgm_read_byte(&GFXsetBit[x & 7]);
    return value == 0 ? 0 : 1;
}

void drawGrid()
{
    display.fillScreen(GxEPD_WHITE);
    for (uint8_t y = 1; y < (CONWAY_HEIGHT - 1); y++)
    {
        for (uint8_t x = 1; x < (CONWAY_WIDTH - 1); x++)
        {
            uint8_t color = getPixel(grid, x, y);
            if (color == 1)
            {
                display.drawPixel(x, y, GxEPD_BLACK);
                //display.fillRect(offsetx + CONWAY_PIXEL_SIZE * x, offsety + CONWAY_PIXEL_SIZE * y, CONWAY_PIXEL_SIZE, CONWAY_PIXEL_SIZE, GxEPD_BLACK);
            }
        }
    }
    display.display(PARTIAL_UPDATE);
}

void initGrid()
{
    for (uint8_t y = 0; y < CONWAY_HEIGHT; y++)
    {
        for (uint8_t x = 0; x < CONWAY_WIDTH; x++)
        {
            if (x == 0 || x == (CONWAY_WIDTH - 1) || y == 0 || y == (CONWAY_HEIGHT - 1))
            {
                setPixel(grid, x, y, 0);
            }
            else
            {
                setPixel(grid, x, y, random(2));
            }
        }
    }
}

inline uint8_t getNumberOfNeighbors(uint8_t x, uint8_t y)
{
    return getPixel(grid, x - 1, y) +
           getPixel(grid, x + 1, y) +
           getPixel(grid, x - 1, y - 1) +
           getPixel(grid, x, y - 1) +
           getPixel(grid, x + 1, y - 1) +
           getPixel(grid, x - 1, y + 1) +
           getPixel(grid, x, y + 1) +
           getPixel(grid, x + 1, y + 1);
}

void computeNewGeneration()
{
    for (uint8_t y = 1; y < (CONWAY_HEIGHT - 1); y++)
    {
        for (uint8_t x = 1; x < (CONWAY_WIDTH - 1); x++)
        {
            uint8_t neighbors = getNumberOfNeighbors(x, y);
            uint8_t current = getPixel(grid, x, y);
            if (current == 1 && (neighbors == 2 || neighbors == 3))
            {
                setPixel(newgrid, x, y, 1);
            }
            else if (current == 1)
                setPixel(newgrid, x, y, 0);
            if (current == 0 && (neighbors == 3))
            {
                setPixel(newgrid, x, y, 1);
            }
            else if (current == 0)
                setPixel(newgrid, x, y, 0);
        }
    }
    memcpy(grid, newgrid, CONWAY_WIDTH * CONWAY_HEIGHT / 8);
}

void initConway()
{
    display.fillScreen(GxEPD_WHITE);
    initGrid();
    drawGrid();
    delay(200);
}

void loopConway()
{
    computeNewGeneration();
    drawGrid();
}