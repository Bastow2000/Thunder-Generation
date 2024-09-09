/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ThunderGenerationAudioProcessor::ThunderGenerationAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), tree (*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{

  
}


ThunderGenerationAudioProcessor::~ThunderGenerationAudioProcessor()
{

}

juce::AudioProcessorValueTreeState::ParameterLayout ThunderGenerationAudioProcessor::createParameterLayout()
{
    // Sets slider values for the GUI
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.reserve(4+4+6+2);

    auto Length = std::make_unique<juce::AudioParameterFloat>(
        shapeSliderOneID, shapeSliderOneName, juce::NormalisableRange<float>(7.0f, 200.0f), 100.0f);

    auto PitchType = std::make_unique<juce::AudioParameterFloat>(
        shapeSliderTwoID, shapeSliderTwoName, juce::NormalisableRange<float>(1.0f, 3.0f), 1.0f);

    auto Noise = std::make_unique<juce::AudioParameterFloat>(
        shapeSliderThreeID, shapeSliderThreeName, juce::NormalisableRange<float>(-64, 6.0f), 0.f);

    auto Segments = std::make_unique<juce::AudioParameterFloat>(
        shapeSliderFourID, shapeSliderFourName, juce::NormalisableRange<float>(1., 35.f), 10.f);

    auto LowPass = std::make_unique<juce::AudioParameterFloat>(
        contentSliderOneID, contentSliderOneName, juce::NormalisableRange<float>(0.01f, 100.0f), 9.0f);

    auto HighPass = std::make_unique<juce::AudioParameterFloat>(
        contentSliderTwoID, contentSliderTwoName, juce::NormalisableRange<float>(2000.0f, 10000.0f), 10000.0f);

    auto WaveSpeed = std::make_unique<juce::AudioParameterFloat>(
        contentSliderThreeID, contentSliderThreeName, juce::NormalisableRange<float>(0.0001f, 1.0f ), 1.0f);

    auto StrikeDelay = std::make_unique<juce::AudioParameterFloat>(
        contentSliderFourID, contentSliderFourName, juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f);

    auto PostGain = std::make_unique<juce::AudioParameterFloat>(
        effectsSliderOneID, effectsSliderOneName, juce::NormalisableRange<float>(0.0f, 64.f), 30.f);

    auto PreGain = std::make_unique<juce::AudioParameterFloat>(
        effectsSliderTwoID, effectsSliderTwoName, juce::NormalisableRange<float>(-64.0f, 64.0f), -10.0f);

    auto Release = std::make_unique<juce::AudioParameterFloat>(
        effectsSliderThreeID, effectsSliderThreeName, juce::NormalisableRange<float>(0.0f, 500.0f), 100.0f);

    auto Threshold = std::make_unique<juce::AudioParameterFloat>(
        effectsSliderFourID, effectsSliderFourName, juce::NormalisableRange<float>(-64.0f, 0), -30.f);

    auto Ratio = std::make_unique<juce::AudioParameterFloat>(
        effectsSliderFiveID, effectsSliderFiveName, juce::NormalisableRange<float>(1.0f, 10.0f), 2.0f);

    auto Attack = std::make_unique<juce::AudioParameterFloat>(
        effectsSliderSixID, effectsSliderSixName, juce::NormalisableRange<float>(0.0f, 500.0f), 10.f);

    auto Volume = std::make_unique<juce::AudioParameterFloat>(
        mainSliderOneID, mainSliderOneName, juce::NormalisableRange<float>(-64.0f, 30.0f), 5.f);

    auto Distance = std::make_unique<juce::AudioParameterFloat>(
        mainSliderTwoID, mainSliderTwoName, juce::NormalisableRange<float>(0.01f, 20000.0f), 10000.f);

    // Push parameters into the vector
    params.push_back(std::move(Length));
    params.push_back(std::move(PitchType));
    params.push_back(std::move(Noise));
    params.push_back(std::move(Segments));

    params.push_back(std::move(LowPass));
    params.push_back(std::move(HighPass));
    params.push_back(std::move(WaveSpeed));
    params.push_back(std::move(StrikeDelay));

    params.push_back(std::move(PostGain));
    params.push_back(std::move(PreGain));
    params.push_back(std::move(Release));
    params.push_back(std::move(Threshold));
    params.push_back(std::move(Ratio));
    params.push_back(std::move(Attack));

    params.push_back(std::move(Volume));
    params.push_back(std::move(Distance));



    return {params.begin(), params.end()};
}
//==============================================================================
const juce::String ThunderGenerationAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ThunderGenerationAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ThunderGenerationAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ThunderGenerationAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ThunderGenerationAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ThunderGenerationAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ThunderGenerationAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ThunderGenerationAudioProcessor::setCurrentProgram (int /*index*/)
{
}

const juce::String ThunderGenerationAudioProcessor::getProgramName (int /*index*/)
{
    return {};
}

void ThunderGenerationAudioProcessor::changeProgramName (int /*index*/, const juce::String& /*newName*/)
{
}

//==============================================================================
void ThunderGenerationAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Sets up class variables
    int bufferSize = samplesPerBlock > 0 ? samplesPerBlock : 512;
    auto numChannels = getTotalNumOutputChannels();
   

    buffer_.setSize(numChannels, samplesPerBlock);
    buffer_.clear();

    int numSounds = 10;
    lowPassFilter_.resize(static_cast<size_type_>(numChannels));
    circularBuffer_.resize(static_cast<size_type_>(numChannels));
    
    for (int channel = 0; channel < numChannels; ++channel) 
    {
        circularBuffer_[static_cast<size_type_>(channel)].resize(static_cast<size_type_>(numSounds));
        for (int numBuf = 0; numBuf < numSounds; ++numBuf) 
        {
            circularBuffer_[static_cast<size_type_>(channel)][static_cast<size_type_>(numBuf)] = std::make_unique<CircularBuffer>(bufferSize, buffer_, numChannels);
        }
        lowPassFilter_[static_cast<size_type_>(channel)] = std::make_unique<LowPassFilter>(sampleRate, cutoffFrequency_,filterOrder_);
        lowPassFilter_[static_cast<size_type_>(channel)]->setup(sampleRate, cutoffFrequency_,filterOrder_);
        lowPassFilter_[static_cast<size_type_>(channel)]->reset();
    }
    ramp_ = std::make_unique<Ramp>(sampleRate);
    
    buffer_.setSize(numChannels, samplesPerBlock);
    
    reverb_ = std::make_unique<AudioEffects>(buffer_, getSampleRate(), bufferSize, numChannels);
    strike_ = std::make_unique<Strike>(buffer_, sampleRate, bufferSize, numChannels, buffer_.getNumSamples());
 
}

