#pragma once

#include "music/songs.h"

typedef struct Playlist {
  int currentSong;
  Song* songs;
} Playlist;

Playlist CreateFullPlaylist(Song* songs);
