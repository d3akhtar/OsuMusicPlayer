#include  "song_info.h"
#include "music/current_playlist.h"
#include "raylib/raylib.h"
#include "raylib/raygui.h"

static Texture2D songIconTexture = {-1u, 0, 0, 0, 0};
static Song* currentSong = NULL;

void init_song_info()
{
  set_song_info_song(&current_playlist()->songs[current_playlist()->currentSong]);
}

void set_song_info_song(Song* song)
{
  currentSong = song;
  songIconTexture = LoadTexture(currentSong->bgFilePath);  
}

void gui_draw_song_info()
{  
  DrawRectangle(915, 5, 360, 360, BLACK);
  DrawTexturePro(songIconTexture, (Rectangle){0,0,(float)songIconTexture.width, (float)songIconTexture.height}, (Rectangle){920, 10, 350, 350}, (Vector2){0,0}, 0.0f, WHITE);
  DrawText(TextFormat("Song: %s", currentSong->songTitle), 920, 370, 20, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
  DrawText(TextFormat("Artist: %s", currentSong->artistName), 920, 390, 20, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
}
