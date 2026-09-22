#include "playlist.h"
#include <stdlib.h>
#include <string.h>

int compare(const void *a, const void *b) {
    return strcmp(((Song*)a)->songInfo, (((Song*)b)->songInfo));
}

Playlist CreatePlaylistForSongs(Song* songs, int nSongs)
{
  qsort(songs, nSongs, sizeof(Song), compare);

  Playlist playlist = {
    .currentSong = 0,
    .nSongs = nSongs,
    .songs = songs
  };

  return playlist;
}
