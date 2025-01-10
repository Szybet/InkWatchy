#include "tetrisPlace.h"

#if TETRIS

// From https://github.com/0xjmux/tetris
// Thanks, it's awesome!
#include "import/tetris.h"

TetrisGame *tg;
#define TETRIS_COUNT_MAX 15
uint8_t tetrisCount = 0;
bool showedGameOver = false;

void deInitTetrisStuff()
{
    end_game(tg);
}

void initTetrisStuff()
{
    tg = create_game();
    create_rand_piece(tg);
    tetrisCount = 0;
    showedGameOver = false;
}

void exitTetris()
{
    deInitTetrisStuff();
    restoreCpuSpeed();
}

void initTetris()
{
    initTetrisStuff();
    getCpuSpeed();
    setCpuSpeed(maxSpeed);
}

#define SQUARE_SIZE 10
#define BOARD_LEFT_X 0
#define BOARD_UP_Y 0

void drawTetrisText()
{
    setFont(&FreeSansBold9pt7b);
    setTextSize(1);
    dis->setTextWrap(false);

    dis->setCursor(96, 17);
    dis->print("Blockchy");

    dis->setCursor(96, 34);
    dis->print("Line clears:");

    dis->setCursor(96, 49);
    dis->print(String(tg->lines_cleared_since_last_level));

    dis->setCursor(96, 65);
    dis->print("Score:");

    dis->setCursor(96, 81);
    dis->print(String(tg->score));

    dis->setCursor(96, 96);
    dis->print("Level:");

    dis->setCursor(96, 114);
    dis->print(String(tg->level));

    dis->setCursor(96, 150);
    dis->print("Up:Right");

    dis->setCursor(96, 168);
    dis->print("Down:Left");

    if (tg->game_over == true)
    {
        setFont(getFont("smileandwave20"));
        dis->setCursor(0, 110);
        dis->print("GAMEOVER");
        showedGameOver = true;
    }
}

void loopTetris()
{
    tetrisCount = tetrisCount + 1;
    enum player_move move = T_NONE;
    buttonState btn = useButton();
    switch (btn)
    {
    case Up:
    {
        move = player_move::T_RIGHT;
        break;
    }
    case Down:
    {
        move = player_move::T_LEFT;
        break;
    }
    case LongDown:
    {
        move = player_move::T_DOWN;
        break;
    }
    case Menu:
    {
        move = player_move::T_UP;
        break;
    }
    case LongMenu:
    {
        deInitTetrisStuff();
        initTetrisStuff();
        break;
    }
    default:
    {
        break;
    }
    }

    if (tetrisCount < TETRIS_COUNT_MAX && move == T_NONE)
    {
        return;
    }
    else
    {
        tetrisCount = 0;
    }

    tg_tick(tg, move);
    if (showedGameOver == false)
    {
        dis->fillScreen(GxEPD_WHITE);
        writeImageN(BOARD_LEFT_X, BOARD_UP_Y, getImg("tetris/tetris"));
        for (uint8_t y = 0; y < TETRIS_ROWS; y++)
        {
            for (uint8_t x = 0; x < TETRIS_COLS; x++)
            {
                if (tg->active_board.board[y][x] != -1)
                {
                    uint16_t rectX = BOARD_LEFT_X + (x * SQUARE_SIZE);
                    uint16_t rectY = BOARD_UP_Y + (y * SQUARE_SIZE);
                    dis->fillRect(rectX, rectY, SQUARE_SIZE, SQUARE_SIZE, GxEPD_BLACK);
                    if (tg->active_board.board[y][x] != tg->board.board[y][x])
                    {
                        dis->drawRect(rectX + 2, rectY + 2, 6, 6, GxEPD_WHITE);
                    }
                }
            }
        }
        drawTetrisText();
        disUp(true, false, true);
    }
    resetSleepDelay();
    // if(move == T_NONE) {
    // delayTask(250);
    //}
}

#endif