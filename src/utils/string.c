#include "string.h"
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
    char line[1 << 8];
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
  size_t* savedSongLens = (size_t*)malloc(playlist->nSongs * sizeof(size_t));
  size_t len = 0;
  for (int i = 0; i < playlist->nSongs; i++) {
    savedSongLens[i] = strlen(playlist->songs[i].songTitle);
    len += savedSongLens[i]+1;
  }

  len--;
  char * res = (char*)malloc(len);
  int resP = 0;
  
  for (int i = 0; i < playlist->nSongs; i++) {
    memcpy(&res[resP], playlist->songs[i].songTitle, savedSongLens[i]);
    resP += savedSongLens[i];
    res[resP++] = ';';
  }

  free(savedSongLens);

  res[resP-1] = '\0';

  return res;
}

static int __get_number_of_lines(char const * str, size_t len)
{
  int nLines = 0;
  for (int i = 0; i < len; i++)
    if (str[i] == '\n') nLines++;

  return nLines;
}
