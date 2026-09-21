#include "osu_db.h"
#include "osu/osu_file_reading.h"
#include <stdio.h>
#include <stdlib.h>

Beatmap* ReadBeatmaps(OsuFile* file, int* nBeatmaps)
{
  int version = ReadInt(file);
  if (version < 20260000) {
    fprintf(stderr, "Version %d not supported\n", version);
    return NULL;
  }

  printf("osu! db version: %d\n", version);

  SkipBytes(file, 13);
  char const * playerName = ReadString(file);
  if (playerName == NULL) {
    fprintf(stderr, "Failed to read player name\n");
    return NULL;
  }
  
  printf("Player name: %s\n", playerName);

  *nBeatmaps = ReadInt(file);

  printf("Reading %d beatmaps\n", *nBeatmaps);

  Beatmap* beatmaps = malloc(*nBeatmaps * sizeof(Beatmap));

  for (int i = 0; i < *nBeatmaps; i++)
  {
    beatmaps[i] = ReadBeatmap(file);
  }

  return beatmaps;
}

Beatmap ReadBeatmap(OsuFile* file)
{
  Beatmap beatmap;

  beatmap.artistName = ReadString(file);
  SkipString(file);
  beatmap.songTitle = ReadString(file);
  SkipString(file);
  SkipString(file);
  SkipString(file);
  beatmap.audioFileName = ReadString(file);
  beatmap.mD5Hash = ReadString(file);
  SkipString(file);
  SkipBytes(file, 39);
  int n = ReadInt(file);
  SkipBytes(file, n * INT_FLOAT_PAIR_SIZE);
  n = ReadInt(file);
  SkipBytes(file, n * INT_FLOAT_PAIR_SIZE);
  n = ReadInt(file);
  SkipBytes(file, n * INT_FLOAT_PAIR_SIZE);
  n = ReadInt(file);
  SkipBytes(file, n * INT_FLOAT_PAIR_SIZE);
  SkipBytes(file, 12);
  n = ReadInt(file);
  SkipBytes(file, n * TIMING_POINT_SIZE);
  ReadInt(file);
  beatmap.beatmapId = ReadInt(file);
  SkipBytes(file, 15);
  beatmap.songSource = ReadString(file);
  beatmap.songTags = ReadString(file);
  ReadShort(file);
  SkipString(file);
  SkipBytes(file, 10);
  beatmap.folderName = ReadString(file);
  SkipBytes(file, 18);    

  return beatmap;
}

Collection* ReadCollections(OsuFile* file, int* nCollections)
{
  ReadInt(file);
  *nCollections = ReadInt(file);
  Collection* collections = malloc(*nCollections * sizeof(Collection));

  for (int i = 0; i < *nCollections; i++)
  {
    collections[i] = ReadCollection(file);
  }

  return collections;
}

Collection ReadCollection(OsuFile* file)
{
  Collection collection;
  collection.name = ReadString(file);
  collection.nBeatmapHashes = ReadInt(file);
  collection.beatmapHashes = malloc(collection.nBeatmapHashes * sizeof(char*));
  for (int i = 0; i < collection.nBeatmapHashes; i++)
  {
    char const * hash = ReadString(file);
    collection.beatmapHashes[i] = hash;
  }

  return collection;
}
