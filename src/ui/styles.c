#include "styles.h"

#include <raylib/raygui.h>

void init_default_styles()
{  
  GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt((Color){244, 226, 245, 255}));
  GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt((Color){61, 61, 61, 255}));
  GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt((Color){0, 0, 0, 255}));
  GuiSetStyle(DEFAULT, LIST_ITEMS_BORDER_NORMAL, ColorToInt(BLACK));
  GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt((Color){255, 163, 217, 255}));
  GuiSetStyle(SLIDER, BASE_COLOR_NORMAL, ColorToInt(GRAY));
  GuiSetStyle(STATUSBAR, BASE_COLOR_NORMAL, ColorToInt((Color){197, 140, 250, 255}));
  GuiSetStyle(DEFAULT, BORDER_WIDTH, 2);
  GuiSetStyle(DROPDOWNBOX, BASE_COLOR_NORMAL, ColorToInt((Color){244, 226, 245, 255}));
}
