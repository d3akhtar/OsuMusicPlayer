#include "fft.h"
#include "music/current_playlist.h"
#include "ui/browse_song_list.h"
#include "ui/elements.h"
#include "ui/music_playback.h"
#include "ui/osu_path_dialog.h"
#include "ui/playlist_song_list.h"
#include "ui/song_info.h"
#include "ui/styles.h"
#include <math.h>
#include <raylib/raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <raylib/raygui.h>

#include <tinyfiledialogs.h>

static void __handle_input();

int main()
{
    Vector2 mousePos = {0,0};

    SetConfigFlags(FLAG_WINDOW_ALWAYS_RUN);        
    InitWindow(1280, 720, "Osu Music Player");
    SetTargetFPS(60);

    init_default_styles();
    init_osu_path_dialog();
    load_playlists();

    InitAudioDevice();
    SetAudioStreamBufferSizeDefault(AUDIO_STREAM_RING_BUFFER_SIZE);

    init_music_playback();
    init_song_info();

    set_song(current_playlist()->songs[current_playlist()->currentSong].audioFilePath);
        
    while (!WindowShouldClose())
    {
        mousePos = GetMousePosition();

        __handle_input();

        update_song_visuals();
        
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        
        BeginDrawing();
            gui_draw_music_playback();
            gui_draw_song_info();
            gui_draw_playlist_song_list();
            gui_draw_browse_song_list();
            gui_draw_osu_path_dialog();
        EndDrawing();
    }

    unload_music_playback();

    CloseAudioDevice();
    CloseWindow();

    unload_playlists();
    
    return 0;
}

static void __handle_input()
{
    if (IsKeyPressed(KEY_SPACE)) toggle_pause();
    if (IsKeyPressed(KEY_LEFT)) retreat_song(5);
    if (IsKeyPressed(KEY_RIGHT)) advance_song(5);
}
