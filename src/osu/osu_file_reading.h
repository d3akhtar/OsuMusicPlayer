#pragma once

#include <stdio.h>
#include <stdbool.h>

#define INT_FLOAT_PAIR_SIZE 10
#define INT_DOUBLE_PAIR_SIZE 14
#define TIMING_POINT_SIZE 17

typedef struct OsuFile {
  char const * path;
  FILE *fptr;
  char const * err;
} OsuFile;

typedef struct Beatmap {
  char const * artistName;
  char const * songTitle;
  char const * audioFileName;
	char const * mD5Hash;
	char const * osuFileName;
	int beatmapId;
	char const * songSource;
	char const * songTags;
	char const * folderName;
} Beatmap;

typedef struct Collection {
  char const * name;
  int nBeatmapHashes;
  char const ** beatmapHashes;
} Collection;

OsuFile open_osu_file(char const * path);
bool read_byte(OsuFile* file, char* value);
bool read_bytes(OsuFile* file, int n, char** value);
bool skip_bytes(OsuFile* file, int n);
bool read_short(OsuFile* file, short* value);
bool read_int(OsuFile* file, int* value);
bool read_long(OsuFile* file, long* value); 
bool read_single(OsuFile* file, float* value); 
bool read_double(OsuFile* file, double* value); 
bool read_bool(OsuFile* file, bool* value); 
bool read_uleb128(OsuFile* file, long* value); 
bool read_string(OsuFile* file, char** value); 
bool skip_string(OsuFile* file);
long tell(OsuFile* file);
void close_osu_file(OsuFile* file);
