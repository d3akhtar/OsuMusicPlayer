#include "playlist.h"
#include <stdlib.h>
#include <string.h>

int compare(const void *a, const void *b) {
    int artistComp = strcmp(((Song*)a)->artistName, (((Song*)b)->artistName));
    return artistComp == 0
      ? strcmp(((Song*)a)->songTitle, (((Song*)b)->songTitle))
      : artistComp;
}

Playlist create_playlist_for_songs(Song* songs, int nSongs, char const *name)
{
  qsort(songs, nSongs, sizeof(Song), compare);

  Playlist playlist = {
    .currentSong = 0,
    .nSongs = nSongs,
    .songs = songs
  };

  playlist.name = (char*)malloc(strlen(name));
  strcpy(playlist.name, name);

  return playlist;
}
