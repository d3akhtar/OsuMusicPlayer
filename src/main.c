#include "fft.h"
#include <raylib/raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <raylib/raygui.h>

void CaptureFrame(FFTData *data, float const audioSamples);
void RenderFrame(FFTData const *data, Image *fftImage);

int main()
{
    InitWindow(1280, 720, "Osu Music Player");
    SetTargetFPS(60);

    int scrollIndex = 0, active = 1;

    while (!WindowShouldClose())
    {
        ClearBackground(BLACK);
        
        BeginDrawing();

            DrawRectangleLines(10, 10, 900, 600, WHITE);
            DrawRectangleLines(20, 20, 880, 580, GRAY);
            DrawRectangleLines(10, 620, 900, 80, GREEN);
            DrawRectangleLines(920, 10, 350, 350, WHITE);
            DrawText("Song: PARTY In PSPACE", 920, 370, 20, WHITE);
            DrawText("Artist: tnshi", 920, 400, 20, WHITE);

            GuiListView((Rectangle) {920, 440, 350, 200}, "Playlist", &scrollIndex, &active);

            GuiButton((Rectangle) {920, 660, 350, 40}, "Browse");
            
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
