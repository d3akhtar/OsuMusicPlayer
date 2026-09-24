#include "string.h"
#include <stdlib.h>
#include <string.h>

static int __get_number_of_lines(char const * str, size_t len);

char ** split_into_lines(char const * str, size_t len, int* nLines)
{
  *nLines = __get_number_of_lines(str, len);

  char ** res = malloc(*nLines * sizeof(char*));

  int resP = 0, lineLen = 0;
  for (int i = 0; i < len; i++)
  {
    char line[1 << 8];
    if (str[i] == '\n' || i == len-1) {
      line[lineLen++] = '\0';
      res[resP] = malloc(lineLen);
      memcpy(res[resP], line, lineLen);
      lineLen = 0;
      i++;
      resP++;
    }

    line[lineLen++] = str[i];
  }

  return res;
}

static int __get_number_of_lines(char const * str, size_t len)
{
  int nLines = 0;
  for (int i = 0; i < len; i++)
    if (str[i] == '\n') nLines++;

  return nLines;
}
