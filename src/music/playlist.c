#include "playlist.h"
#include <stdlib.h>
#include <string.h>

Playlist* create_playlist_for_songs(Song* songs, int nSongs, char const *name)
{
  Playlist *playlist = (Playlist*)malloc(sizeof(Playlist));
  playlist->currentSong = 0;
  playlist->nSongs = nSongs;
  playlist->songs = songs;

  playlist->name = (char*)malloc(strlen(name));
  strcpy(playlist->name, name);

  return playlist;
}

Song* playlist_current_song(Playlist *playlist)
{
  return &playlist->songs[playlist->currentSong];
}

void free_playlist(Playlist *playlist)
{
  for (int i = 0; i < playlist->nSongs; i++)
  {
    Song *s = &(playlist->songs[i]);
    free((char*)s->songTitle);
    free((char*)s->artistName);
    free((char*)s->audioFilePath);
    free((char*)s->bgFilePath);
  }

  free(playlist->songs);
  free(playlist->name);
  free(playlist);
}
