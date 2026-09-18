#include "osu_db.h"
#include "osu/osu_file_reading.h"
#include <stdio.h>
#include <stdlib.h>

Beatmap* ReadBeatmaps(OsuFile* file)
{
  int version = ReadInt(file);
  if (version < 20260000) {
    printf("Version %d not supported", version);
    return NULL;
  }

  ReadBytes(file, 13);
  ReadString(file);

  int nBeatmaps = ReadInt(file);

  Beatmap* beatmaps = malloc(nBeatmaps * sizeof(Beatmap));

  for (int i = 0; i < nBeatmaps; i++)
  {
    beatmaps[i] = ReadBeatmap(file);
  }

  return beatmaps;
}

Beatmap ReadBeatmap(OsuFile* file)
{
  Beatmap beatmap;

  ReadString(file); // skip artist name
  beatmap.artistName = ReadString(file);
  ReadString(file); // skip song title
  beatmap.songTitle = ReadString(file);
  ReadString(file);
  ReadString(file);
  beatmap.audioFileName = ReadString(file);
  beatmap.mD5Hash = ReadString(file);
  ReadString(file);
  ReadBytes(file, 39);
  int n = ReadInt(file);
  ReadBytes(file, n * INT_FLOAT_PAIR_SIZE);
  n = ReadInt(file);
  ReadBytes(file, n * INT_FLOAT_PAIR_SIZE);
  n = ReadInt(file);
  ReadBytes(file, n * INT_FLOAT_PAIR_SIZE);
  n = ReadInt(file);
  ReadBytes(file, n * INT_FLOAT_PAIR_SIZE);
  ReadBytes(file, 12);
  n = ReadInt(file);
  ReadBytes(file, n * TIMING_POINT_SIZE);
  ReadInt(file);
  beatmap.beatmapId = ReadInt(file);
  ReadBytes(file, 15);
  beatmap.songSource = ReadString(file);
  beatmap.songTags = ReadString(file);
  ReadShort(file);
  ReadString(file);
  ReadBytes(file, 10);
  beatmap.folderName = ReadString(file);
  ReadBytes(file, 18);    

  return beatmap;
}

Collection* ReadCollections(OsuFile* file)
{
  ReadInt(file);
  int nCollections = ReadInt(file);
  Collection* collections = malloc(nCollections * sizeof(Collection));

  for (int i = 0; i < nCollections; i++)
  {
    collections[i] = ReadCollection(file);
  }

  return collections;
}

Collection ReadCollection(OsuFile* file)
{
  Collection collection;
  collection.name = ReadString(file);
  int nHashes = ReadInt(file);
  collection.beatmapHashes = malloc(nHashes * sizeof(char*));
  for (int i = 0; i < nHashes; i++)
  {
    collection.beatmapHashes[i] = ReadString(file);
  }

  return collection;
}
