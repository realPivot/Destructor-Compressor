/*
  ==============================================================================

    CircularBuffer.h
    Created: 18 Mar 2021 2:27:35am
    Author:  tjbac

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
class CircularBuffer {
public:
    CircularBuffer();
    CircularBuffer(int buffersize, int delayLength);
    float getData();
    void setData(float data);
    void nextSample();
private:
    AudioSampleBuffer buffer;
    int writeIndex;
    int readIndex;
    int delayLength;
};