/*
  ==============================================================================

    CircularBuffer.cpp
    Created: 18 Mar 2021 2:27:35am
    Author:  tjbac

  ==============================================================================
*/

#include "CircularBuffer.h"

CircularBuffer::CircularBuffer()
{
    buffer = AudioSampleBuffer();
    writeIndex = readIndex = delayLength = 0;
}

CircularBuffer::CircularBuffer(int buffersize, int delayLength)
{
    buffer = AudioSampleBuffer(1, buffersize);
    buffer.clear();
    writeIndex = delayLength;
    readIndex = 0;
    this->delayLength = delayLength;
}

float CircularBuffer::getData()
{
    return buffer.getSample(0, readIndex);
}

void CircularBuffer::setData(float data)
{
    buffer.setSample(0, writeIndex, data);
}

void CircularBuffer::nextSample()
{
    int bufferLength = buffer.getNumSamples();
    readIndex = ((bufferLength + writeIndex) - delayLength) % bufferLength;
    writeIndex = (writeIndex + 1) % bufferLength;
}
