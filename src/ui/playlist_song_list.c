#include "playlist_song_list.h"
#include "music/current_playlist.h"
#include "music/playlist.h"
#include "utils/string.h"

#include <raylib/raygui.h>

static int currentPlaylistScrollIndex = 0;
static int currentPlaylistActive = 1;

void init_playlist_song_list()
{
}

void gui_draw_playlist_song_list()
{
  // TODO: save these in current playlist file
  char const * playlistTitleText = TextFormat("Playlist: %s", current_playlist()->name);
  char const * playlistSongsList = create_list_view_text_for_playlist(current_playlist());

  DrawText(playlistTitleText, 920, 420, 20, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
  GuiSetStyle(LISTVIEW, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
  GuiSetStyle(LISTVIEW, TEXT_PADDING, 10);
  GuiSetStyle(LISTVIEW, BORDER_WIDTH, 2);
  GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_NORMAL, 1);
  GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_WIDTH, 1);
  GuiListView((Rectangle) {920, 450, 350, 200}, playlistSongsList, &currentPlaylistScrollIndex, &currentPlaylistActive);
  GuiSetStyle(LISTVIEW, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
  GuiSetStyle(LISTVIEW, TEXT_PADDING, 0);
  GuiSetStyle(LISTVIEW, BORDER_WIDTH, 2);
  GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_NORMAL, 1);
  GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_WIDTH, 1);
}
