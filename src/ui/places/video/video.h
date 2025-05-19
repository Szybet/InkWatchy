#pragma once

#include "defines.h"

#if VIDEO_PLAYER

extern String videoName;

// Video player functions
void initVideoPlayer();
void loopVideoPlayer();
void exitVideoPlayer();

// Video menu functions (implemented in videoMenu.cpp)
void initVideoMenu();
void choosedVideo();

#endif