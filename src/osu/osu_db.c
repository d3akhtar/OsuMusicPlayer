#include "osu_db.h"
#include "osu/osu_file_reading.h"
#include "raylib/raylib.h"
#include "utils/string.h"
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char const * __fmt_err_msg(char const * msg);
bool __read_beatmap(OsuFile* file, Beatmap* beatmap);
bool __read_collection(OsuFile* file, Collection* collection);
char const ** __read_section_from_beatmap_information_file(FILE *fptr, int *nLines);

Beatmap* read_beatmaps(OsuFile* file, int* nBeatmaps)
{
  int version;
  if (!read_int(file, &version)) {
    file->err = __fmt_err_msg("Failed to read version");
    return NULL;
  }
  
  if (version < 20260000) {
    file->err = __fmt_err_msg(TextFormat("Version %d not supported", version));
    return NULL;
  }

  printf("osu! db version: %d\n", version);

  if (!skip_bytes(file, 13)) {
    file->err = __fmt_err_msg("Failed to skip to player name");
    return NULL;    
  }
  
  char *playerName;
  if (!read_string(file, &playerName)) {
    file->err = __fmt_err_msg("Failed to read player name");
    return NULL;    
  }
  
  printf("Player name: %s\n", playerName);

  if (!read_int(file, nBeatmaps)) {
    fprintf(stderr, "Failed to read number of beatmaps\n");
    return NULL;    
  }

  printf("Reading %d beatmaps\n", *nBeatmaps);

  Beatmap* beatmaps = (Beatmap*)malloc(*nBeatmaps * sizeof(Beatmap));
  for (int i = 0; i < *nBeatmaps; i++) {
    if (!__read_beatmap(file, &beatmaps[i])) {
      fprintf(stderr, "(index: %d) error while reading beatmap: %s\n", i, file->err);
      return NULL;
    }
  }

  return beatmaps;
}

