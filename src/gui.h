#pragma once

#include <raylib/raylib.h>
#include <raylib/raygui.h>

int GuiDrawIconButton(int iconId, int posX, int posY, int pixelSize, Color iconColor);

void GuiDrawMusicProgressBar(int posX, int posY, int width, int height, float progress, Color unfinishedColor, Color finishedColor);
