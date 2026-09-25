#include "fft.h"
#include "ui/elements.h"
#include "ui/music_playback.h"
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
        
    InitWindow(1280, 720, "Osu Music Player");
    SetTargetFPS(60);

    init_default_styles();
    init_song_info();

    InitAudioDevice();
    SetAudioStreamBufferSizeDefault(AUDIO_STREAM_RING_BUFFER_SIZE);

    init_music_playback();
    set_song("resources/music/testSong.mp3");

    int currentPlaylistScrollIndex = 0, currentPlaylistActive = 1;

    bool showBrowseSongList = false;
    bool searchBrowseSongListEdit = false;
    int const searchBrowseSongListQueryMaxSize = 1024;
    char searchBrowseSongList[1024] = {'\0'};

    int collectionDropdownOption = 0;
    bool collectionDropdownIsSelecting = false;

    int songListIndex = 0;
    int songListActive = 0;

    char const * osuPath = "{SELECT OSU! PATH}";
    char const * errorMessage = "";
    bool showSelectOsuPathDialog = false;
    
    Texture2D placeholderTexture = LoadTexture("./resources/pspace.PNG");

    while (!WindowShouldClose())
    {
        mousePos = GetMousePosition();

        __handle_input();

        update_song_visuals();
        
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        
        BeginDrawing();
            gui_draw_music_playback();

            gui_draw_song_info();

            DrawText("Playlist: tnshi songs", 920, 420, 20, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
            GuiSetStyle(LISTVIEW, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
            GuiSetStyle(LISTVIEW, TEXT_PADDING, 10);
            GuiSetStyle(LISTVIEW, BORDER_WIDTH, 2);
            GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_NORMAL, 1);
            GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_WIDTH, 1);
            GuiListView((Rectangle) {920, 450, 350, 200}, "song 1; song 2; song 3; song 4; song 5; song 6; song 7; song 8", &currentPlaylistScrollIndex, &currentPlaylistActive);
            GuiSetStyle(LISTVIEW, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
            GuiSetStyle(LISTVIEW, TEXT_PADDING, 0);
            GuiSetStyle(LISTVIEW, BORDER_WIDTH, 2);
            GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_NORMAL, 1);
            GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_WIDTH, 1);

            if (GuiButton((Rectangle) {920, 660, 350, 40}, "Browse")) showBrowseSongList = true;

            if (showBrowseSongList) {
                if (GuiWindowBox((Rectangle) {340, 10, 600, 700 }, "Browse songs") == RESULT_PRESSED) {
                    showBrowseSongList = false;
                }
                
                if (GuiTextBox((Rectangle) {340, 34, 450, 30}, searchBrowseSongList, searchBrowseSongListQueryMaxSize, searchBrowseSongListEdit)) {
                    searchBrowseSongList[0] = '\0';
                    searchBrowseSongListEdit = !searchBrowseSongListEdit;
                }

                if (!searchBrowseSongListEdit && strlen(searchBrowseSongList) == 0) {
                    DrawText("Search songs...", 345, 40, 18, BLACK);
                }

                GuiSetStyle(LISTVIEW, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
                GuiSetStyle(LISTVIEW, TEXT_PADDING, 10);
                GuiSetStyle(LISTVIEW, BORDER_WIDTH, 2);
                GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_NORMAL, 1);
                GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_WIDTH, 1);
                GuiListView((Rectangle) {340, 60, 600, 656}, "Song 1;Song 2;Song 3;Song 4;Song 5", &songListIndex, &songListActive);
                GuiSetStyle(LISTVIEW, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
                GuiSetStyle(LISTVIEW, TEXT_PADDING, 0);
                GuiSetStyle(LISTVIEW, BORDER_WIDTH, 0);
                GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_NORMAL, 0);
                GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_WIDTH, 0);

                GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
                GuiSetStyle(DROPDOWNBOX, TEXT_PADDING, 10);
                switch (GuiDropdownBox((Rectangle) {790, 34, 150, 28}, "All; Collection 1; Collection 2; Collection 3333333333333", &collectionDropdownOption, collectionDropdownIsSelecting)) {
                    case RESULT_CHANGED:
                        collectionDropdownIsSelecting = false;
                        break;
                    case RESULT_PRESSED:
                        collectionDropdownIsSelecting = true;
                        break;
                }
                GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
                GuiSetStyle(DROPDOWNBOX, TEXT_PADDING, 0);
            }

            if (showSelectOsuPathDialog) {
                GuiPanel((Rectangle) {140, 230, 1000, 200}, "Select osu! Path");
                DrawText(errorMessage, 540, 263, 18, RED);
                DrawText("Path:", 180, 290, 60, BLACK);
                DrawRectangleLines(350, 290, 650, 60, BLACK);
                DrawText(osuPath, 360, 310, 20, (Color){133, 90, 129, 255});

                if (GuiButton((Rectangle) {1010, 290, 90, 60}, "Browse")) {
                    osuPath = tinyfd_selectFolderDialog("Select osu! location", "");
                }

                GuiSetState(strlen(errorMessage) == 0 ? STATE_NORMAL : STATE_DISABLED);
                if (GuiButton((Rectangle){150, 370, 980, 50}, "Confirm")) {
                    showSelectOsuPathDialog = false;
                }
                GuiSetState(STATE_NORMAL);
            }
            
        EndDrawing();
    }

    unload_music_playback();

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

static void __handle_input()
{
    if (IsKeyPressed(KEY_SPACE)) toggle_pause();
    if (IsKeyPressed(KEY_LEFT)) retreat_song(5);
    if (IsKeyPressed(KEY_RIGHT)) advance_song(5);
    if (IsKeyPressed(KEY_LEFT_ALT)) set_song("./resources/music/testSong2.mp3");
}
