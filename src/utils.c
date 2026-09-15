#include "utils.h"
#include "raylib/raylib.h"

const char * FormatTimer(int seconds)
{
  int minutes = seconds/60;
  return TextFormat("%d:%02d", minutes, seconds%60);
}

const char * FormatTimerProgress(int seconds, int totalSeconds)
{
  return TextFormat("%s/%s", FormatTimer(seconds), FormatTimer(totalSeconds));
}
