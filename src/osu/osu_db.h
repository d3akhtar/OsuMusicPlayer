#pragma once

#include "osu_file_reading.h"

Beatmap* ReadBeatmaps(OsuFile* file);
Beatmap ReadBeatmap(OsuFile* file);

Collection* ReadCollections(OsuFile* file);
Collection ReadCollection(OsuFile* file);
