#include "osu_db.h"
#include "osu/osu_file_reading.h"
#include "utils/string.h"
#include <_stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Beatmap __read_beatmap(OsuFile* file);
Collection __read_collection(OsuFile* file);
char const ** __read_section_from_beatmap_information_file(FILE *fptr, int *nLines);

Beatmap* read_beatmaps(OsuFile* file, int* nBeatmaps)
{
  int version = read_int(file);
  if (version < 20260000) {
    fprintf(stderr, "Version %d not supported\n", version);
    return NULL;
  }

  printf("osu! db version: %d\n", version);

  skip_bytes(file, 13);
  char const * playerName = read_string(file);
  if (playerName == NULL) {
    fprintf(stderr, "Failed to read player name\n");
    return NULL;
  }
  
  printf("Player name: %s\n", playerName);

  *nBeatmaps = read_int(file);

  printf("Reading %d beatmaps\n", *nBeatmaps);

  Beatmap* beatmaps = malloc(*nBeatmaps * sizeof(Beatmap));
  for (int i = 0; i < *nBeatmaps; i++) beatmaps[i] = __read_beatmap(file);

  return beatmaps;
}

Collection* read_collections(OsuFile* file, int* nCollections)
{
  read_int(file);
  *nCollections = read_int(file);
  Collection* collections = malloc(*nCollections * sizeof(Collection));

  for (int i = 0; i < *nCollections; i++)
  {
    collections[i] = __read_collection(file);
  }

  return collections;
}

char const * extract_bg_file_name(char const *beatmapInformationFilePath)
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

Beatmap __read_beatmap(OsuFile* file)
{
  Beatmap beatmap;

  beatmap.artistName = read_string(file);
  skip_string(file);
  beatmap.songTitle = read_string(file);
  skip_string(file);
  skip_string(file);
  skip_string(file);
  beatmap.audioFileName = read_string(file);
  beatmap.mD5Hash = read_string(file);
  beatmap.osuFileName = read_string(file);
  skip_bytes(file, 39);
  int n = read_int(file);
  skip_bytes(file, n * INT_FLOAT_PAIR_SIZE);
  n = read_int(file);
  skip_bytes(file, n * INT_FLOAT_PAIR_SIZE);
  n = read_int(file);
  skip_bytes(file, n * INT_FLOAT_PAIR_SIZE);
  n = read_int(file);
  skip_bytes(file, n * INT_FLOAT_PAIR_SIZE);
  skip_bytes(file, 12);
  n = read_int(file);
  skip_bytes(file, n * TIMING_POINT_SIZE);
  read_int(file);
  beatmap.beatmapId = read_int(file);
  skip_bytes(file, 15);
  beatmap.songSource = read_string(file);
  beatmap.songTags = read_string(file);
  read_short(file);
  skip_string(file);
  skip_bytes(file, 10);
  beatmap.folderName = read_string(file);
  skip_bytes(file, 18);    

  return beatmap;
}

Collection __read_collection(OsuFile* file)
{
  Collection collection;
  collection.name = read_string(file);
  collection.nBeatmapHashes = read_int(file);
  collection.beatmapHashes = malloc(collection.nBeatmapHashes * sizeof(char*));
  for (int i = 0; i < collection.nBeatmapHashes; i++)
  {
    char const * hash = read_string(file);
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

      return (char const **)split_into_lines(buf, len, nLines);
    }
  }

  return NULL;
}
