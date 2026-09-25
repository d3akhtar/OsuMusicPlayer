#include "playlist.h"
#include <stdlib.h>
#include <string.h>

int compare(const void *a, const void *b) {
    int artistComp = strcmp(((Song*)a)->artistName, (((Song*)b)->artistName));
    return artistComp == 0
      ? artistComp
      : strcmp(((Song*)a)->songTitle, (((Song*)b)->songTitle));
}

Playlist create_playlist_for_songs(Song* songs, int nSongs)
{
  qsort(songs, nSongs, sizeof(Song), compare);

  Playlist playlist = {
    .currentSong = 0,
    .nSongs = nSongs,
    .songs = songs
  };

  return playlist;
}
