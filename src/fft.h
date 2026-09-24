#pragma once

#include "raylib/raylib.h"

#define MONO                           1
#define SAMPLE_RATE                    44100
#define SAMPLE_RATE_F                  44100.0f
#define FFT_WINDOW_SIZE                1024
#define BUFFER_SIZE                    512
#define PER_SAMPLE_BIT_DEPTH           16
#define AUDIO_STREAM_RING_BUFFER_SIZE  (FFT_WINDOW_SIZE*2)
#define EFFECTIVE_SAMPLE_RATE          (SAMPLE_RATE_F*0.5f)
#define WINDOW_TIME                    ((double)FFT_WINDOW_SIZE/(double)EFFECTIVE_SAMPLE_RATE)
#define FFT_HISTORICAL_SMOOTHING_DUR   2.0f
#define MIN_DECIBELS                   (-100.0f) // https://developer.mozilla.org/en-US/docs/Web/API/AnalyserNode/minDecibels
#define MAX_DECIBELS                   (-30.0f)  // https://developer.mozilla.org/en-US/docs/Web/API/AnalyserNode/maxDecibels
#define INVERSE_DECIBEL_RANGE          (1.0f/(MAX_DECIBELS - MIN_DECIBELS))
#define DB_TO_LINEAR_SCALE             (20.0f/2.302585092994046f)
#define SMOOTHING_TIME_CONSTANT        0.8f // https://developer.mozilla.org/en-US/docs/Web/API/AnalyserNode/smoothingTimeConstant
#define TEXTURE_HEIGHT                 1
#define FFT_ROW                        0
#define UNUSED_CHANNEL                 0.0f

typedef struct FFTComplex {
  float real, imaginary;
} FFTComplex;

typedef struct FFTData {
  FFTComplex *spectrum;
  FFTComplex *workBuffer;
  float *prevMagnitudes;
  float (*fftHistory)[BUFFER_SIZE];
  int fftHistoryLen;
  int historyPos;
  double lastFftTime;
  float tapbackPos;
} FFTData;

void capture_frame(FFTData *data, float const * audioSamples);
void render_frame(FFTData const *data, Image *fftImage);
void cooley_tukey_fft_slow(FFTComplex *spectrum, int n);

