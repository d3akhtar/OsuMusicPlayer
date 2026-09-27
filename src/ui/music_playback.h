#pragma once

#include "raylib/raylib.h"

void init_music_playback();

void set_song(char const *path);
void advance_song(unsigned int advanceAmount);
void retreat_song(unsigned int retreatAmount);
void toggle_pause();
void set_visualizer_bar_color(Color color);

void update_song_visuals();
int gui_draw_music_playback();
void unload_music_playback();
