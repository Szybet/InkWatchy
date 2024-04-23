#ifndef CONWAY_H
#define CONWAY_H

#include "../../defines/defines.h"

void initConwayGrid(uint8_t *conwayGrid, uint8_t randomPerc, uint8_t conwayHeight, uint8_t conwayWidth);
void drawGrid(uint8_t *conwayGrid, uint8_t conwayHeight, uint8_t conwayWidth, uint8_t xOffset, uint8_t yOffset);
void computeNewGeneration(uint8_t *currentGrid, uint8_t *newGrid, uint8_t conwayHeight, uint8_t conwayWidth);

void initConway();
void loopConway();

#endif