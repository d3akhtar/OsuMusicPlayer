#include  "song_info.h"
#include "music/current_playlist.h"
#include "music/playlist.h"
#include "raylib/raylib.h"
#include "raylib/raygui.h"
#include "ui/music_playback.h"

static Texture2D songIconTexture = {-1u, 0, 0, 0, 0};
static Song* currentSong = NULL;

void init_song_info()
{
  set_song_info_song(playlist_current_song(current_playlist()));
}

void set_song_info_song(Song* song)
{
  currentSong = song;
  UnloadTexture(songIconTexture);

  Image bgImage = LoadImage(currentSong->bgFilePath);
  songIconTexture = LoadTextureFromImage(bgImage);

  unsigned long avgColorR = 0;
  unsigned long avgColorG = 0;
  unsigned long avgColorB = 0;
  Color *pixels = LoadImageColors(bgImage);
  int nPixels = bgImage.width * bgImage.height;
  for (int i = 0; i < nPixels; i++)
  {
    avgColorR += pixels[i].r;
    avgColorG += pixels[i].g;
    avgColorB += pixels[i].b;
  }

  Color avgColor = {
    .r = (unsigned char)((avgColorR / (float)nPixels)),
    .g = (unsigned char)((avgColorG / (float)nPixels)),
    .b = (unsigned char)((avgColorB / (float)nPixels)),
    .a = 255
  };
  
  set_visualizer_bar_color(avgColor);

  UnloadImageColors(pixels);
  UnloadImage(bgImage);
}

void gui_draw_song_info()
{  
  DrawRectangle(915, 5, 360, 360, BLACK);
  DrawTexturePro(songIconTexture, (Rectangle){0,0,(float)songIconTexture.width, (float)songIconTexture.height}, (Rectangle){920, 10, 350, 350}, (Vector2){0,0}, 0.0f, WHITE);
  DrawText(TextFormat("Song: %s", currentSong->songTitle), 920, 370, 20, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
  DrawText(TextFormat("Artist: %s", currentSong->artistName), 920, 390, 20, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
}
