#pragma once

#include "osu_file_reading.h"

Beatmap* read_beatmaps(OsuFile* file, int* nBeatmaps);
Collection* read_collections(OsuFile* file, int *nCollections);
char const * extract_bg_file_name(char const *beatmapInformationFilePath);
