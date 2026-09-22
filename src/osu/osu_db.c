#include "osu_db.h"
#include "osu/osu_file_reading.h"
#include "utils/string.h"
#include <_stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char const ** __read_section_from_beatmap_information_file(FILE *fptr, int *nLines);

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
  beatmap.osuFileName = ReadString(file);
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

char const * ExtractBackgroundFileName(char const *beatmapInformationFilePath)
{
  FILE* fptr = fopen(beatmapInformationFilePath, "r");

  if (fptr == NULL) {
    fprintf(stderr, "Error while opening file %s\n", beatmapInformationFilePath);
    return NULL;
  }

  while (!feof(fptr))
  {
    int nLines;
    const char ** section = __read_section_from_beatmap_information_file(fptr, &nLines);
    if (section != NULL && strstr(section[0], "[Events]") != NULL) {
      for (int i = 1; i < nLines; i++)
      {
        if (strncmp(section[i], "0,0,", 4) == 0) {
          size_t lineLen = strlen(section[i]);
          char path[lineLen];
          size_t pathLen = 0;
          for (int j = 4; j < lineLen; j++)
          {
            if (section[i][j] == '"') continue;
            if (section[i][j] == ',' || section[i][j] == '\n') break;

            path[pathLen++] = section[i][j];
          }

          if (pathLen == 0) {
            fclose(fptr);
            return NULL;
          }

          char * res = malloc(pathLen+1);
          memcpy(res, path, pathLen);
          res[pathLen] = '\0';

          fclose(fptr);
          return res;
        } 
      }

      fclose(fptr);
      return NULL;
    }
  }

  fclose(fptr);
  return NULL;
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

char const ** __read_section_from_beatmap_information_file(FILE *fptr, int *nLines)
{
  int ch;
  char buf[1 << 8];
  while ((ch = fgetc(fptr)) != EOF) {
    if (ch == '[') {
      size_t len = 0;
      buf[len++] = ch;
      while ((ch = fgetc(fptr)) != '[' && ch != EOF)
        buf[len++] = ch;
     
      fseek(fptr, -1, SEEK_CUR);

      return (char const **)SplitIntoLines(buf, len, nLines);
    }
  }

  return NULL;
}
