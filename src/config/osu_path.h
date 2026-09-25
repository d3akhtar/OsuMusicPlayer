#pragma once

#include <stdbool.h>

void init_osu_path();
char const * get_osu_path();
bool validate_osu_path(char const * path);
void save_osu_path(char const *path);