void ThunderGenerationAudioProcessor::releaseResources()
{
   
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ThunderGenerationAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

float ThunderGenerationAudioProcessor::sliderControl(float slider, juce::String sliderId)
{
    slider = *tree.getRawParameterValue(sliderId);
          
    return slider;
}

// Sets paramets for the sliders
void ThunderGenerationAudioProcessor::setRawParameterValues()
{
    // Gets the slider values and sets the values to the sliders
    float Length = *tree.getRawParameterValue(shapeSliderOneID);
    float Pitch = *tree.getRawParameterValue(shapeSliderTwoID);
    float Noise = *tree.getRawParameterValue(shapeSliderThreeID);
    float Segments = *tree.getRawParameterValue(shapeSliderFourID);

    float LowPass = *tree.getRawParameterValue(contentSliderOneID);
    float HighPass = *tree.getRawParameterValue(contentSliderTwoID);
    float WaveSpeed = *tree.getRawParameterValue(contentSliderThreeID);
    float StrikeDelay = *tree.getRawParameterValue(contentSliderFourID);

    float PostGain = *tree.getRawParameterValue(effectsSliderOneID);
    float PreGain = *tree.getRawParameterValue(effectsSliderTwoID);
    float Release = *tree.getRawParameterValue(effectsSliderThreeID);
    float Threshold = *tree.getRawParameterValue(effectsSliderFourID);
    float Ratio = *tree.getRawParameterValue(effectsSliderFiveID);
    float Attack = *tree.getRawParameterValue(effectsSliderSixID);

    float Volume = *tree.getRawParameterValue(mainSliderOneID);
    float Distance = *tree.getRawParameterValue(mainSliderTwoID);

    std::array<float, numSliders> shapeValues = {Length, Pitch, Noise,Segments,0,0 };
    shapeValues_ = shapeValues;
    strike_->setSliders(shapeValues,0);
    
    std::array<float,numSliders> contentValues = {LowPass, HighPass, WaveSpeed,StrikeDelay,0,0};
    contentValues_ = contentValues;
    strike_->setSliders(contentValues_,1);

    std::array<float,numSliders> effectsValues = {PostGain, PreGain, Release,Threshold,Ratio,Attack};
    effectsValues_ = effectsValues;
    strike_->setSliders(effectsValues,2);

    std::array<float,numSliders> mainValues = {Volume, Distance, 0,0,0,0};
    mainValues_ = mainValues;

    strike_->setSliders(mainValues,3);
}

// Helper function
void ThunderGenerationAudioProcessor::writeCircularBuffer(juce::AudioBuffer<float>& buffer, int index)
{
    for (int channel = 0; channel <  buffer.getNumChannels(); ++channel) 
    {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float sampleValue = buffer.getSample(channel, sample);
    
            circularBuffer_[static_cast<size_type_>(channel)][static_cast<size_type_>(index)]->writeSample(channel, sampleValue);
        }
    }   
}

