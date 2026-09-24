#include "elements.h"
#include "raylib/raylib.h"

bool musicProgressBarChanging = false;

int gui_draw_icon_button(int iconId, int posX, int posY, int pixelSize, Color iconColor)
{
  int result = RESULT_NONE;
  
  Color color = iconColor;
  
  Vector2 mousePos = GetMousePosition();

  Rectangle rect = {(float)posX, (float)posY, 16.0f * pixelSize, 16.0f * pixelSize};

  if (CheckCollisionPointRec(mousePos, rect)) {
    color = GREEN;

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) color = BLUE;

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
      color = GREEN;
      result = RESULT_PRESSED;
    }
  }

  GuiDrawIcon(iconId, posX, posY, pixelSize, color);

  return result;
}
