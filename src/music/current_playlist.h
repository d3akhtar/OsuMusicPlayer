#pragma once

#include "music/playlist.h"

void load_playlists();
Playlist* current_playlist();
char const ** playlist_names();
char const ** playlist_song_names();
void unload_playlists();
