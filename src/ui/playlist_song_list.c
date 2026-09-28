#include "playlist_song_list.h"
#include "music/current_playlist.h"
#include "music/playlist.h"
#include "ui/music_playback.h"
#include "ui/song_info.h"

#include <raylib/raygui.h>

static int previousSelectedIndex = 0;
static int currentPlaylistScrollIndex = 0;
static int currentPlaylistActive = 0;


void gui_draw_playlist_song_list()
{
  // TODO: save these in current playlist file
  char const * playlistTitleText = TextFormat("Playlist: %s", current_playlist()->name);
  char ** playlistSongsList = (char**)playlist_song_names();

  previousSelectedIndex = currentPlaylistActive;

  DrawText(playlistTitleText, 920, 420, 20, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
  GuiSetStyle(LISTVIEW, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
  GuiSetStyle(LISTVIEW, TEXT_PADDING, 10);
  GuiSetStyle(LISTVIEW, BORDER_WIDTH, 2);
  GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_NORMAL, 1);
  GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_WIDTH, 1);

  GuiListViewEx((Rectangle) {920, 450, 350, 200}, playlistSongsList, current_playlist()->nSongs, &currentPlaylistScrollIndex, &currentPlaylistActive, NULL);
  if (currentPlaylistActive != previousSelectedIndex) {
    set_playlist_current_song(currentPlaylistActive);
    set_song(playlist_current_song(current_playlist())->audioFilePath);
    set_song_info_song(playlist_current_song(current_playlist()));
  }
  
  GuiSetStyle(LISTVIEW, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
  GuiSetStyle(LISTVIEW, TEXT_PADDING, 0);
  GuiSetStyle(LISTVIEW, BORDER_WIDTH, 2);
  GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_NORMAL, 1);
  GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_WIDTH, 1);
}
