#include <raylib/raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <raylib/raygui.h>

int main()
{
    InitWindow(1280, 720, "Osu Music Player");
    SetTargetFPS(60);

    int currentPlaylistScrollIndex = 0, currentPlaylistActive = 1;

    bool showBrowseSongList = false;
    bool searchBrowseSongListEdit = false;
    char* searchBrowseSongList = "";

    int collectionDropdownOption = 0;
    bool collectionDropdownIsSelecting = false;

    int songListIndex = 0;
    int songListActive = 0;

    while (!WindowShouldClose())
    {
        ClearBackground(BLACK);
        
        BeginDrawing();

            DrawRectangleLines(10, 10, 900, 600, WHITE);
            DrawRectangleLines(20, 20, 880, 580, GRAY);
            DrawRectangleLines(10, 620, 900, 80, GREEN);
            DrawRectangleLines(920, 10, 350, 350, WHITE);
            DrawText("Song: PARTY In PSPACE", 920, 370, 20, WHITE);
            DrawText("Artist: tnshi", 920, 390, 20, WHITE);

            DrawText("Playlist: tnshi songs", 920, 420, 20, WHITE);
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
                GuiWindowBox((Rectangle) {340, 10, 600, 700 }, "Browse songs");
                if (GuiTextBox((Rectangle) {340, 34, 450, 30}, searchBrowseSongList, 20, searchBrowseSongListEdit)) {
                    searchBrowseSongList[0] = '\0';
                    searchBrowseSongListEdit = !searchBrowseSongListEdit;
                }

                if (!searchBrowseSongListEdit && strlen(searchBrowseSongList) == 0) {
                    DrawText("Search songs...", 345, 40, 18, GRAY);
                }

                GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
                GuiSetStyle(DROPDOWNBOX, TEXT_PADDING, 10);
                switch (GuiDropdownBox((Rectangle) {790, 34, 150, 30}, "All; Collection 1; Collection 2; Collection 3333333333333", &collectionDropdownOption, collectionDropdownIsSelecting)) {
                    case RESULT_CHANGED:
                        collectionDropdownIsSelecting = false;
                        break;
                    case RESULT_PRESSED:
                        collectionDropdownIsSelecting = true;
                }

                GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
                GuiSetStyle(DROPDOWNBOX, TEXT_PADDING, 0);

                GuiSetStyle(LISTVIEW, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
                GuiSetStyle(LISTVIEW, TEXT_PADDING, 10);
                GuiSetStyle(LISTVIEW, BORDER_WIDTH, 2);
                GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_NORMAL, 1);
                GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_WIDTH, 1);
                GuiListView((Rectangle) {340, 60, 600, 656}, "Song 1;Song 2;Song 3;Song 4;Song 5;", &songListIndex, &songListActive);
                GuiSetStyle(LISTVIEW, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
                GuiSetStyle(LISTVIEW, TEXT_PADDING, 0);
                GuiSetStyle(LISTVIEW, BORDER_WIDTH, 0);
                GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_NORMAL, 0);
                GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_WIDTH, 0);
            }
            
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
