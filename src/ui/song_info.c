#include  "song_info.h"
#include "raylib/raylib.h"
#include "raylib/raygui.h"

static Texture2D songIconTexture = {-1u, 0, 0, 0, 0};

void init_song_info()
{
  songIconTexture = LoadTexture("./resources/pspace.PNG");
}

void gui_draw_song_info()
{  
  DrawRectangle(915, 5, 360, 360, BLACK);
  DrawTextureRec(songIconTexture, (Rectangle){songIconTexture.width/4.0f,songIconTexture.height/4.0f,350,350}, (Vector2){920,10}, WHITE);
  DrawText("Song: PARTY In PSPACE", 920, 370, 20, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
  DrawText("Artist: tnshi", 920, 390, 20, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
}
