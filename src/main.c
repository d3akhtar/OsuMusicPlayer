#include "fft.h"
#include <math.h>
#include <raylib/raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <raylib/raygui.h>

#include <tinyfiledialogs.h>

int main()
{
    InitWindow(1280, 720, "Osu Music Player");
    SetTargetFPS(60);

    int screenWidth = GetScreenWidth(), screenHeight = GetScreenHeight();
    int fftRenderBufWidth = 880, fftRenderBufHeight = 580;

    Image fftImage = GenImageColor(BUFFER_SIZE, TEXTURE_HEIGHT, WHITE);
    Texture2D fftTexture = LoadTextureFromImage(fftImage);
    RenderTexture2D bufA = LoadRenderTexture(fftRenderBufWidth, fftRenderBufHeight);
    Vector2 iResolution = { (float)screenHeight, (float)screenHeight };

    Shader fftShader = LoadShader(0, "./resources/shaders/fft.fs");
    int iResolutionLoc = GetShaderLocation(fftShader, "iResolution");
    int iChannel0Loc = GetShaderLocation(fftShader, "iChannel0");
    SetShaderValue(fftShader, iResolutionLoc, &iResolution, SHADER_UNIFORM_VEC2);
    SetShaderValueTexture(fftShader, iChannel0Loc, fftTexture);

    InitAudioDevice();
    SetAudioStreamBufferSizeDefault(AUDIO_STREAM_RING_BUFFER_SIZE);

    Wave wav = LoadWave("./resources/music/testSong.mp3");
    WaveFormat(&wav, SAMPLE_RATE, PER_SAMPLE_BIT_DEPTH, MONO);

    AudioStream audioStream = LoadAudioStream(SAMPLE_RATE, PER_SAMPLE_BIT_DEPTH, MONO);
    PlayAudioStream(audioStream);

    int fftHistoryLen = (int)ceilf(FFT_HISTORICAL_SMOOTHING_DUR/WINDOW_TIME)+1;
    FFTData fftData = {
        .spectrum = RL_CALLOC(sizeof(FFTComplex), FFT_WINDOW_SIZE),
        .workBuffer = RL_CALLOC(sizeof(FFTComplex), FFT_WINDOW_SIZE),
        .prevMagnitudes = RL_CALLOC(BUFFER_SIZE, sizeof(float)),
        .fftHistory = RL_CALLOC(fftHistoryLen, sizeof(float[BUFFER_SIZE])),
        .fftHistoryLen = fftHistoryLen,
        .historyPos = 0,
        .lastFftTime = 0.0f,
        .tapbackPos = 0.01f
    };

    unsigned int wavCursor = 0;
    short const *wavPCM16 = wav.data;

    short chunkSamples[AUDIO_STREAM_RING_BUFFER_SIZE] = {0};
    float audioSamples[FFT_WINDOW_SIZE] = {0};

    float volumeValue = 0.0f;
    
    float songProgress = 0.0f;

    int currentPlaylistScrollIndex = 0, currentPlaylistActive = 1;

    bool showBrowseSongList = false;
    bool searchBrowseSongListEdit = false;
    char* searchBrowseSongList = "";

    int collectionDropdownOption = 0;
    bool collectionDropdownIsSelecting = false;

    int songListIndex = 0;
    int songListActive = 0;

    char const * osuPath = "{SELECT OSU! PATH}";
    // char const * errorMessage = "Invalid osu! Path";
    char const * errorMessage = "";
    bool showSelectOsuPathDialog = false;

    Texture2D placeholderTexture = LoadTexture("./resources/pspace.PNG");

    while (!WindowShouldClose())
    {
        while (IsAudioStreamProcessed(audioStream))
        {
            for (int i = 0; i < AUDIO_STREAM_RING_BUFFER_SIZE; i++)
            {
                int left = (wav.channels == 2) ? wavPCM16[wavCursor*2 + 0] : wavPCM16[wavCursor];
                int right = (wav.channels == 2) ? wavPCM16[wavCursor*2 + 1] : left;
                chunkSamples[i] = (short)((left + right) / 2);

                if (++wavCursor >= wav.frameCount) wavCursor = 0;
            }

            UpdateAudioStream(audioStream, chunkSamples, AUDIO_STREAM_RING_BUFFER_SIZE);

            for (int i = 0; i < FFT_WINDOW_SIZE; i++)
                audioSamples[i] = (chunkSamples[i*2] + chunkSamples[i*2+1]) * 0.5f/32767.0f;
        }

        CaptureFrame(&fftData, audioSamples);
        RenderFrame(&fftData, &fftImage);
        UpdateTexture(fftTexture, fftImage.data);
        
        ClearBackground(BLACK);
        
        BeginDrawing();

            BeginShaderMode(fftShader);
                SetShaderValueTexture(fftShader, iChannel0Loc, fftTexture);
                DrawTextureRec(bufA.texture, (Rectangle){0, 0, fftRenderBufWidth, -fftRenderBufHeight}, (Vector2) {20,20}, WHITE);
            EndShaderMode();

            DrawRectangleLines(10, 10, 900, 600, WHITE);
            DrawRectangleLines(20, 20, 880, 580, GRAY);
            DrawRectangle(10, 620, 900, 80, GRAY);

            GuiDrawIcon(122, 25, 630, 2, WHITE);
            GuiSliderBar((Rectangle){65, 637, 200, 15}, "", "", &volumeValue, 0.0f, 1.0f);
            GuiDrawIcon(129, 360, 630, 2, WHITE);
            GuiDrawIcon(131, 410, 630, 2, WHITE);
            GuiDrawIcon(132, 460, 630, 2, WHITE);
            GuiDrawIcon(134, 510, 630, 2, WHITE);
            GuiDrawIcon(58, 860, 630, 2, WHITE);
            
            DrawText("0:01/10:00", 25, 670, 20, WHITE);
            DrawRectangle(130, 675, 770, 10, WHITE);
            DrawCircle(135, 680, 10, RED);

            DrawTextureRec(placeholderTexture, (Rectangle){placeholderTexture.width/4.0f,placeholderTexture.height/4.0f,350,350}, (Vector2){920,10}, WHITE);
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
                if (GuiWindowBox((Rectangle) {340, 10, 600, 700 }, "Browse songs") == RESULT_PRESSED) {
                    showBrowseSongList = false;
                }
                
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

    UnloadShader(fftShader);
    UnloadRenderTexture(bufA);
    UnloadTexture(fftTexture);
    UnloadImage(fftImage);
    UnloadAudioStream(audioStream);
    UnloadWave(wav);
    CloseAudioDevice();

    RL_FREE(fftData.spectrum);
    RL_FREE(fftData.workBuffer);
    RL_FREE(fftData.prevMagnitudes);
    RL_FREE(fftData.fftHistory);

    CloseWindow();
    return 0;
}
