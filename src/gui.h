#pragma once

#include <raylib/raylib.h>
#include <raylib/raygui.h>

int gui_draw_icon_button(int iconId, int posX, int posY, int pixelSize, Color iconColor);
int gui_draw_music_progress_bar(int posX, int posY, int width, int height, float *progress, Color unfinishedColor, Color finishedColor);
