#include "osu_file_reading.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define read_implementation_for_simple(T) {\
  if (fread(value, sizeof(T), 1, file->fptr) != 1) { \
    fprintf(stderr, "Failed to read %s (file position %ld)\n", #T, tell(file)); \
    return false; \
  } \
  return true; \
} 

OsuFile open_osu_file(char const * path)
{
  OsuFile file;
  file.fptr = fopen(path, "rb");

  if (file.fptr == NULL) return file;
  
  file.path = path;
  return file;
}

bool read_byte(OsuFile* file, char* value) read_implementation_for_simple(char);

bool read_bytes(OsuFile* file, int n, char** value)
{
  size_t expectedNumBytesRead = n * sizeof(char);
  *value = (char*)malloc(expectedNumBytesRead);
  size_t bytesRead = fread(*value, sizeof(char), n, file->fptr);
  if (bytesRead != expectedNumBytesRead) {
    fprintf(stderr, "Failed to read %d bytes, expected to read %llu bytes, read %zu bytes", n, expectedNumBytesRead, bytesRead);
    return false;
  }

  return value;  
}

bool skip_bytes(OsuFile* file, int n)
{
  if (fseek(file->fptr, n, SEEK_CUR) != 0) {
    fprintf(stderr, "Error occurred while skipping %d bytes: %s\n", n, strerror(errno));
    return false;
  }

  return true;
}

bool read_short(OsuFile* file, short* value) read_implementation_for_simple(short);
bool read_int(OsuFile* file, int* value) read_implementation_for_simple(int);
bool read_long(OsuFile* file, long* value) read_implementation_for_simple(long);
bool read_single(OsuFile* file, float* value) read_implementation_for_simple(float);
bool read_double(OsuFile* file, double* value) read_implementation_for_simple(double);
bool read_bool(OsuFile* file, bool* value) read_implementation_for_simple(bool);

bool read_uleb128(OsuFile* file, long* value)
{
  *value = 0;
  int shift = 0;
  while (1)
  {
    char b;
    if (!read_byte(file, &b)) {
      fprintf(stderr, "Failed to read byte while reading uleb128 (shift = %d)\n", shift);
      return false;      
    }

    int isLastByte = (b & 0x80) == 0;
    *value |= (long)(b & 0x7F) << shift;
    if (isLastByte) {
      break;
    }

    shift += 7;
  }

  return true;
}

bool read_string(OsuFile* file, char** value)
{
  char prefix;
  if (!read_byte(file, &prefix)) {
      fprintf(stderr, "Failed to read prefix byte for string\n");
      return false;    
  }
  
  if (prefix == 0x00) {
    *value = "";
    return true;
  }

  if (prefix != 0x0b) {
    fprintf(stderr, "If prefix byte isn't 0x00, it should be 0x0b, instead it is: 0x%02x\n", prefix);
    return false;
  }

  long len;
  if (!read_uleb128(file, &len)) {
    fprintf(stderr, "Failed to read uleb128 for string length\n");
    return false;    
  }

  size_t expectedNumBytesRead = sizeof(char) * len;
  *value = (char*)malloc((len+1) * sizeof(char));
  size_t bytesRead = fread(*value, sizeof(char), len, file->fptr);

  if (bytesRead != expectedNumBytesRead) {
    fprintf(stderr, "Failed to read %ld bytes for string, read %zu bytes instead", len, bytesRead);
    return false;
  }

  (*value)[len] = '\0';

  return true;
}

bool skip_string(OsuFile* file)
{
  char prefix;
  if (!read_byte(file, &prefix)) {
      fprintf(stderr, "Failed to read prefix byte for string\n");
      return false;    
  }
  
  if (prefix == 0x00) return true;

  if (prefix != 0x0b) {
    fprintf(stderr, "If prefix byte isn't 0x00, it should be 0x0b, instead it is: 0x%02x\n", prefix);
    return false;
  }

  long len;
  if (!read_uleb128(file, &len)) {
    fprintf(stderr, "Failed to read uleb128 for string length\n");
    return false;    
  }

  if (!skip_bytes(file, len)) return false;

  return true;
}

long tell(OsuFile* file)
{
  return ftell(file->fptr);
}

void close_osu_file(OsuFile* file)
{
  fclose(file->fptr);  
}

