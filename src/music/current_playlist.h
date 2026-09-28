#pragma once

#include "music/playlist.h"

void load_playlists();
Playlist* current_playlist();
int number_of_playlists();
char const ** playlist_names();
char const ** playlist_song_names();
void set_current_playlist(unsigned int index);
void set_playlist_current_song(unsigned int index);
void unload_playlists();
