#include "music_playback.h"
#include "raylib/raygui.h"
#include "raylib/raymath.h"

static bool musicProgressBarChanging = false;

int gui_draw_music_progress_bar(int posX, int posY, int width, int height, float *progress, Color unfinishedColor, Color finishedColor)
{
  Vector2 mousePos = GetMousePosition();

  Clamp(*progress, 0.0f, 1.0f);
  
  DrawRectangle(posX, posY, width**progress, height, finishedColor);
  DrawRectangle(posX+width**progress, posY, width*(1-*progress), height, unfinishedColor);

  int playbackCursorPosX = posX + 5 + width**progress;
  DrawCircle(playbackCursorPosX, posY + 5, 12, BLACK);
  DrawCircle(playbackCursorPosX, posY + 5, 10, PINK);

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && (CheckCollisionPointRec(mousePos, (Rectangle){(float)posX, (float)posY, (float)width, (float)height}) || musicProgressBarChanging)) {
    musicProgressBarChanging = true;
    *progress = (mousePos.x - posX) / (float)width;
    return RESULT_CHANGED;
  } else musicProgressBarChanging = false;

  return RESULT_NONE;
}
