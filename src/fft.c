#include "fft.h"
#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include <math.h>
#include <string.h>

void CaptureFrame(FFTData *data, float const * audioSamples)
{
  for (int i = 0; i < FFT_WINDOW_SIZE; i++)
  {
    float x = (2.0f * PI * i)/(FFT_WINDOW_SIZE - 1.0f);
    float blackmanWeight = 0.42f - 0.5f*cosf(x) + 0.08f*cosf(2.0f*x);
    data->workBuffer[i].real = audioSamples[i] * blackmanWeight;
    data->workBuffer[i].imaginary = 0.0f;
  }

  CooleyTukeyFFTSlow(data->workBuffer, FFT_WINDOW_SIZE);
  memcpy(data->spectrum, data->workBuffer, sizeof(FFTComplex)*FFT_WINDOW_SIZE);

  float smoothedSpectrum[BUFFER_SIZE];

  for (int bin = 0; bin < BUFFER_SIZE; bin++)
  {
    float re = data->workBuffer[bin].real;
    float im = data->workBuffer[bin].imaginary;
    float linearMagnitude = sqrtf(re*re + im*im) / FFT_WINDOW_SIZE;

    float smoothedMagnitude = SMOOTHING_TIME_CONSTANT*data->prevMagnitudes[bin] + (1.0f - SMOOTHING_TIME_CONSTANT) * linearMagnitude;
    data->prevMagnitudes[bin] = smoothedMagnitude;

    float db = logf(fmaxf(smoothedMagnitude, 1e-40f)) * DB_TO_LINEAR_SCALE;
    float normalized = (db - MIN_DECIBELS)*INVERSE_DECIBEL_RANGE;
    smoothedSpectrum[bin] = Clamp(normalized, 0.0f, 1.0f);
  }

  data->lastFftTime = GetTime();
  memcpy(data->fftHistory[data->historyPos], smoothedSpectrum, sizeof(smoothedSpectrum));
  data->historyPos = (data->historyPos + 1) % data->fftHistoryLen;
}

void RenderFrame(FFTData const *data, Image *fftImage)
{
  float framesSinceTapback = floorf((float)(data->tapbackPos));
  framesSinceTapback = Clamp(framesSinceTapback, 0.0f, (float)(data->fftHistoryLen)-1);

  int historyPosition = (data->historyPos - 1 - (int)framesSinceTapback%data->fftHistoryLen);
  if (historyPosition < 0) historyPosition += data->fftHistoryLen;

  float const *amplitude = data->fftHistory[historyPosition];
  for (int bin = 0; bin < BUFFER_SIZE; bin++)
    ImageDrawPixel(fftImage, bin, FFT_ROW, ColorFromNormalized((Vector4){amplitude[bin], UNUSED_CHANNEL, UNUSED_CHANNEL, UNUSED_CHANNEL}));
}

void CooleyTukeyFFTSlow(FFTComplex *spectrum, int n)
{
  int j = 0;
  for (int i = 1; i < n - 1; i++)
  {
    int bit = n >> 1;
    while (j >= bit)
    {
      j -= bit;
      bit >>= 1;
    }

    j += bit;
    if (i < j) {
      FFTComplex temp = spectrum[i];
      spectrum[i] = spectrum[j];
      spectrum[j] = temp;
    }
  }

  for (int len = 2; len <= n; len <<= 1)
  {
    float angle = -2.0f*PI/len;
    FFTComplex twiddleUnit = { cosf(angle), sinf(angle) };
    for (int i = 0; i < n; i += len)
    {
      FFTComplex twiddleCurrent = {1.0f, 0.0f};
      for (int j = 0; j < len/2; j++)
      {
        FFTComplex even = spectrum[i + j];
        FFTComplex odd = spectrum[i + j + len/2];
        FFTComplex twiddleOdd = {
          odd.real * twiddleCurrent.real - odd.imaginary * twiddleCurrent.imaginary,
          odd.real * twiddleCurrent.imaginary - odd.imaginary * twiddleCurrent.real
        };

        spectrum[i + j].real = even.real + twiddleOdd.real;
        spectrum[i + j].imaginary = even.imaginary + twiddleOdd.imaginary;
        spectrum[i + j + len/2].real = even.real - twiddleOdd.real;
        spectrum[i + j + len/2].imaginary = even.imaginary - twiddleOdd.imaginary;

        float twiddleRealNext = twiddleCurrent.real * twiddleUnit.real - twiddleCurrent.imaginary * twiddleUnit.imaginary;
        twiddleCurrent.imaginary = twiddleCurrent.real * twiddleUnit.imaginary + twiddleCurrent.imaginary * twiddleUnit.real;
        twiddleCurrent.real = twiddleRealNext;
      }
    }
  }
}