Collection* read_collections(OsuFile* file, int* nCollections)
{
  if (!skip_bytes(file, sizeof(int))) {
    file->err = __fmt_err_msg("Failed to skip to collections");
    return NULL;    
  }
  
  if (!read_int(file, nCollections)) {
    file->err = __fmt_err_msg("Failed to read number of collections");
    return NULL;
  }

  Collection* collections = (Collection*)malloc(*nCollections * sizeof(Collection));

  for (int i = 0; i < *nCollections; i++) {
    if (!__read_collection(file, &collections[i])) {
      fprintf(stderr, "(index: %d) error while reading collection\n", i);
      return NULL;      
    }
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
          size_t const lineLen = strlen(section[i]);
          char *path = (char*)alloca(lineLen);
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

          char* res = (char*)malloc(pathLen+1);
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

void free_beatmaps(Beatmap *beatmaps, int nBeatmaps)
{
  for (int i = 0; i < nBeatmaps; i++)
  {
    free((char*)beatmaps[i].artistName);
    free((char*)beatmaps[i].songTitle);
    free((char*)beatmaps[i].audioFileName);
    free((char*)beatmaps[i].mD5Hash);
    free((char*)beatmaps[i].osuFileName);
    free((char*)beatmaps[i].songSource);
    free((char*)beatmaps[i].songTags);
    free((char*)beatmaps[i].folderName);
  }

  free(beatmaps);
}

void free_collections(Collection *collections, int nCollections)
{
  for (int i = 0; i < nCollections; i++)
  {
    free((char*)collections[i].name);
    for (int j = 0; j < collections[i].nBeatmapHashes; j++)
      free((char*)collections->beatmapHashes[j]);
  }

  free(collections);  
}

char const * __fmt_err_msg(char const * msg)
{
  return TextFormat("osu file read error: %s\n", msg);
}

bool __read_beatmap(OsuFile* file, Beatmap* beatmap)
{
  if (!read_string(file, (char**)&beatmap->artistName)) {
    file->err = __fmt_err_msg("Failed to read beatmap artist name");
    return false;
  }
  
  if (!skip_string(file)) {
    file->err = __fmt_err_msg("Failed to skip string");
    return false;
  }
  
  if (!read_string(file, (char**)&beatmap->songTitle)) {
    file->err = __fmt_err_msg("Failed to read beatmap song title");
    return false;
  }
  
  if (!skip_string(file)) {
    file->err = __fmt_err_msg("Failed to skip string");
    return false;
  }
  
  if (!skip_string(file)) {
    file->err = __fmt_err_msg("Failed to skip string");
    return false;
  }
  
  if (!skip_string(file)) {
    file->err = __fmt_err_msg("Failed to skip string");
    return false;
  }
  
  if (!read_string(file, (char**)&beatmap->audioFileName)) {
    file->err = __fmt_err_msg("Failed to read beatmap audio file name");
    return false;
  }
  
  if (!read_string(file, (char**)&beatmap->mD5Hash)) {
    file->err = __fmt_err_msg("Failed to read beatmap md5 hash");
    return false;
  }
  
  if (!read_string(file, (char**)&beatmap->osuFileName)) {
    file->err = __fmt_err_msg("Failed to read beatmap osu file name");
    return false;
  }
  
  if (!skip_bytes(file, 39)) {
    file->err = __fmt_err_msg("Failed to skip 39 bytes");
    return false;
  }
  
  int n;
  if (!read_int(file, &n)) {
    file->err = __fmt_err_msg("Failed to read numbter of int float pairs");
    return false;
  }

  if (!skip_bytes(file, n * INT_FLOAT_PAIR_SIZE)) {
    file->err = __fmt_err_msg(TextFormat("Failed to skip %d bytes", n * INT_FLOAT_PAIR_SIZE));
    return false;
  }
  
  if (!read_int(file, &n)) {
    file->err = __fmt_err_msg("Failed to read numbter of int float pairs");
    return false;
  }
  
  if (!skip_bytes(file, n * INT_FLOAT_PAIR_SIZE)) {
    file->err = __fmt_err_msg(TextFormat("Failed to skip %d bytes", n * INT_FLOAT_PAIR_SIZE));
    return false;
  }
  
  if (!read_int(file, &n)) {
    file->err = __fmt_err_msg("Failed to read numbter of int float pairs");
    return false;
  }
  
  if (!skip_bytes(file, n * INT_FLOAT_PAIR_SIZE)) {
    file->err = __fmt_err_msg(TextFormat("Failed to skip %d bytes", n * INT_FLOAT_PAIR_SIZE));
    return false;
  }
  
  if (!read_int(file, &n)) {
    file->err = __fmt_err_msg("Failed to read numbter of int float pairs");
    return false;
  }
  
  if (!skip_bytes(file, n * INT_FLOAT_PAIR_SIZE)) {
    file->err = __fmt_err_msg(TextFormat("Failed to skip %d bytes", n * INT_FLOAT_PAIR_SIZE));
    return false;
  }
  
  if (!skip_bytes(file, 12)) {
    file->err = __fmt_err_msg("Failed to skip 12 bytes");
    return false;
  }
  
  if (!read_int(file, &n)) {
    file->err = __fmt_err_msg("Failed to read numbter of int float pairs");
    return false;
  }
  
  if (!skip_bytes(file, n * TIMING_POINT_SIZE)) {
    file->err = __fmt_err_msg(TextFormat("Failed to skip %d bytes", n * TIMING_POINT_SIZE));
    return false;
  }
  
  if (!skip_bytes(file, sizeof(int))) {
    file->err = __fmt_err_msg(TextFormat("Failed to skip %d bytes", sizeof(int)));
    return false;
  }
  
  if (!read_int(file, &beatmap->beatmapId)) {
    file->err = __fmt_err_msg("Failed to read beatmap id");
    return false;  
  }
  
  if (!skip_bytes(file, 15)) {
    file->err = __fmt_err_msg(TextFormat("Failed to skip %d bytes", n * INT_FLOAT_PAIR_SIZE));
    return false;
  }
  
  if (!read_string(file, (char**)&beatmap->songSource)) {
    file->err = __fmt_err_msg("Failed to read beatmap song source");
    return false;
  }
  
  if (!read_string(file, (char**)&beatmap->songTags)) {
    file->err = __fmt_err_msg("Failed to read beatmap song tags");
    return false;
  }
  
  if (!skip_bytes(file, sizeof(short))) {
    file->err = __fmt_err_msg(TextFormat("Failed to skip %d bytes", n * INT_FLOAT_PAIR_SIZE));
    return false;
  }
  
  if (!skip_string(file)) {
    file->err = __fmt_err_msg("Failed to skip string");
    return false;
  }
  
  if (!skip_bytes(file, 10)) {
    file->err = __fmt_err_msg(TextFormat("Failed to skip %d bytes", n * INT_FLOAT_PAIR_SIZE));
    return false;
  }
  
  if (!read_string(file, (char**)&beatmap->folderName)) {
    file->err = __fmt_err_msg("Failed to read beatmap folder name");
    return false;
  }
  
  if (!skip_bytes(file, 18)) {
    file->err = __fmt_err_msg("Failed to skip 18 bytes");
    return false;
  }

  return true;
}

bool __read_collection(OsuFile* file, Collection *collection)
{
  if (!read_string(file, (char**)&collection->name)) {
    file->err = __fmt_err_msg("Failed to read collection name");
    return false;        
  }

  if (!read_int(file, &collection->nBeatmapHashes)) {
    file->err = __fmt_err_msg("Failed to read collection number of beatmap hashes");
    return false;     
  }

  collection->beatmapHashes = (char const **)malloc(collection->nBeatmapHashes * sizeof(char*));
  for (int i = 0; i < collection->nBeatmapHashes; i++)
  {
    if (!read_string(file, (char**)&collection->beatmapHashes[i])) {
      file->err = __fmt_err_msg(TextFormat("(index %d) Failed to read beatmap hash", i));
      return false;
    }
  }

  return true;
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
