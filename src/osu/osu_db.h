#pragma once

#include "osu_file_reading.h"

Beatmap* ReadBeatmaps(OsuFile* file, int* nBeatmaps);
Beatmap ReadBeatmap(OsuFile* file);
char const * ExtractBackgroundFileName(char const *beatmapInformationFilePath);

Collection* ReadCollections(OsuFile* file, int *nCollections);
Collection ReadCollection(OsuFile* file);
