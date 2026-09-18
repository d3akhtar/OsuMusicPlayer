#pragma once

#include "osu/osu_file_reading.h"

typedef struct Song {
  char const * audioFilePath;
  char const * name;
  char const * artist;
  int beatmapId;
  char const ** beatmapHashes;
} Song;

Song* ExtractSongs(Beatmap* beatmaps);
