#include "music/songs.h"
#include "core/hash.h"
#include "raylib/raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Song* __extract_songs_from_seen_song_paths(HashMap *seenSongPaths);

Song* extract_songs(Beatmap* beatmaps, int nBeatmaps, int *nSongs)
{
  HashMap seenSongPaths;
  init_hash_map(&seenSongPaths);

  for (int i = 0; i < nBeatmaps; i++)
  {
    char const * audioFilePath = TextFormat("%s/%s", beatmaps[i].folderName, beatmaps[i].audioFileName);

    if (exists(&seenSongPaths, audioFilePath) == HASH_KEY_NOT_FOUND) {
      char const * songInfo = TextFormat("%s - %s", beatmaps[i].artistName, beatmaps[i].songTitle);
      insert(&seenSongPaths, audioFilePath, songInfo);
    }
  }

  Song* songs = __extract_songs_from_seen_song_paths(&seenSongPaths);
  *nSongs = seenSongPaths.nElems;

  return songs;
}

Song* extract_songs_for_collection(Collection* collection, Beatmap* beatmaps, int nBeatmaps, int *nSongs)
{
  HashMap beatmapHashes;
  init_hash_map(&beatmapHashes);

  for (int i = 0; i < collection->nBeatmapHashes; i++)
  {
    if (exists(&beatmapHashes, collection->beatmapHashes[i]) == HASH_KEY_NOT_FOUND)
      insert(&beatmapHashes, collection->beatmapHashes[i], "");
  }
    
  HashMap seenSongPaths;
  init_hash_map(&seenSongPaths);

  for (int i = 0; i < nBeatmaps; i++)
  {
    if (exists(&beatmapHashes, beatmaps[i].mD5Hash) == HASH_KEY_NOT_FOUND) continue;
    
    char const * audioFilePath = TextFormat("%s/%s", beatmaps[i].folderName, beatmaps[i].audioFileName);

    if (exists(&seenSongPaths, audioFilePath) == HASH_KEY_NOT_FOUND) {
      char const * songInfo = TextFormat("%s - %s", beatmaps[i].artistName, beatmaps[i].songTitle);
      insert(&seenSongPaths, audioFilePath, songInfo);
    }
  }

  Song* songs = __extract_songs_from_seen_song_paths(&seenSongPaths);
  *nSongs = seenSongPaths.nElems;

  return songs;
}

Song* __extract_songs_from_seen_song_paths(HashMap *seenSongPaths)
{ 
  printf("Number of songs: %d\n", seenSongPaths->nElems);

  Song* songs = malloc(sizeof(Song) * seenSongPaths->nElems);

  for (int i = 0; i < seenSongPaths->nElems; i++)
  {
    char const * audioFilePath = seenSongPaths->keys[i];
    char const * songInfo;
   
    if (search(seenSongPaths, audioFilePath, &songInfo) == HASH_KEY_NOT_FOUND)
      fprintf(stderr, "Key: %s not found\n", audioFilePath);

    songs[i].songInfo = malloc(strlen(songInfo) + 1);
    songs[i].audioFilePath = malloc(strlen(audioFilePath) + 1);
 
    strcpy((char*)songs[i].songInfo, songInfo);
    strcpy((char*)songs[i].audioFilePath, audioFilePath);
  }

  return songs;
}
