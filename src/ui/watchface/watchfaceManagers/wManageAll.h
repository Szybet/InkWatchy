#ifndef WMANAGE_ALL_H
#define WMANAGE_ALL_H

#include "../../../defines/defines.h"

// This is a forward declaration
struct watchfaceDefOne;

const watchfaceDef *getCurrentWatchface();
const watchfaceDefOne *getwatchfaceDefOne();
void initWatchfaceManage();
void loopWatchfaceManage();

#endif
