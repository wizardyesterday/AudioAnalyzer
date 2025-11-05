//**************************************************************************
// file name: AudioAnalyzer.h
//**************************************************************************
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// This class implements a signal processing block known as an audio
// analyzer.  Given 16-bit PCM audio samples  plots can be displayed
// of the magnitude of the signal or the power spectrum of the signal.
///_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

#ifndef __SIGNALANALYZER__
#define __SIGNALANALYZER__

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>

#include <fftw3.h>

#include <X11/Xlib.h>

// This is the FFT size.
#define N (8192)

enum DisplayType {SignalMagnitude=1, PowerSpectrum};

class AudioAnalyzer
{
  //***************************** operations **************************

  public:

  AudioAnalyzer(DisplayType displayType,
      float verticalGain,
      float sampleRate,
      int32_t baselineInDb);

 ~AudioAnalyzer(void);

  void plotSignalAmplitude(int16_t *signalBufferPtr,uint32_t bufferLength);
  void plotPowerSpectrum(int16_t *signalBufferPtr,uint32_t bufferLength);

  private:

  //*******************************************************************
  // Utility functions.
  //*******************************************************************
  void initializeFftw(void);
  void initializeX(void);
  void initializeAnnotationParameters(float sampleRate);
  uint16_t convertRgbTo16Bit(uint8_t red,uint8_t green,uint8_t blue);
  void drawGridlines(void);

  uint32_t computePowerSpectrum(int16_t *signalBufferPtr,
                                uint32_t bufferLength);

  //*******************************************************************
  // Attributes.
  //*******************************************************************
  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  // Display support.
  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  DisplayType displayType;
  uint16_t scopeBackgroundColor;
  uint16_t scopeGridColor;
  uint16_t scopeSignalColor;

  char sweepTimeBuffer[80];
  char sweepTimeDivBuffer[80];
  char frequencySpanBuffer[80];
  char frequencySpanDivBuffer[80];

  int annotationHorizontalPosition;
  int annotationFirstLinePosition;
  int annotationSecondLinePosition;

  uint32_t spectrumStride;
  uint32_t signalStride;
  float verticalGain;
  int32_t baselineInDb;
  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

  // We ulitmately map values to these pixels.
  int windowWidthInPixels;
  int windowHeightInPixels;

  // This is used for plotting of signals.
  XPoint points[1024];

  // This is used for signal magnitude results.
  int16_t magnitudeBuffer[N];

  // This will be used for windowing data before the FFT.
  double hanningWindow[N];

  // FFTW3 support.
  fftw_complex *fftInputPtr;
  fftw_complex *fftOutputPtr;
  fftw_plan fftPlan;

  // Xlib support.
  Display *displayPtr;
  Window window;
  GC graphicsContext;
};

#endif // __SIGNALANALYZER__
