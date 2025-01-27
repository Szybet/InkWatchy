#pragma once

#include "defines.h"

#if VIDEO_PLAYER

extern String videoName;

void initVideoPlayer();
void loopVideoPlayer();
void exitVideoPlayer();

void initVideoMenu();

#endif