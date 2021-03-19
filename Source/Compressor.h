/*
  ==============================================================================

    Compressor.h
    Created: 18 Mar 2021 7:27:14pm
    Author:  tjbac

  ==============================================================================
*/

#pragma once
#include "CircularBuffer.h"

class Compressor {
public:
    Compressor();
    float compressSample(float data, float thresh, float ratio, float attack, float release, float knee);
    float interpolatePoints(float* xPoints, float* yPoints, float detectedValue);
private:
    CircularBuffer buffer;
    float timedAverage;
    float rms;
    float gain;
};