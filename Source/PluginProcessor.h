/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Strike.h"

#include "LowPassFilter.h"
#include "CircularBuffer.h"
#include "Ramp.h"
#include "AudioEffects.h"
//==============================================================================
/**
*/
class ThunderGenerationAudioProcessor  : public juce::AudioProcessor, private juce::Timer
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    ThunderGenerationAudioProcessor();
    ~ThunderGenerationAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void setRawParameterValues();
    void writeCircularBuffer(juce::AudioBuffer<float>& buffer, int index);
    void readCircularBuffer(juce::AudioBuffer<float>& buffer);
    void startNoise();
    void stopNoise();
    void resetNoise();
    int resetTimerCounter();
    void juceLowPassFilter(juce::AudioBuffer<float>& buffer, float cutoffFreq);
    float sliderControl(float slider, juce::String sliderId);
    juce::AudioProcessorValueTreeState& getTree() 
    { return tree; }
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState tree;

    bool strikeOnly = false;
    bool rumbleOnly = false;
    bool thunderOnly = false;
    bool afterImageOnly = false;

    bool generateNoise = false;
    bool bufferIsPlaying = false;
    bool bufferHasFinished = false;

   

private:
    float cutoffFrequency_{20000};
    int filterOrder_{10};
    
    float mix_ {0.f};
    juce::Random random;
    juce::AudioBuffer<float> buffer_;
    juce::AudioBuffer<float> buffer2_;
    std::vector<juce::AudioBuffer<float>> buffers_;

    std::array<float, numSliders> shapeValues_;
    std::array<float,numSliders> contentValues_;
    std::array<float,numSliders> effectsValues_;
    std::array<float,numSliders> mainValues_;
  
    std::unique_ptr<Strike> strike_;
    std::unique_ptr<Ramp> ramp_;
   
    std::vector<std::unique_ptr<LowPassFilter>> lowPassFilter_;
    std::unique_ptr<AudioEffects> reverb_;

    using Filter = juce::dsp::IIR::Filter<float>;
    using FilterCoefs = juce::dsp::IIR::Coefficients<float>;

    using highPassFilterOne = juce::dsp::IIR::Filter<float>;
    using highPassFilterCoef = juce::dsp::IIR::Coefficients<float>;
    juce::dsp::ProcessorChain<juce::dsp::ProcessorDuplicator<highPassFilterOne, highPassFilterCoef>> highPassFilter_;

    std::vector<std::vector<std::unique_ptr<CircularBuffer>>> circularBuffer_;
    
    juce::dsp::ProcessSpec spec_;

    using size_type_ = std::vector<std::unique_ptr<LowPassFilter>>::size_type;
    
    void timerCallback() override 
    {
      generateNoise = false;
    }
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThunderGenerationAudioProcessor)
};
