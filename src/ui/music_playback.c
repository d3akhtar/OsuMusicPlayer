#include "music_playback.h"
#include "fft.h"
#include "raylib/raygui.h"
#include "raylib/raymath.h"
#include "raylib/raylib.h"
#include "ui/elements.h"
#include "ui/styles.h"
#include "utils/format.h"
#include <stdlib.h>

static int __gui_draw_music_progress_bar(int posX, int posY, int width, int height, float *progress, Color unfinishedColor, Color finishedColor);

static void __set_song_progress_based_on_seconds(int newCurrentAudioStreamTimeSeconds);

static Image fftImage;
static Shader fftShader;
static RenderTexture2D bufA;
static Texture2D fftTexture;
static short *wavPCM16;
static short chunkSamples[AUDIO_STREAM_RING_BUFFER_SIZE] = {0};
static float audioSamples[FFT_WINDOW_SIZE] = {0};
static FFTData fftData;
static Wave wav;
static AudioStream audioStream;
static int iChannel0Loc;
  
static unsigned int wavCursor = 0;
static unsigned int frameCount = 0;
static int audioStreamTotalSeconds = 0;
static int currentAudioStreamTimeSeconds = 0;
static float songProgress = 0.0f;
static bool paused = false;
static bool musicProgressBarChanging = false;
static float volumeValue = 1.0f;

static int fftRenderBufWidth = 880, fftRenderBufHeight = 580;

void init_music_playback()
{
  int screenWidth = GetScreenWidth(), screenHeight = GetScreenHeight();

  fftImage = GenImageColor(BUFFER_SIZE, TEXTURE_HEIGHT, WHITE);
  fftTexture = LoadTextureFromImage(fftImage);
  bufA = LoadRenderTexture(fftRenderBufWidth, fftRenderBufHeight);
  Vector2 iResolution = { (float)screenHeight, (float)screenHeight };

  fftShader = LoadShader(0, "./resources/shaders/fft.fs");
  int iResolutionLoc = GetShaderLocation(fftShader, "iResolution");
  iChannel0Loc = GetShaderLocation(fftShader, "iChannel0");
  SetShaderValue(fftShader, iResolutionLoc, &iResolution, SHADER_UNIFORM_VEC2);
  SetShaderValueTexture(fftShader, iChannel0Loc, fftTexture);

  int fftHistoryLen = (int)ceilf(FFT_HISTORICAL_SMOOTHING_DUR/WINDOW_TIME)+1;
  fftData = (FFTData){
      .spectrum = (FFTComplex*)RL_CALLOC(sizeof(FFTComplex), FFT_WINDOW_SIZE),
      .workBuffer = (FFTComplex*)RL_CALLOC(sizeof(FFTComplex), FFT_WINDOW_SIZE),
      .prevMagnitudes = (float*)RL_CALLOC(BUFFER_SIZE, sizeof(float)),
      .fftHistory = RL_CALLOC(fftHistoryLen, sizeof(float[BUFFER_SIZE])),
      .fftHistoryLen = fftHistoryLen,
      .historyPos = 0,
      .lastFftTime = 0.0f,
      .tapbackPos = 0.01f
  };
}

void set_song(char const *path)
{
  UnloadAudioStream(audioStream);
  
  wav = LoadWave(path);
  WaveFormat(&wav, SAMPLE_RATE, PER_SAMPLE_BIT_DEPTH, MONO);
  frameCount = wav.frameCount;

  audioStream = LoadAudioStream(SAMPLE_RATE, PER_SAMPLE_BIT_DEPTH, MONO);
  PlayAudioStream(audioStream);

  audioStreamTotalSeconds = wav.frameCount / SAMPLE_RATE;
  currentAudioStreamTimeSeconds = 0;

  wavPCM16 = (short*)wav.data;
  wavCursor = 0;
}

void advance_song(unsigned int advanceAmount)
{  
  __set_song_progress_based_on_seconds((int)fmin(currentAudioStreamTimeSeconds+advanceAmount, audioStreamTotalSeconds-1));
}

void retreat_song(unsigned int retreatAmount)
{  
  __set_song_progress_based_on_seconds((int)fmax(currentAudioStreamTimeSeconds-retreatAmount, 0));
}

void toggle_pause()
{
  paused = !paused;
}

