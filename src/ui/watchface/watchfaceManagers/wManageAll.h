#pragma once

#include "defines.h"

#define WATCHFACE_COUNT 3

// This is a forward declaration
struct watchfaceDefOne;
extern const watchfaceDef *watchfacesList[];

const watchfaceDef *getCurrentWatchface();
const watchfaceDefOne *getwatchfaceDefOne();
void initWatchfaceManage();
void loopWatchfaceManage();

