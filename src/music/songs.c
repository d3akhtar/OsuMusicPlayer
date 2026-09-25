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

    Beatmap *b = &beatmaps[i];

    if (exists(&seenSongPaths, audioFilePath) == HASH_KEY_NOT_FOUND)
      insert(&seenSongPaths, audioFilePath, &b, sizeof(Beatmap*));
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
      insert(&beatmapHashes, collection->beatmapHashes[i], "", 0);
  }
    
  HashMap seenSongPaths;
  init_hash_map(&seenSongPaths);

  for (int i = 0; i < nBeatmaps; i++)
  {
    if (exists(&beatmapHashes, beatmaps[i].mD5Hash) == HASH_KEY_NOT_FOUND) continue;

    Beatmap *b = &beatmaps[i];
    
    char const * audioFilePath = TextFormat("%s/%s", beatmaps[i].folderName, beatmaps[i].audioFileName);

    if (exists(&seenSongPaths, audioFilePath) == HASH_KEY_NOT_FOUND)
      insert(&seenSongPaths, audioFilePath, &b, sizeof(Beatmap*));
  }

  Song* songs = __extract_songs_from_seen_song_paths(&seenSongPaths);
  *nSongs = seenSongPaths.nElems;

  return songs;
}

Song* __extract_songs_from_seen_song_paths(HashMap *seenSongPaths)
{ 
  printf("Number of songs: %d\n", seenSongPaths->nElems);

  Song* songs = (Song*)malloc(sizeof(Song) * seenSongPaths->nElems);

  for (int i = 0; i < seenSongPaths->nElems; i++)
  {
    char const * audioFilePath = seenSongPaths->keys[i];
    Beatmap * beatmap;
    Beatmap ** b = &beatmap;
    size_t len;
   
    if (search(seenSongPaths, audioFilePath, ((void **)&b), &len) == HASH_KEY_NOT_FOUND) {
      fprintf(stderr, "Key: %s not found\n", audioFilePath);
      continue;
    }

    beatmap = *b;

    songs[i].artistName = (char*)malloc(strlen(beatmap->artistName) + 1);
    songs[i].songTitle = (char*)malloc(strlen(beatmap->songTitle) + 1);
    songs[i].audioFilePath = (char*)malloc(strlen(audioFilePath) + 1);
 
    strcpy((char*)songs[i].artistName, beatmap->artistName);
    strcpy((char*)songs[i].songTitle, beatmap->songTitle);
    strcpy((char*)songs[i].audioFilePath, audioFilePath);
  }

  return songs;
}
