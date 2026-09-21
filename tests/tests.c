#include "osu/osu_db.h"
#include <osu/osu_file_reading.h>
#include <stdio.h>
#include <stdlib.h>

void PrintBeatmap(Beatmap* beatmap);
void PrintCollection(Collection* collection);

int TestReadingOsuDb(char const *path);
int TestReadingOsuCollections(char const *path);

int main()
{
  printf("Starting tests...\n");
  
  char const * osuDbPath = "./tests/data/osu!.db";
  char const * collectionsPath = "./tests/data/collection.db";

  if (!TestReadingOsuDb(osuDbPath))
  {
    fprintf(stderr, "TestReadingOsuDb failed\n");
    exit(-1);
  }

  if (!TestReadingOsuCollections(collectionsPath))
  {
    fprintf(stderr, "TestReadingOsuCollections failed\n");
    exit(-1);
  }
}

void PrintBeatmap(Beatmap* beatmap)
{
  printf("[%d] %s - %s => Song location: %s/%s\n", beatmap->beatmapId, beatmap->artistName, beatmap->songTitle, beatmap->folderName, beatmap->audioFileName);
}

void PrintCollection(Collection* collection)
{
  printf("%s - %d maps\n", collection->name, collection->nBeatmapHashes);
}

int TestReadingOsuDb(char const *path)
{
  printf("== TestReadingOsuDb(%s) ==\n", path);
  
  OsuFile file = OpenOsuFile(path);
  if (file.fptr == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", path);
    return 0;
  }

  int nBeatmaps;
  Beatmap* beatmaps = ReadBeatmaps(&file, &nBeatmaps);

  printf("Read %d beatmaps, displaying first 20\n", nBeatmaps);

  for (int i = 0; i < 20; i++)
  {
    PrintBeatmap(&beatmaps[i]);
  }

  CloseOsuFile(&file);

  return 1;
}

int TestReadingOsuCollections(char const *path)
{
  printf("== TestReadingOsuCollections(%s) ==\n", path);

  OsuFile file = OpenOsuFile(path);
  if (file.fptr == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", path);
    return 0;
  }

  int nCollections;
  Collection* collections = ReadCollections(&file, &nCollections);

  printf("Read %d collections, displaying first 20\n", nCollections);

  for (int i = 0; i < 20; i++)
  {
    PrintCollection(&collections[i]);
  }

  CloseOsuFile(&file);

  return 1;  
}
