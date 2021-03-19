/*
  ==============================================================================

    Compressor.cpp
    Created: 18 Mar 2021 7:27:14pm
    Author:  tjbac

  ==============================================================================
*/

#include "Compressor.h"

Compressor::Compressor()
{
    buffer = CircularBuffer(150, 20);
    timedAverage = 0.01;
    rms = 0;
    gain = 1;
}

float Compressor::compressSample(float data, float thresh, float ratio, float attack, float release, float knee)
{
    rms = (1 - timedAverage) * rms + timedAverage * std::pow(data, 2.0f);           // Adjusts rms value depending on incoming signal
    float dbRMS = 10 * std::log10(rms);                                             // Converts value to decibel

    float slope = 1 - (1 / ratio);                                                  // Calculate slope from ratio

    if (knee > 0 && dbRMS > (thresh - knee / 2.0) && dbRMS < (thresh + knee / 2.0)) {
        float kneeBottom = thresh - knee / 2.0;
        float kneeTop = thresh + knee / 2.0;
        float xPoints[2], yPoints[2];
        xPoints[0] = kneeBottom;
        xPoints[1] = kneeTop;
        xPoints[1] = std::fmin(0.0f, kneeTop);
        yPoints[0] = 0.0f;
        yPoints[1] = slope;
        slope = interpolatePoints(&xPoints[0], &yPoints[0], thresh);
        thresh = kneeBottom;
    }

    float dbGain = std::min(0.0f, (slope * (thresh - dbRMS)));                      // Find gain to be applied in db, make sure it's less than 0.0f
    float newGain = std::pow(10, dbGain / 20);                                      // Calculate newGain in linear scale

    float coeff;
    if (newGain < gain) coeff = attack;                                             
    else coeff = release;
    gain = (1 - coeff) * gain + coeff * newGain;                                    // Adjust gain based on new gain and coeff. AKA Do the compression here

    float compressedSample = gain * buffer.getData();
    buffer.setData(data);
    buffer.nextSample();

    return compressedSample;
}

float Compressor::interpolatePoints(float* xPoints, float* yPoints, float detectedValue)
{
    float result = 0.0f;
    int n = 2;

    for (int i = 0; i < n; i++) {
        float term = 1.0f;
            for (int j = 0; j < n; j++) {
                if (j != i) {
                    term *= (detectedValue - xPoints[j]) / (xPoints[i] - xPoints[j]);
                }
            }
            result += term * yPoints[i];
    }
    return result;
}
