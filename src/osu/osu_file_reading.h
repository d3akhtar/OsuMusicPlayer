#pragma once

#include <stdio.h>

#define INT_FLOAT_PAIR_SIZE 10
#define INT_DOUBLE_PAIR_SIZE 14
#define TIMING_POINT_SIZE 17

typedef struct OsuFile {
  char const * path;
  FILE *fptr;
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
char read_byte(OsuFile* file);
char* read_bytes(OsuFile* file, int n);
void skip_bytes(OsuFile* file, int n);
short read_short(OsuFile* file);
int read_int(OsuFile* file);
long read_long(OsuFile* file); 
float read_single(OsuFile* file); 
double read_double(OsuFile* file); 
int read_bool(OsuFile* file); 
long read_uleb128(OsuFile* file); 
char* read_string(OsuFile* file); 
void skip_string(OsuFile* file);
long tell(OsuFile* file);
void close_osu_file(OsuFile* file);
