#include "gui.h"
#include "raylib/raylib.h"
#include "raylib/raymath.h"

int GuiDrawIconButton(int iconId, int posX, int posY, int pixelSize, Color iconColor)
{
  int result = RESULT_NONE;
  
  Color color = iconColor;
  
  Vector2 mousePos = GetMousePosition();

  Rectangle rect = {posX, posY, 16 * pixelSize, 16 * pixelSize};

  if (CheckCollisionPointRec(mousePos, rect)) {
    color = GREEN;

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      color = BLUE;
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
      color = GREEN;
      result = RESULT_PRESSED;
    }
  }

  GuiDrawIcon(iconId, posX, posY, pixelSize, color);

  return result;
}

void GuiDrawMusicProgressBar(int posX, int posY, int width, int height, float progress, Color unfinishedColor, Color finishedColor)
{
  Clamp(progress, 0.0f, 1.0f);
  
  DrawRectangle(posX, posY, width*progress, height, finishedColor);
  DrawRectangle(posX+width*progress, posY, width*(1-progress), height, unfinishedColor);

  int playbackCursorPosX = posX + 5 + width*progress;
  DrawCircle(playbackCursorPosX, posY + 5, 12, BLACK);
  DrawCircle(playbackCursorPosX, posY + 5, 10, PINK);
}
