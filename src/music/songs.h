#pragma once

#include "osu/osu_file_reading.h"

typedef struct Song {
  char const * songInfo;
  char const * audioFilePath;
} Song;

Song* ExtractSongs(Beatmap* beatmaps, int nBeatmaps, int *nSongs);
Song* ExtractSongsForCollection(Collection* collection, Beatmap* beatmaps, int nBeatmaps, int *nSongs);
