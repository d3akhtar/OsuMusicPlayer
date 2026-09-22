#pragma once

#include "music/songs.h"

typedef struct Playlist {
  int currentSong, nSongs;
  Song* songs;
} Playlist;

Playlist CreatePlaylistForSongs(Song* songs, int nSongs);
