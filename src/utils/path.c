#include "path.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#if defined (_WIN32) || defined(__WIN32__) || defined(WIN32)
  #define DIR_SEP '\\'
#else
  #define DIR_SEP '/'
#endif

char* join_paths(char const* a, char const* b)
{
  // If a ends with a slash, ignore it
  size_t aLen = strlen(a);
  if (a[aLen-1] == '\\' || a[aLen-1] == '/') aLen--;

  size_t bLen = strlen(b);
  size_t totalLen = aLen + bLen + 1; // + 1 for DIR_SEP

  char *res = (char*)malloc(totalLen);

  memcpy(res, a, aLen);
  res[aLen] = DIR_SEP;
  memcpy(&res[aLen+1], b, bLen);

  for (int i = 0; i < totalLen; i++)
    if (res[i] == '\\' || res[i] == '/') res[i] = DIR_SEP; // Ensure all slashes are represented properly according to their OS

  return res;
}
