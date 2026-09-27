#pragma once

#include <stdbool.h>

#define OSU_DB_FILE_NAME "osu!.db"
#define OSU_COLLECTIONS_FILE_NAME "collection.db"

void init_osu_path();
char const * get_osu_path();
char const * get_osu_db_path();
char const * get_osu_collections_path();
bool validate_osu_path(char const * path);
void save_osu_path(char const *path);

