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
	int beatmapId;
	char const * songSource;
	char const * songTags;
	char const * folderName;
} Beatmap;

typedef struct Collection {
  char const * name;
  char const ** beatmapHashes;
} Collection;

OsuFile OpenOsuFile(char const * path);
char ReadByte(OsuFile* file);
char* ReadBytes(OsuFile* file, int n);
short ReadShort(OsuFile* file);
int ReadInt(OsuFile* file);
long ReadLong(OsuFile* file); 
float ReadSingle(OsuFile* file); 
double ReadDouble(OsuFile* file); 
int ReadBool(OsuFile* file); 
long ReadULEB128(OsuFile* file); 
char* ReadString(OsuFile* file); 
long Tell(OsuFile* file);
void CloseOsuFile(OsuFile* file);
