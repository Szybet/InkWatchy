#include "accDebug.h"
#include "rtcMem.h"

#if DEBUG_MENUS && ACC_ENABLED

// Real-world dimensions in mm
const float LENGTH_MM = 46.0f;
const float WIDTH_MM = 34.0f;
const float THICKNESS_MM = 16.0f;

// Normalized dimensions (longest side = 2 units)
const float BASE_SCALE = 2.0f / LENGTH_MM; // Normalize to longest dimension
const float WIDTH_RATIO = WIDTH_MM * BASE_SCALE;
const float THICKNESS_RATIO = THICKNESS_MM * BASE_SCALE;

const int CUBE_SIZE = 70; // Visual scaling factor
int16_t centerX = 200 / 2;
int16_t centerY = 200 / 2;

// Dimension-corrected vertices
float rectVertices[8][3] = {
    // Front face
    {-1.0f, -WIDTH_RATIO / 2, -THICKNESS_RATIO / 2},
    {1.0f, -WIDTH_RATIO / 2, -THICKNESS_RATIO / 2},
    {1.0f, WIDTH_RATIO / 2, -THICKNESS_RATIO / 2},
    {-1.0f, WIDTH_RATIO / 2, -THICKNESS_RATIO / 2},
    // Back face
    {-1.0f, -WIDTH_RATIO / 2, THICKNESS_RATIO / 2},
    {1.0f, -WIDTH_RATIO / 2, THICKNESS_RATIO / 2},
    {1.0f, WIDTH_RATIO / 2, THICKNESS_RATIO / 2},
    {-1.0f, WIDTH_RATIO / 2, THICKNESS_RATIO / 2}};

uint8_t edges[12][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Front face
    {4, 5},
    {5, 6},
    {6, 7},
    {7, 4}, // Back face
    {0, 4},
    {1, 5},
    {2, 6},
    {3, 7} // Connecting edges
};

Accel acc;

void loopAcc3D()
{
    rM.SBMA.getAccel(&acc);
    int16_t accX = acc.x;
    int16_t accY = acc.y;
    int16_t accZ = acc.z;

    float pitch = atan2(-accX, sqrt(accY * accY + accZ * accZ));
    float roll = atan2(accY, accZ);

    int16_t projX[8], projY[8];

    for (int i = 0; i < 8; i++)
    {
        float x = rectVertices[i][0];
        float y = rectVertices[i][1];
        float z = rectVertices[i][2];

        // X-axis rotation
        float y1 = y * cos(pitch) - z * sin(pitch);
        float z1 = y * sin(pitch) + z * cos(pitch);

        // Y-axis rotation
        float x2 = x * cos(roll) + z1 * sin(roll);
        float z2 = -x * sin(roll) + z1 * cos(roll);

        projX[i] = centerX + (int)(x2 * CUBE_SIZE);
        projY[i] = centerY + (int)(y1 * CUBE_SIZE);
    }

    // Drawing section
    dis->fillScreen(GxEPD_WHITE);

    // Draw thick edges with consistent 3-pixel width
    for (int i = 0; i < 12; i++)
    {
        uint8_t start = edges[i][0];
        uint8_t end = edges[i][1];

        int16_t x1 = projX[start];
        int16_t y1 = projY[start];
        int16_t x2 = projX[end];
        int16_t y2 = projY[end];

        // Calculate line direction
        float dx = x2 - x1;
        float dy = y2 - y1;
        float length = sqrt(dx * dx + dy * dy);

        if (length > 0)
        {
            // Get perpendicular direction for thickness
            float perpX = -dy / length;
            float perpY = dx / length;

            // Draw 3 parallel lines for thickness
            for (int t = -1; t <= 1; t++)
            {
                int16_t offsetX = round(t * perpX);
                int16_t offsetY = round(t * perpY);
                dis->drawLine(x1 + offsetX, y1 + offsetY,
                              x2 + offsetX, y2 + offsetY, GxEPD_BLACK);
            }
        }
    }

    // Calculate front face center
    int16_t frontCenterX = (projX[4] + projX[5] + projX[6] + projX[7]) / 4;
    int16_t frontCenterY = (projY[4] + projY[5] + projY[6] + projY[7]) / 4;

    // Calculate proper perpendicular directions using face edges
    // Get right edge midpoint (between vertices 5 and 6)
    int16_t rightMidX = (projX[5] + projX[6]) / 2;
    int16_t rightMidY = (projY[5] + projY[6]) / 2;

    // Get top edge midpoint (between vertices 6 and 7)
    int16_t topMidX = (projX[6] + projX[7]) / 2;
    int16_t topMidY = (projY[6] + projY[7]) / 2;

    // Calculate direction vectors from center
    int16_t hourDirX = rightMidX - frontCenterX;
    int16_t hourDirY = rightMidY - frontCenterY;
    int16_t minuteDirX = topMidX - frontCenterX;
    int16_t minuteDirY = topMidY - frontCenterY;

    // Clock hands lengths
    const float HOUR_HAND_LENGTH = 0.6f;   // 60% of max
    const float MINUTE_HAND_LENGTH = 0.4f; // 40% of max

    // Calculate hand endpoints
    int16_t hourEndX = frontCenterX + HOUR_HAND_LENGTH * hourDirX;
    int16_t hourEndY = frontCenterY + HOUR_HAND_LENGTH * hourDirY;

    int16_t minuteEndX = frontCenterX + MINUTE_HAND_LENGTH * minuteDirX;
    int16_t minuteEndY = frontCenterY + MINUTE_HAND_LENGTH * minuteDirY;

    // Draw clock hands
    dis->drawLine(frontCenterX, frontCenterY, hourEndX, hourEndY, GxEPD_BLACK);
    dis->drawLine(frontCenterX, frontCenterY, minuteEndX, minuteEndY, GxEPD_BLACK);

    dUChange = true;
}

#endif