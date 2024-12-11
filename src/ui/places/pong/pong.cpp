#include "pong.h"

#if PONG

#define INIT_BALL_DIR_Y 1
#define BALL_SPEED 8
#define BALL_RADIUS 5
#define BALL_BORDER_MINUS 5
#define BALL_RANDOM_RANGE 5
#define PADDLE_W 5
#define PADDLE_X 0
#define PADDLE_H 50
#define PADDLE_SPEED 30
#define POINT_Y 20
#define BALL_CENTER_FASTER 1.3
#define BALL_CENTER_FASTER_X 120

struct xy
{
    int x;
    int y;
};

xy ball;
xy ballDirection;

int paddle; // The bottom of the paddle
int points;

bool forcePaddle = false;
bool lostPong = false;

int ballRandomGet()
{
    int rand = betterRandom(0 - BALL_RANDOM_RANGE, 0 + BALL_RANDOM_RANGE + 1);
    if (rand >= -1 && rand <= 1)
    {
        return ballRandomGet();
    }
    return rand;
}

void drawPoint()
{
    setFont(font);
    setTextSize(1);
    writeTextCenterReplaceBack(String(points), POINT_Y);
}

void zeroBalls()
{
    if (ball.x < 0)
    {
        ball.x = 0;
    }
    if (ball.x > display.width())
    {
        ball.x = display.width();
    }
    if (ball.y < 0)
    {
        ball.y = 0;
    }
    if (ball.y > display.height())
    {
        ball.y = display.height();
    }
}

void drawBall()
{
    display.fillCircle(ball.x, ball.y, BALL_RADIUS, GxEPD_WHITE);
    if (ball.x > BALL_CENTER_FASTER_X)
    {
        ball.x = ball.x + ballDirection.x * (BALL_SPEED * BALL_CENTER_FASTER);
        ball.y = ball.y + ballDirection.y * (BALL_SPEED * BALL_CENTER_FASTER);
    }
    else
    {
        ball.x = ball.x + ballDirection.x * BALL_SPEED;
        ball.y = ball.y + ballDirection.y * BALL_SPEED;
    }
    zeroBalls();

    if (ball.x >= display.width() - BALL_BORDER_MINUS || ball.x <= 0 + BALL_BORDER_MINUS)
    {
        // Point?
        if (ball.x < BALL_BORDER_MINUS * 1.5)
        {
            if (ball.y <= paddle + PADDLE_H && ball.y >= paddle)
            {
                debugLog("Point!");
                points = points + 1;
                drawPoint();
                forcePaddle = true;
            }
            else
            {
                lostPong = true;
                writeTextCenterReplaceBack("You lost!", display.height() / 2);
            }
        }
        ballDirection.x = ballDirection.x * -1;
        int rand = ballRandomGet();
        // Make sure it's the same as in below above zero
        if ((ballDirection.x > 0 && rand < 0) || (ballDirection.x < 0 && rand > 0))
        {
            rand = rand * -1;
        }
        ballDirection.x = rand;
        if (ball.x > display.width() - BALL_BORDER_MINUS)
        {
            ball.x = display.width() - BALL_BORDER_MINUS - 3;
        }
        if (ball.x < 0 + BALL_BORDER_MINUS)
        {
            ball.x = ball.x + BALL_BORDER_MINUS + 3;
        }
    }
    if (ball.y > display.height() - BALL_BORDER_MINUS || ball.y < 0 + BALL_BORDER_MINUS)
    {
        ballDirection.y = ballDirection.y * -1;
    }

    display.fillCircle(ball.x, ball.y, BALL_RADIUS, GxEPD_BLACK);
}

void movePaddle(int h)
{
    h = h * -1;
    paddle = paddle + h;
    if (paddle > display.height() - PADDLE_H)
    {
        paddle = display.height() - PADDLE_H;
    }
    if (paddle < 0)
    {
        paddle = 0;
    }
}

void drawPaddle(buttonState btn, bool force)
{
    int oldPaddle = paddle;
    switch (btn)
    {
    case Up:
    {
        movePaddle(PADDLE_SPEED);
        break;
    }
    case Down:
    {
        movePaddle(PADDLE_SPEED * -1);
        break;
    }
    case LongUp:
    {
        movePaddle(2 * PADDLE_SPEED);
        break;
    }
    case LongDown:
    {
        movePaddle(2 * PADDLE_SPEED * -1);
        break;
    }
    default:
    {
        if (force == true)
        {
            forcePaddle = false;
        }
        else
        {
            return;
        }
        break;
    }
    }
    display.fillRect(PADDLE_X, oldPaddle, PADDLE_W, PADDLE_H, GxEPD_WHITE);
    display.fillRect(PADDLE_X, paddle, PADDLE_W, PADDLE_H, GxEPD_BLACK);
}

void initPong()
{
    display.fillScreen(GxEPD_WHITE);
    disUp(true);

    ball.x = display.width() / 2;
    ball.y = display.height() / 2;
    ballDirection.x = ballRandomGet();
    if (ballDirection.x < 0)
    {
        ballDirection.x = ballDirection.x * -1;
    }
    ballDirection.y = INIT_BALL_DIR_Y;
    paddle = display.height() / 2;
    points = 0;
    lostPong = false;

    drawPaddle(None, true);
    drawPoint();
}

void loopPong()
{
    debugLog("Ball is: " + String(ball.x) + "x" + String(ball.y));
    buttonState btn = useButton();
    if (lostPong == false)
    {
        drawBall();
        drawPaddle(btn, forcePaddle);
    }
    else
    {
        if (btn != None)
        {
            initPong();
            delayTask(250);
        }
    }

    disUp(true);
    if (btn != None)
    {
        resetSleepDelay(SLEEP_EVERY_MS);
    }
}

#endif