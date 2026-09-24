#include "osu_file_reading.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

OsuFile open_osu_file(char const * path)
{
  OsuFile file;
  file.fptr = fopen(path, "rb");

  if (file.fptr == NULL) return file;
  
  file.path = path;
  return file;
}

char read_byte(OsuFile* file)
{
  char res;
  fread(&res, sizeof(char), 1, file->fptr);
  return res;
}

char* read_bytes(OsuFile* file, int n)
{
  char* res = malloc(n * sizeof(char));
  fread(res, sizeof(char), n, file->fptr);
  return res;  
}

void skip_bytes(OsuFile* file, int n)
{
  if (fseek(file->fptr, n, SEEK_CUR) != 0) {
    fprintf(stderr, "Error occurred while seeking\n");
  }
}

short read_short(OsuFile* file)
{
  short res;
  fread(&res, sizeof(short), 1, file->fptr);
  return res;
}

int read_int(OsuFile* file)
{
  int res;
  fread(&res, sizeof(int), 1, file->fptr);
  return res;  
}

long read_long(OsuFile* file)
{
  long res;
  fread(&res, sizeof(long), 1, file->fptr);
  return res;    
}

float read_single(OsuFile* file)
{
  float res;
  fread(&res, sizeof(float), 1, file->fptr);
  return res;  
}

double read_double(OsuFile* file)
{
  double res;
  fread(&res, sizeof(double), 1, file->fptr);
  return res;    
}

int read_bool(OsuFile* file)
{
  char res;
  fread(&res, sizeof(char), 1, file->fptr);
  return res;      
}

long read_uleb128(OsuFile* file)
{
  long res = 0;
  int shift = 0;
  while (1)
  {
    char b = read_byte(file);
    int isLastByte = (b & 0x80) == 0;
    res |= (long)(b & 0x7F) << shift;
    if (isLastByte) {
      break;
    }

    shift += 7;
  }

  return res;
}

char* read_string(OsuFile* file)
{
  char prefix = read_byte(file);
  if (prefix == 0x00) {
    return "";
  }

  if (prefix != 0x0b) {
    fprintf(stderr, "If prefix byte isn't 0x00, it should be 0x0b, instead it is: 0x%02x\n", prefix);
    return NULL;
  }

  long len = read_uleb128(file);

  char* res = malloc((len+1) * sizeof(char));
  fread(res, sizeof(char), len, file->fptr);
  res[len] = '\0';

  return res;
}

void skip_string(OsuFile* file)
{
  char prefix = read_byte(file);
  if (prefix == 0x00 || prefix != 0x0b) {
    return;
  }

  long len = read_uleb128(file);
  skip_bytes(file, len);
}

long tell(OsuFile* file)
{
  return ftell(file->fptr);
}

void close_osu_file(OsuFile* file)
{
  fclose(file->fptr);  
}

