#pragma once
#include <JuceHeader.h>
#include <algorithm>
#include <array>
#include <random>
#include "CircularBuffer.h"

enum
{
    filterIndex,   
    preGainIndex,
    waveshaperIndex,
    postGainIndex,
    compressorIndex
};

class AudioEffects {
public:
    // Default Constructor
    AudioEffects();

    // Constructor with samplerate & frequency cutoff
    AudioEffects(juce::AudioBuffer<float>& buffer, double sampleRate, int bufferSize, int numChannels);

    // Default Destructor 
    ~AudioEffects();

    // setups juce audio effects aiming at less repetitive code
    juce::dsp::ProcessContextReplacing<float> setup(juce::AudioBuffer<float>& audioBuffer);

    // setups juce audio effects aiming at less repetitive code
    void specSetup();

    // Forward delay line
    void forwardDelay(juce::AudioBuffer<float>& audioBuffer,float delayTime, float gain, float mixRate);

    // Feedback delay line
    void feedbackDelay(juce::AudioBuffer<float>& audioBuffer,float delayTime,float gain, float mixRate,size_t channel, float feedback);

    // Feedback delay line with multiple channels
    void diffusionSetup(juce::AudioBuffer<float>& audioBuffer,float delayTime,float gain, float mixRate,size_t channel, float feedback);

    // Feedback delay line with multiple channels
    void feedbackMultiDelay(juce::AudioBuffer<float>& audioBuffer,float delayTime1,float delayTime2, float gain, float mixrate,float feedback);

    // Set the mix rate
    void setMixRate(float mixRate);

    // Set the filter parameters
    void setFilterParams(float lowPass, float highPass);

    // Apply a low pass filter using the juce framework
    void juceLowPassFilter(juce::AudioBuffer<float>& audioBuffer, float cutoffFreq, float resonance);

    // Apply a high pass filter using the juce framework
    void juceHighPassFilter(juce::AudioBuffer<float>& audioBuffer, float cutoffFreq, float resonance);

    // Apply a householder matrix to the audio buffer
    void houseHolderMatrix();

    // Apply a diffusion effect to the audio buffer
    void diffusion(juce::AudioBuffer<float>& audioBuffer,float delayTime1, float delayTime2,float gain, float feedback);

    // Apply a juce reverb effect to the audio buffer
    void juceReverb(juce::AudioBuffer<float>& audioBuffer, std::array<float, 5> reverbParams);

    // Apply a juce compression effect to the audio buffer
    void juceCompression(juce::AudioBuffer<float>& audioBuffer, std::array<float, 4> compressionParams, std::array<float, 2> waveshaperParams);

private:
    juce::AudioBuffer<float> buffer_;

    juce::dsp::Reverb::Parameters params_;
   
    juce::dsp::Reverb reverb_;

    using highPassFilterOne = juce::dsp::IIR::Filter<float>;
    using highPassFilterCoef = juce::dsp::IIR::Coefficients<float>;
    juce::dsp::ProcessorChain<juce::dsp::ProcessorDuplicator<highPassFilterOne, highPassFilterCoef>> highPassFilter_;

    using lowPassFilterOne = juce::dsp::IIR::Filter<float>;
    using lowPassFilterCoef = juce::dsp::IIR::Coefficients<float>;
    juce::dsp::ProcessorChain<juce::dsp::ProcessorDuplicator<lowPassFilterOne, lowPassFilterCoef>> lowPassFilter_;

    juce::dsp::ProcessSpec spec_ {};
   
    using Filter = juce::dsp::IIR::Filter<float>;
    using FilterCoefs = juce::dsp::IIR::Coefficients<float>;
    using Compressor = juce::dsp::Compressor<float>;

    juce::dsp::ProcessorChain<juce::dsp::ProcessorDuplicator<Filter, FilterCoefs>,
        juce::dsp::Gain<float>,
        juce::dsp::WaveShaper<float>,
        juce::dsp::Gain<float>,
        Compressor> processorChain_;
    float mix_ = 0.0f;
    std::vector<std::unique_ptr<CircularBuffer>> circularBuffer_;
    double sampleRate_;
    int bufferSize_;
    int numChannels_;

    float lowPass_;
    float highPass_;
    float q_ {0.1f};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioEffects)
};

