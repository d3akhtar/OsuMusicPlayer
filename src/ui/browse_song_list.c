#include "browse_song_list.h"
#include "music/current_playlist.h"
#include "ui/music_playback.h"
#include "ui/song_info.h"
#include "utils/string.h"

#include <string.h>
#include <raylib/raylib.h>
#include <raylib/raygui.h>

static bool showBrowseSongList = false;
static bool searchBrowseSongListEdit = false;
static int const searchBrowseSongListQueryMaxSize = 1024;
static char searchBrowseSongList[1024] = {'\0'};

static int collectionDropdownOption = 0;
static bool collectionDropdownIsSelecting = false;

static int songListPreviousActive = 0;
static int songListIndex = 0;
static int songListActive = 0;

void gui_draw_browse_song_list()
{
  songListPreviousActive = songListActive;
  
  char **songList = (char**)playlist_song_full_names();
  char const *playlists = create_list_view_text_for_string_list((char**)playlist_names(), number_of_playlists());
  
  if (GuiButton((Rectangle) {920, 660, 350, 40}, "Browse")) showBrowseSongList = true;

  if (showBrowseSongList) {
      if (GuiWindowBox((Rectangle) {340, 10, 600, 700 }, "Browse songs") == RESULT_PRESSED)
          showBrowseSongList = false;
    
      if (GuiTextBox((Rectangle) {340, 34, 450, 30}, searchBrowseSongList, searchBrowseSongListQueryMaxSize, searchBrowseSongListEdit)) {
          searchBrowseSongList[0] = '\0';
          searchBrowseSongListEdit = !searchBrowseSongListEdit;
      }

      if (!searchBrowseSongListEdit && strlen(searchBrowseSongList) == 0)
          DrawText("Search songs...", 345, 40, 18, BLACK);

      GuiSetStyle(LISTVIEW, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
      GuiSetStyle(LISTVIEW, TEXT_PADDING, 10);
      GuiSetStyle(LISTVIEW, BORDER_WIDTH, 2);
      GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_NORMAL, 1);
      GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_WIDTH, 1);
      GuiListViewEx((Rectangle) {340, 60, 600, 656}, songList, current_playlist()->nSongs, &songListIndex, &songListActive, NULL);
      GuiSetStyle(LISTVIEW, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
      GuiSetStyle(LISTVIEW, TEXT_PADDING, 0);
      GuiSetStyle(LISTVIEW, BORDER_WIDTH, 0);
      GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_NORMAL, 0);
      GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_WIDTH, 0);

      GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
      GuiSetStyle(DROPDOWNBOX, TEXT_PADDING, 10);
      switch (GuiDropdownBox((Rectangle) {790, 34, 150, 28}, playlists, &collectionDropdownOption, collectionDropdownIsSelecting)) {
          case RESULT_CHANGED:
            set_current_playlist(collectionDropdownOption);
            collectionDropdownIsSelecting = false;
            songListActive = 0;
            songListPreviousActive = 0;
            set_playlist_current_song(0);
            set_song(playlist_current_song(current_playlist())->audioFilePath);
            set_song_info_song(playlist_current_song(current_playlist()));
            break;
          case RESULT_PRESSED:
            collectionDropdownIsSelecting = true;
            break;
      }
      GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
      GuiSetStyle(DROPDOWNBOX, TEXT_PADDING, 0);

      if (songListActive != songListPreviousActive) {
        set_playlist_current_song(songListActive);
        set_song(playlist_current_song(current_playlist())->audioFilePath);
        set_song_info_song(playlist_current_song(current_playlist()));
      }

      if (songListActive != current_playlist()->currentSong)
        songListActive = current_playlist()->currentSong;
  }  
}
