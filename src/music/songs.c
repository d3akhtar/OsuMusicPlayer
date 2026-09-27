#include "music/songs.h"
#include "config/osu_path.h"
#include "core/hash.h"
#include "osu/osu_db.h"
#include "raylib/raylib.h"
#include "utils/path.h"
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
  char const *osuPath = get_osu_path();
  char const *songsPath = osuPath == NULL
    ? NULL
    : join_paths(osuPath, "Songs");

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
    songs[i].audioFilePath = join_paths(songsPath == NULL ? "" : songsPath, audioFilePath);
 
    strcpy((char*)songs[i].artistName, beatmap->artistName);
    strcpy((char*)songs[i].songTitle, beatmap->songTitle);

    if (osuPath == NULL) continue;

    char const * beatmapFolderPath = join_paths(songsPath, beatmap->folderName);
    char const * beatmapInformationFilePath = join_paths(beatmapFolderPath, beatmap->osuFileName);
    char const * bgFileName = extract_bg_file_name(beatmapInformationFilePath);
    if (bgFileName == NULL) {
      printf("Couldn't read bg file name for beatmap at path: %s\n", beatmap->folderName);
      free((char*)beatmapFolderPath);
      free((char*)beatmapInformationFilePath);
      continue;
    }

    char const *bgFilePath = join_paths(beatmapFolderPath, bgFileName);
    songs[i].bgFilePath = (char*)malloc(strlen(bgFilePath) + 1);
    strcpy((char*)songs[i].bgFilePath, bgFilePath);

    free((char*)beatmapFolderPath);
    free((char*)beatmapInformationFilePath);
    free((char*)bgFilePath);
  }

  free((char*)songsPath);

  printf("Loaded %d songs\n", seenSongPaths->nElems);

  return songs;
}
