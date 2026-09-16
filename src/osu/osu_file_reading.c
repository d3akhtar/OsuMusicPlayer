#include "osu_file_reading.h"

OsuFile OpenOsuFile(char const * path)
{
  OsuFile file;
  fopen_s(&file.fptr, path, "rb");

  if (file.fptr == NULL) return file;
  
  file.path = path;
  return file;
}

char ReadByte(OsuFile* file)
{
  char res;
  fread(&res, sizeof(char), 1, file->fptr);
  return res;
}

char* ReadBytes(OsuFile* file, int n)
{
  char* res;
  fread(&res, sizeof(char), n, file->fptr);
  return res;  
}

short ReadShort(OsuFile* file)
{
  short res;
  fread(&res, sizeof(short), 1, file->fptr);
  return res;
}

int ReadInt(OsuFile* file)
{
  int res;
  fread(&res, sizeof(int), 1, file->fptr);
  return res;  
}

long ReadLong(OsuFile* file)
{
  long res;
  fread(&res, sizeof(long), 1, file->fptr);
  return res;    
}

float ReadSingle(OsuFile* file)
{
  float res;
  fread(&res, sizeof(float), 1, file->fptr);
  return res;  
}

double ReadDouble(OsuFile* file)
{
  double res;
  fread(&res, sizeof(double), 1, file->fptr);
  return res;    
}

bool ReadBool(OsuFile* file)
{
  bool res;
  fread(&res, sizeof(bool), 1, file->fptr);
  return res;      
}

long ReadULEB128(OsuFile* file)
{
  long res = 0;
  int shift = 0;
  while (true)
  {
    char b = ReadByte(file);
    bool isLastByte = (b & 0x80) == 0;
    res |= (long)(b & 0x7F) << shift;
    if (isLastByte) {
      break;
    }

    shift += 7;
  }

  return res;
}

char* ReadString(OsuFile* file)
{
  char prefix = ReadByte(file);
  if (prefix == 0x00 || prefix != 0x11) {
    return NULL;
  }

  long len = ReadULEB128(file);
  char* res = ReadBytes(file, len);

  return res;
}

long Tell(OsuFile* file)
{
  return ftell(file->fptr);
}

void CloseOsuFile(OsuFile* file)
{
  fclose(file->fptr);  
}