void update_song_visuals()
{
    songProgress = (float)wavCursor / (float)frameCount;
    currentAudioStreamTimeSeconds = audioStreamTotalSeconds*songProgress;
    
    SetAudioStreamVolume(audioStream, volumeValue);

    while (!paused && !musicProgressBarChanging && IsAudioStreamProcessed(audioStream))
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

    capture_frame(&fftData, audioSamples);
    render_frame(&fftData, &fftImage);
    UpdateTexture(fftTexture, fftImage.data);  
}

int gui_draw_music_playback()
{  
    DrawRectangle(15, 15, 890, 590, BLACK);
    DrawRectangleRounded((Rectangle){10, 620, 900, 80}, 0.2f, 1, (Color){143, 119, 141, 255});
    DrawRectangleRoundedLinesEx((Rectangle){10, 620, 900, 80}, 0.2f, 1, 2, BLACK);

    BeginShaderMode(fftShader);
        SetShaderValueTexture(fftShader, iChannel0Loc, fftTexture);
        DrawTextureRec(bufA.texture, (Rectangle){0, 0, (float)fftRenderBufWidth, (float)-fftRenderBufHeight}, (Vector2) {20,20}, WHITE);
    EndShaderMode();

    GuiDrawIcon(ICON_AUDIO, 25, 630, 2, CYAN);
    GuiSliderBar((Rectangle){65, 637, 200, 15}, "", "", &volumeValue, 0.0f, 1.0f);

    gui_draw_icon_button(ICON_PLAYER_PREVIOUS, 360, 630, 2, CYAN);
    if (gui_draw_icon_button(ICON_PLAYER_PLAY, 410, 630, 2, paused ? CYAN : GREEN)) paused = false;
    if (gui_draw_icon_button(ICON_PLAYER_PAUSE, 460, 630, 2, paused ? GREEN : CYAN)) paused = true;
    gui_draw_icon_button(ICON_PLAYER_NEXT, 510, 630, 2, CYAN);
    gui_draw_icon_button(ICON_REDO, 860, 630, 2, CYAN);
    
    DrawText(format_timer_progress(currentAudioStreamTimeSeconds, audioStreamTotalSeconds), 25, 670, 20, CYAN);
    if (__gui_draw_music_progress_bar(130, 675, 770, 10, &songProgress, LIGHTGRAY, (Color){22,201,201,255})) {
        musicProgressBarChanging = true;
        wavCursor = songProgress * (float)wav.frameCount;
    } else musicProgressBarChanging = false;
}

void unload_music_playback()
{
  UnloadShader(fftShader);
  UnloadRenderTexture(bufA);
  UnloadTexture(fftTexture);
  UnloadImage(fftImage);

  UnloadAudioStream(audioStream);
  UnloadWave(wav);

  RL_FREE(fftData.spectrum);
  RL_FREE(fftData.workBuffer);
  RL_FREE(fftData.prevMagnitudes);
  RL_FREE(fftData.fftHistory);
}

static int __gui_draw_music_progress_bar(int posX, int posY, int width, int height, float *progress, Color unfinishedColor, Color finishedColor)
{
  Vector2 mousePos = GetMousePosition();

  Clamp(*progress, 0.0f, 1.0f);
  
  DrawRectangle(posX, posY, width**progress, height, finishedColor);
  DrawRectangle(posX+width**progress, posY, width*(1-*progress), height, unfinishedColor);

  int playbackCursorPosX = posX + 5 + width**progress;
  DrawCircle(playbackCursorPosX, posY + 5, 12, BLACK);
  DrawCircle(playbackCursorPosX, posY + 5, 10, PINK);

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && (CheckCollisionPointRec(mousePos, (Rectangle){(float)posX, (float)posY, (float)width, (float)height}) || musicProgressBarChanging)) {
    musicProgressBarChanging = true;
    *progress = (mousePos.x - posX) / (float)width;
    return RESULT_CHANGED;
  } else musicProgressBarChanging = false;

  return RESULT_NONE;
}

static void __set_song_progress_based_on_seconds(int newCurrentAudioStreamTimeSeconds)
{
  currentAudioStreamTimeSeconds = Clamp(newCurrentAudioStreamTimeSeconds, 0, audioStreamTotalSeconds-1);
  songProgress = (float)currentAudioStreamTimeSeconds / (float)audioStreamTotalSeconds;
  wavCursor = frameCount * songProgress;  
}
