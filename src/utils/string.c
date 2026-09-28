#include "string.h"
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

static int __get_number_of_lines(char const * str, size_t len);

char ** split_into_lines(char const * str, size_t len, int* nLines)
{
  *nLines = __get_number_of_lines(str, len);

  char ** res = (char**)malloc(*nLines * sizeof(char*));

  int resP = 0, lineLen = 0;
  for (int i = 0; i < len; i++)
  {
    char line[65536];
    if (str[i] == '\n' || i == len-1) {
      line[lineLen++] = '\0';
      res[resP] = (char*)malloc(lineLen);
      memcpy(res[resP], line, lineLen);
      lineLen = 0;
      i++;
      resP++;
    }

    line[lineLen++] = str[i];
  }

  return res;
}

char const * create_list_view_text_for_playlist(Playlist* playlist)
{
  char **playlistSongNames = (char**)alloca(playlist->nSongs * sizeof(char*));
  for (int i = 0; i < playlist->nSongs; i++)
    playlistSongNames[i] = (char*)playlist->songs[i].songTitle;

  char const *res = create_list_view_text_for_string_list(playlistSongNames, playlist->nSongs);
  return res;                           
}

static int __get_number_of_lines(char const * str, size_t len)
{
  int nLines = 0;
  for (int i = 0; i < len; i++)
    if (str[i] == '\n') nLines++;

  return nLines;
}

char const * create_list_view_text_for_string_list(char** items, int nItems)
{
  size_t* savedSongLens = (size_t*)malloc(nItems * sizeof(size_t));
  size_t len = 0;
  for (int i = 0; i < nItems; i++) {
    savedSongLens[i] = strlen(items[i]);
    len += savedSongLens[i]+1;
  }

  len--;
  char *res = (char*)malloc(len);
  int resP = 0;

  for (int i = 0; i < nItems; i++) {
    memcpy(&res[resP], items[i], savedSongLens[i]);
    resP += savedSongLens[i];
    res[resP++] = ';';
  }

  free(savedSongLens);

  res[resP-1] = '\0';

  return res;
}
