#pragma once
#include <algorithm>
#include <cmath>
#include <iostream>
#include <JuceHeader.h>
#include <random>
#include <vector>

#include "../../ParamIds.h"
#include "AudioEffects.h"
#include "CircularBuffer.h"
#include "LowPassFilter.h"
#include "Ramp.h"

class AudioEffects;

// This class generates the attempted thunder heard in the standalone application
class Strike : public AudioEffects
{
public:
    // Default Constructor
    Strike();

    // Constructor specifying a sample rate and frequency cutoff
    Strike(juce::AudioBuffer<float>& buffer, double sampleRate, int bufferSize, int numChannels, int numSamples);

    // Setup the Strike object with the specified parameters
    void setup(juce::AudioBuffer<float>& buffer, double sampleRate, int bufferSize, int numChannels, int numSamples);

    // Generates White Noise
    void strikeWhiteNoise(float variation);

    // Modifies the White noise to have bias 
    float calculateNoise();

    // Modifies the amplitude of the signal by the specified factor
    void modifyAmplitude(float factor);

    // Creates a gaussian curve using the box muller transformation
    float gaussianCurve();

    // Generate a low pass filter with a varying cutoff frequency
    void varyingLowPassFilter(float steepness, float cutoffFrequencyStart, float cutoffFrequencyEnd);

    // Generate a low pass filter with the specified cutoff frequency and filter order
    void lowPassFilter(float cutoffFrequency, int filterOrder);

    // Generates a burst of low, mid, and high frequencies with exponential decay
    void fftModifications(float expDecay,std::vector<std::tuple<float, float, float>> freqModifications);

    // Generate an N wave with the specified number of segments
    std::vector<double> NWave(int numSegments);

    // multiple diffusion functions one after the other
    void afterimage();

    // Perform the strike operation with the specified parameters
    void strike(float transitionRate, float expDecay, float cutoffFrequencyStart, float cutoffFrequencyEnd);

    // Process the strike operation with the specified parameters
    void process(float expDecay, float cutoffFrequencyEnd);

    // Sets Slider Values
    void setSliders(std::array<float,numSliders> value, float slider);

    // Reset the previous history of the filter
    void reset();

    // Destructor
    ~Strike();

private:
    // Helper function to set the mix rate
    void setMixRate(float mixRate)
    {
        mix_ = juce::jlimit(0.0f, 1.0f, mixRate);
    }

    // Member variables
    int bufferSize_;
    int numChannels_;
    int numSamples_;
    int filterOrder_;
    int fftOrder_;

    double sampleRate_;

    float mix_;
    float frequencyCutoff_;

    using Filter = juce::dsp::IIR::Filter<float>;
    using FilterCoefs = juce::dsp::IIR::Coefficients<float>;

    juce::dsp::ProcessorChain<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Filter<float>> processorChain_;
    juce::dsp::ProcessSpec spec_;
    
    bool isFiltering_;

    std::array<float, 4> sliderOne_;
    std::array<float, 4> sliderTwo_;
    std::array<float, 4> sliderThree_;
    std::array<float, 4> sliderFour_;
    float sliderFive_;
    float sliderSix_;

    juce::AudioBuffer<float>& buffer_;

    juce::AudioBuffer<float>& tempBuffer_;

    juce::Random random_;

    std::vector<std::tuple<float, float, float>> freqModifications2_ =
    {
        {0, 63, -1},
        {64, 127, -5},
        {128, 257, -10},
        {258, 511, -15},
        {512, 1023, -20},
        {1024, 2047, -25},
        {2048, 4095, -30},
        {4096, 6000, -45},
        {6000, 8171, -55},
        {8172, 16343, -70},
        {16344, 20000, -90},
        {20001, 44100, 0}
    };

    std::vector<std::tuple<float, float, float>> freqModifications3_ =
    {
        {0, 63, -1},
        {64, 127, -5},
        {128, 257, -10},
        {258, 511, -15},
        {512, 1023, -20},
        {1024, 2047, -25},
        {2048, 4095, -30},
        {4096, 6000, -35},
        {6000, 8171, -40},
        {8172, 16343, -50},
        {16344, 20000, -60},
        {20001, 44100, 0}
    };

    std::vector<std::tuple<float, float, float>> freqModifications1_ =
    {
        {0, 63, -1},
        {64, 127, -3},
        {128, 257, -5},
        {258, 511, -5},
        {512, 1023, -5},
        {1024, 2047, -10},
        {2048, 4095, -10},
        {4096, 6000, -10},
        {6000, 8171, -15},
        {8172, 16343, -15},
        {16344, 20000, -20},
        {20001, 44100, 0}
    };

    std::vector<std::tuple<float, float, float>> freqModifications_ =
    {
        {0, 63, -1},
        {64, 127, -5},
        {128, 257, -5},
        {258, 511, -7},
        {512, 1023, -10},
        {1024, 2047, -15},
        {2048, 4095, -15},
        {4096, 6000, -15},
        {6000, 8171, -20},
        {8172, 16343, -30},
        {16344, 20000, -35},
        {20001, 44100, 0}
    };

    std::vector<std::unique_ptr<LowPassFilter>> lowPassFilter_;
    std::vector<std::unique_ptr<LowPassFilter>> varyingLowPassFilter_;
    using size_type_ = std::vector<std::unique_ptr<LowPassFilter>>::size_type;
    std::unique_ptr<Ramp> ramp_;
    std::vector<std::unique_ptr<CircularBuffer>> circularBuffer_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Strike)
};
