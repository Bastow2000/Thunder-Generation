#include <cmath>
#include <iostream>
#include "LowPassFilter.h"


// Constructor
LowPassFilter::LowPassFilter() {}

void LowPassFilter::setup(double sampleRate, float frequencyCutoff, int filterOrder)
{
    sampleRate_ = sampleRate;
    inverseSampleRate_ = 1.0f / sampleRate;
    cutoffFrequency_ = frequencyCutoff;
    filterOrder_ = filterOrder;
    calculateCoefficients(cutoffFrequency_);
    setFilterOrder(filterOrder_);
    reset();

    ready_ = true;
}


// Constructor specifying a sample rate
LowPassFilter::LowPassFilter(double sampleRate, float frequencyCutoff, int filterOrder)
{
    setup(sampleRate, frequencyCutoff, filterOrder);
}

// Set the frequency and recalculate coefficients
void LowPassFilter::setFrequencyCutoff(float frequency)
{
    // Stops the frequency from wobbling
    if (frequency <= 20.0f)
    {
        frequency = 0.0f;
    }
    cutoffFrequency_ = frequency;
    calculateCoefficients(frequency);


}


// Set Filter Order
void LowPassFilter::setFilterOrder(float filterOrder)
{
    filterOrder = filterOrder_;
}


void LowPassFilter::calculateCoefficients(float frequency)
{

    float omega_c, T;

    // Calculate coefficients for two second-order sections
    for (int i = 0; i < 10; ++i)
    {
        T = 1 / sampleRate_;
        frequency = (2 / T) * tan((frequency * T) / 2);
        omega_c = 2.0 * M_PI * frequency ;


        b0_[i] = omega_c * T;
        b1_[i] = omega_c * T;
        a1_[i] = (omega_c * T - 2) / (omega_c * T + 2) ;


    }

    // Initialise first-order section history
    for (int i = 0; i < 10; ++i)
    {
        lastX_[i] = 0.0;
        lastY_[i] = 0.0;
    }
    ready_ = true;
}


// Reset previous history of filter
void LowPassFilter::reset()
{
    for (int i = 0; i < 10; ++i)
    {
        lastX_[i] = 0.0;
        lastY_[i] = 0.0;
    }
    setFrequencyCutoff(20000);
    ready_ = true;
}

// Set Transition Rate
void LowPassFilter::setTransitionRate(float rate)
{
    transitionRate_ = rate;
}

// Get Cutoff Frequency
float LowPassFilter::getCutoffFrequency() const
{
    return cutoffFrequency_;
}

// Calculate the next sample of output, changing the envelope
// state as needed
float LowPassFilter::process(float input)
{
    if (!ready_)
        return input;

    float out = input;

    // Process through each first-order section
    for (int i = 0; i < 10; ++i)
    {
        float input = out * b0_[i] + lastX_[i] * b1_[i] - lastY_[i] * a1_[i];

        lastX_[i] = out;
        lastY_[i] = input;
        out = input;
    }

    return out;
}

// Destructor
LowPassFilter::~LowPassFilter()
{

}