// helper function for reading circular buffer
void ThunderGenerationAudioProcessor::readCircularBuffer(juce::AudioBuffer<float>& buffer)
{
    float volume = *tree.getRawParameterValue(mainSliderOneID);

    float delay = *tree.getRawParameterValue(contentSliderFourID);

    float volumeControl = juce::Decibels::decibelsToGain(volume);
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        float value = 0; 
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float strike = circularBuffer_[static_cast<size_type_>(channel)][0]->readSample(channel);
            float afterimage = circularBuffer_[static_cast<size_type_>(channel)][1]->readSample(channel);

            if (thunderOnly )
            {
                value += strike;
            }
            else 
            {
               value *= 0;
            }

            circularBuffer_[static_cast<size_type_>(channel)][2]->writeSample(channel, afterimage *2 );
            circularBuffer_[static_cast<size_type_>(channel)][2]->setReadPosition(channel, 512 * delay);
            float afterimage2 = circularBuffer_[static_cast<size_type_>(channel)][2]->readSample(channel);
        
            buffer.setSample(channel, sample, (((value )+afterimage2) * volumeControl) *10000000000000000 );
            
        }
        std::array<float,4> compParams = {-1.f,10.0f,1.0f,50.0f };
    
        std::array<float,2> waveshaperParams = {0,0};
        strike_->juceLowPassFilter(buffer,3000.0f,0.1f);
        strike_->juceLowPassFilter(buffer_,3000.0f,0.1f);
      
       
    }
}

void ThunderGenerationAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/) 
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    if (generateNoise) 
    {
        if (resetTimerCounter() == 0) 
        {
            startNoise();
            
            float cutoffFrequencyEnd = 0.f; 
            float expDecay = 50.f;

            setRawParameterValues();

            
            buffer.clear();
         
            // Generate thunder
            strike_->process(expDecay,cutoffFrequencyEnd);
            
          
        
            // Write to circular buffer
            writeCircularBuffer(buffer_, 0);

            // Clear for next strike
            buffer.clear();

            // Repeat process to a diffectent circular buffer.
            strike_->process(expDecay,cutoffFrequencyEnd);
            
          
        
            writeCircularBuffer(buffer_, 1);

        }
       
    }  

        if (resetTimerCounter() == 1) 
        {
            stopNoise();
          
            if (!generateNoise) 
            {
                // Done to make sure no wobble is occuring
                for (auto& filter : lowPassFilter_) 
                {
                    filter->reset();
                    filter->setFrequencyCutoff(20000);
                }
            }
        }      

    readCircularBuffer(buffer);

    float Distance = *tree.getRawParameterValue(mainSliderTwoID);
    
    juceLowPassFilter(buffer, Distance);
    juceLowPassFilter(buffer_, Distance);
   
}

void ThunderGenerationAudioProcessor::juceLowPassFilter(juce::AudioBuffer<float>& buffer, float cutoffFreq)
{
    spec_.sampleRate = getSampleRate();
    spec_.maximumBlockSize = static_cast<juce::uint32> (buffer.getNumSamples());
    spec_.numChannels = static_cast<juce::uint32> (buffer.getNumChannels());
    highPassFilter_.reset();

    // Setup high-pass filter
    auto& filter = highPassFilter_.template get<0>();
    auto coefficients = FilterCoefs::makeLowPass(getSampleRate(), cutoffFreq,0.1f);
    *filter.state = *coefficients;

    // Prepare processor chain
    highPassFilter_.prepare(spec_);

   
    // Process audio through processor chain
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> pcr(block);

    highPassFilter_.process(pcr);
}


void ThunderGenerationAudioProcessor::startNoise()
{
    if (!generateNoise) 
    {
        
        startTimer(10);
        generateNoise = true;
        
        // Done to make sure no wobble is occuring
        for (auto& filter : lowPassFilter_) 
        {
            filter->setFrequencyCutoff(cutoffFrequency_);
        }
        stopTimer();
    }

}


void ThunderGenerationAudioProcessor::resetNoise() 
{
    stopNoise();
    generateNoise = false;
    // Done to make sure no wobble is occuring
    for (auto& filter : lowPassFilter_) 
    {
        filter->setFrequencyCutoff(20000.0f);
    }
    
}

int ThunderGenerationAudioProcessor::resetTimerCounter() 
{
    int timerCounter_ = 0;
    timerCounter_++;
    if (timerCounter_ == 1)
        timerCounter_ -= 1;

 
    return timerCounter_;
}


void ThunderGenerationAudioProcessor::stopNoise()
{
    generateNoise = false;
    stopTimer();
    // Done to make sure no wobble is occuring
    for (auto& filter : lowPassFilter_) 
    {
        filter->setFrequencyCutoff(0.0f);
    }
}

//==============================================================================
bool ThunderGenerationAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ThunderGenerationAudioProcessor::createEditor()
{
    return new ThunderGenerationAudioProcessorEditor (*this);
}

//==============================================================================
void ThunderGenerationAudioProcessor::getStateInformation (juce::MemoryBlock& /*destData*/)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ThunderGenerationAudioProcessor::setStateInformation (const void* /*data*/, int /*sizeInBytes*/)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ThunderGenerationAudioProcessor();
}
