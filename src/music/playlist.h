#pragma once

#include "music/songs.h"

typedef struct Playlist {
  char * name;
  int currentSong, nSongs;
  Song* songs;
} Playlist;

Playlist* create_playlist_for_songs(Song* songs, int nSongs, char const *name);
Song* playlist_current_song(Playlist *playlist);
void free_playlist(Playlist *playlist);
