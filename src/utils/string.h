#pragma once

#include "music/playlist.h"
#include <stddef.h>

char ** split_into_lines(char const * str, size_t len, int *nLines);
char const * create_list_view_text_for_string_list(char** items, int nItems);
char const * create_list_view_text_for_playlist(Playlist* playlist);
