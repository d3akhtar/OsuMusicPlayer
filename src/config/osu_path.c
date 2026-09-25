#include "osu_path.h"

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static char const * osuPathStorageFile = "./path.db";
static char* osuPath = NULL;

void init_osu_path()
{
  FILE* fptr = fopen(osuPathStorageFile, "rb");
  if (fptr == NULL) {
    printf("Osu path not saved at path %s yet\n", osuPathStorageFile);
    return;
  }

  char buf[1024];
  size_t len = 0;

  len = fread(buf, sizeof(char), 1024, fptr);

  if (ferror(fptr)) {
    fprintf(stderr, "Error while reading path file: %s\n", osuPathStorageFile);
    fclose(fptr);
    return;
  }

  osuPath = (char*)malloc(len);
  memcpy(osuPath, buf, len);

  fclose(fptr);
}

char const * get_osu_path()
{
  return osuPath;
}

bool validate_osu_path(char const * path)
{
  DIR *dir = opendir(path);

  if (dir == NULL) {
    fprintf(stderr, "Error while opening directory: %s\n", path);
    return false;
  }
  
  bool containsSongsFolder = false, containsOsuDbFile = false;
  struct dirent *ent;

  while ((ent = readdir(dir)) != NULL)
  {
    if (ent->d_type == DT_DIR && strcmp("Songs", ent->d_name) == 0) containsSongsFolder = true;
    else if (strcmp("osu!.db", ent->d_name) == 0) containsOsuDbFile = true;

    if (containsSongsFolder && containsOsuDbFile) break;
  }

  closedir(dir);
  
  return containsSongsFolder && containsOsuDbFile;
}

void save_osu_path(char const *path)
{
  if (!validate_osu_path(path)) return;
  
  FILE* fptr = fopen(osuPathStorageFile, "wb");

  if (fptr == NULL) {
    fprintf(stderr, "Error while opening path file %s for write\n", path);
    return;
  }

  size_t len = strlen(path);
  size_t wrote = fwrite(path, sizeof(char), len, fptr);
  if (wrote != len) {
    fprintf(stderr, "Error while writing %zu bytes to file (wrote %zu bytes instead)\n", len, wrote);
    fclose(fptr);
    if (remove(path) != 0) fprintf(stderr, "Failed to remove file: %s after failed write\n", path); 
    return;
  }

  fclose(fptr);
  return;
}
