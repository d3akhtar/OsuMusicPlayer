#include "format.h"
#include "raylib/raylib.h"

static const char * __format_timer(int seconds);

const char * format_timer_progress(int seconds, int totalSeconds)
{
  return TextFormat("%s/%s", __format_timer(seconds), __format_timer(totalSeconds));
}

static const char * __format_timer(int seconds)
{
  int minutes = seconds/60;
  return TextFormat("%d:%02d", minutes, seconds%60);
}
