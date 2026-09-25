#pragma once

#include "osu/osu_file_reading.h"

typedef struct Song {
  char const * artistName;
  char const * songTitle;
  char const * audioFilePath;
} Song;

Song* extract_songs(Beatmap* beatmaps, int nBeatmaps, int *nSongs);
Song* extract_songs_for_collection(Collection* collection, Beatmap* beatmaps, int nBeatmaps, int *nSongs);
