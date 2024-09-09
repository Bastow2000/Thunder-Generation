#include "AudioEffects.h"
AudioEffects::AudioEffects() 
: buffer_ (*(new juce::AudioBuffer<float>())),
sampleRate_ (0),
bufferSize_ (0),
numChannels_ (0)
{
}

// Constructor with samplerate & frequency cutoff
AudioEffects::AudioEffects (juce::AudioBuffer<float>& buffer, double sampleRate, int bufferSize, int numChannels) 
: buffer_ (buffer),
sampleRate_ (sampleRate),
bufferSize_ (bufferSize),
numChannels_ (numChannels)
{
    circularBuffer_.resize (static_cast<size_t>(numChannels));
    buffer_.setSize (numChannels_, bufferSize_);
    buffer_ = buffer;
    for (int channel = 0; channel < numChannels_; channel++)
    {
        circularBuffer_[static_cast<size_t>(channel)] = std::make_unique<CircularBuffer> (bufferSize_, buffer, numChannels_);
    }
}

// Helper function to set up the spec
juce::dsp::ProcessContextReplacing<float> AudioEffects::setup(juce::AudioBuffer<float>& audioBuffer)
{
    audioBuffer.setSize (numChannels_, bufferSize_);
    juce::dsp::AudioBlock<float> block(audioBuffer);
    juce::dsp::ProcessContextReplacing<float> pcr(block);

    return pcr;
}

// Helper function to set up the spec
void AudioEffects::specSetup()
{
    spec_.sampleRate = sampleRate_;
    spec_.maximumBlockSize = static_cast<juce::uint32> (bufferSize_);
    spec_.numChannels = static_cast<juce::uint32> (numChannels_);
}

void AudioEffects::setMixRate (float mixRate)
{
    mix_ = juce::jlimit (0.0f, 1.0f, mixRate);
}

// Forward delay line
//y[n] = x[n] + g * x[n - d]
void AudioEffects::forwardDelay (juce::AudioBuffer<float>& audioBuffer, float delayTime, float gain, float mixRate)
{
    // Set the mix rate
    setMixRate (mixRate);
    audioBuffer.setSize (numChannels_, bufferSize_);

    // Make a more represented mapping of the delay time
    // Before 1 would be 44 samples, 11 would be 511, now it's spread out more evenly
    float mappedValue = juce::jmap (delayTime, 1.0f, 1000.0f, 1.0f, 11.0f);

    // Calculate the delay time in samples
    int delaySamples = static_cast<int> ((sampleRate_ * mappedValue) / 1000);
    delaySamples = juce::jmin (delaySamples, bufferSize_ - 1);

    for (size_t channel = 0; channel < static_cast<size_t> (numChannels_); channel++)
    {
        // Get write buffer pointer
        auto* buffer = audioBuffer.getWritePointer (static_cast<int> (channel));

        for (int sample = 0; sample < bufferSize_; ++sample)
        {
            // Input x[n]
            float inputSample = buffer[sample];

            // Writing input to circular buffer x[n]
            circularBuffer_[channel]->writeSample (static_cast<int> (channel), inputSample);

            // Delaying read position to circular buffer x[n-d]
            circularBuffer_[channel]->setReadPosition (static_cast<int> (channel), delaySamples);

            // Read at delayed time x[n-d]
            float delayedSample = circularBuffer_[channel]->readSample (static_cast<int> (channel));

            // Calculate output sample
            //y[n] = x[n] + g * x[n - d]
            float outputSample = inputSample + gain * delayedSample;

            // Calculate the dry and wet signals
            float drySignal = (1.0f - mix_) * inputSample;
            float wetSignal = mix_ * outputSample;

            // Set the output sample in the buffer as the combination of dry and wet signals
            audioBuffer.setSample (static_cast<int>(channel), sample, gain * (drySignal + wetSignal));
        }
    }
   
}

//y[n] = x[n] - a * y[n - d]
void AudioEffects::feedbackDelay (juce::AudioBuffer<float>& audioBuffer, float delayTime, float gain, float mixRate, size_t channel, float feedback)
{
    setMixRate (mixRate);

    // Same as previously stated
    float mappedValue = juce::jmap (delayTime, 1.0f, 1000.0f, 1.0f, 11.0f);
    int delaySamples = static_cast<int> ((sampleRate_ * mappedValue) / 1000);
    delaySamples = juce::jmin (delaySamples, bufferSize_ - 1);
    float outputSample = 0.0f;

    auto* buffer = audioBuffer.getWritePointer (static_cast<int>(channel));

    for (int sample = 0; sample < bufferSize_; ++sample)
    {
        // Input x[n]
        float inputSample = buffer[sample];

        // Writing input to circular buffer x[n]

        float delayedSample = circularBuffer_[channel]->readSample (static_cast<int> (channel));

        // Calculate output sample
        //y[n] = x[n] - g * y[n - d]
        outputSample = inputSample + feedback * (delayedSample);

        float drySignal = (1.0f - mix_) * (inputSample);
        float wetSignal = mix_ * (outputSample);

        audioBuffer.setSample (channel, sample, gain * (drySignal + wetSignal));

        circularBuffer_[channel]->writeSample (static_cast<int> (channel), outputSample);
        circularBuffer_[channel]->setReadPosition (static_cast<int>(channel), delaySamples);
      
      
    }
}

void AudioEffects::diffusionSetup (juce::AudioBuffer<float>& audioBuffer, float delayTime, float gain, float mixRate, size_t channel, float feedback)
{
    // Set mix rate
    setMixRate (mixRate);

    // Get delay samples
    int delaySamples = static_cast<int> ((sampleRate_ * delayTime) / 1000);

    // Create vector of channels
    std::vector<size_t> channels (static_cast<size_t>(numChannels_));

    // Shuffle the channels
    std::iota (channels.begin(), channels.end(), 0);
    std::shuffle (channels.begin(), channels.end(), std::default_random_engine());

    // normal feedback
    delaySamples = juce::jmin (delaySamples, bufferSize_ - 1);

    auto* buffer = audioBuffer.getWritePointer (channels[static_cast<size_t>(channel)]);

    for (int sample = 0; sample < bufferSize_; ++sample)
    {
        // Input x[n]
        float inputSample = buffer[sample];

        // Writing input to circular buffer x[n]

        float delayedSample = circularBuffer_[channel]->readSample (static_cast<int> (channel));

        // Calculate output sample
        //y[n] = x[n] - g * y[n - d]
        float outputSample = inputSample + feedback * (delayedSample);

        float drySignal = (1.0f - mix_) * (inputSample);
        float wetSignal = mix_ * (outputSample);

        // Shuffled, Inverse of the delayed sample
        audioBuffer.setSample (static_cast<int>(channel), sample, -(gain * (drySignal + wetSignal)));

        circularBuffer_[channel]->writeSample (static_cast<int> (channel), outputSample);
        circularBuffer_[channel]->setReadPosition (static_cast<int>(channel), delaySamples);
    }
}

void AudioEffects::setFilterParams(float lowPass, float highPass)
{
    lowPass_ = lowPass;
    highPass_ = highPass;
}

void AudioEffects::feedbackMultiDelay (juce::AudioBuffer<float>& audioBuffer, float delayTime1, float delayTime2, float gain, float mixrate, float feedback)
{
    // Make a more represented mapping of the delay time
    float mappedValue1 = juce::jmap (delayTime1, 1.0f, 2000.0f, 1.0f, 11.0f);
    float mappedValue2 = juce::jmap (delayTime2, 1.0f, 2000.0f, 1.0f, 11.0f);

    // Calculate the delay time in samples
    int delaySamples1 = static_cast<int>((sampleRate_ * mappedValue1) / 1000);
    int delaySamples2 = static_cast<int>((sampleRate_ * mappedValue2) / 1000);

    // Ensure the delay samples are within the buffer size
    delaySamples1 = juce::jmin (delaySamples1, bufferSize_ - 1);
    delaySamples2 = juce::jmin (delaySamples2, bufferSize_ - 1);

    // Allows for both channels to have delay 
    feedbackDelay (audioBuffer, delayTime1, gain, mixrate, 0, feedback);
    feedbackDelay (audioBuffer, delayTime2, gain, mixrate, 1, feedback);

    // Applies filters to reduce artefacts
    juceHighPassFilter(audioBuffer, lowPass_, q_);
    juceLowPassFilter(audioBuffer, highPass_, q_);
   
    
}

void AudioEffects::diffusion (juce::AudioBuffer<float>& audioBuffer, float delayTime1, float delayTime2, float gain, float feedback)
{
    // Make a more represented mapping of the delay time
    float mappedValue1 = juce::jmap (delayTime1, 1.0f, 2000.0f, 1.0f, 11.0f);
    float mappedValue2 = juce::jmap (delayTime2, 1.0f, 2000.0f, 1.0f, 11.0f);

    int delaySamples1 = static_cast<int> (mappedValue1 * sampleRate_);
    int delaySamples2 = static_cast<int> (mappedValue2 * sampleRate_);

    delaySamples1 = juce::jmin (delaySamples1, bufferSize_ - 1);
    delaySamples2 = juce::jmin (delaySamples2, bufferSize_ - 1);

    diffusionSetup (audioBuffer, delayTime1, gain, 1, 0, feedback);
    diffusionSetup (audioBuffer, delayTime2, gain, 1, 1, feedback);

    houseHolderMatrix();
    
    juceHighPassFilter(audioBuffer, lowPass_, q_);
    juceLowPassFilter(audioBuffer, highPass_, q_);
}

void AudioEffects::houseHolderMatrix()
{
     // Initialise reflection vectors for each channel
    std::vector<float> reflectionVectors(static_cast<size_t>(numChannels_));
    
    // Set the reflection vector as the mean of the data
    for (size_t channel = 0; channel < static_cast<size_t>(numChannels_); ++channel) 
    {
        float* buffer = buffer_.getWritePointer(static_cast<int>(channel));
        float sum = 0.0f;
        for (int sample = 0; sample < bufferSize_; ++sample)
        {
            // Calculate the sum for the mean
            sum += buffer[sample];
        }
       
        // Set the reflection vector as the mean of the data
        reflectionVectors[channel] = sum / static_cast<float>(bufferSize_);
    }

    float normalise = 0.0f;

    // Compute the magnitude of the reflection vector for normalization
    for (size_t channel = 0; channel <  static_cast<size_t>(numChannels_); ++channel) {
        normalise += reflectionVectors[channel] * reflectionVectors[channel];
    }
    
    // Take the square root to get the magnitude
    normalise = std::sqrt(normalise);

    // Normalise the reflection vector so its magnitude is 1
    for (size_t channel = 0; channel <  static_cast<size_t>(numChannels_); ++channel) {
        reflectionVectors[channel] /= normalise;
    }

    // Calculate the constant vTv (dot product of the reflection vector with itself)
    float vTv = 0.0f;
    for (size_t channel = 0; channel <  static_cast<size_t>(numChannels_); ++channel) {
        vTv += reflectionVectors[channel] * reflectionVectors[channel];
    }

    // Process each channel independently
    for (size_t channel = 0; channel <  static_cast<size_t>(numChannels_); ++channel)
    {
        float* buffer = buffer_.getWritePointer(static_cast<int>(channel));
        
        // Iterate over each sample in the buffer
        for (int sample = 0; sample < bufferSize_; ++sample)
        {
            // Reset vTx for each sample
            float vTx = 0.0f; 
            
            // Calculate vTx (dot product of reflection vector and signal)
            vTx += reflectionVectors[channel] * buffer[sample];

            // Apply the Householder transformation directly on the buffer
            buffer[sample] -= 2.0f * (vTx / vTv) * reflectionVectors[channel];
        }
    }
}


void AudioEffects::juceHighPassFilter(juce::AudioBuffer<float>& audioBuffer, float cutoffFreq, float resonance)
{
    specSetup();
    highPassFilter_.reset();

    audioBuffer.setSize (numChannels_, bufferSize_);

    // Setup high-pass filter
    auto& filter = highPassFilter_.template get<0>();
    auto coefficients = FilterCoefs::makeHighPass(sampleRate_, cutoffFreq, resonance);
    *filter.state = *coefficients;

    // Prepare processor chain
    highPassFilter_.prepare(spec_);

   
    // Process audio through processor chain
    juce::dsp::AudioBlock<float> block(audioBuffer);
    juce::dsp::ProcessContextReplacing<float> pcr(block);

    highPassFilter_.process(pcr);
}

void AudioEffects::juceLowPassFilter(juce::AudioBuffer<float>& audioBuffer, float cutoffFreq, float resonance)
{
    specSetup();

    lowPassFilter_.reset();

    audioBuffer.setSize (numChannels_, bufferSize_);

    // Setup low-pass filter
    auto& filter = lowPassFilter_.template get<0>();
    auto coefficients = FilterCoefs::makeLowPass(sampleRate_, cutoffFreq, resonance);
    *filter.state = *coefficients;

    // Prepare processor chain
    lowPassFilter_.prepare(spec_);

 
    juce::dsp::AudioBlock<float> block(audioBuffer);
    juce::dsp::ProcessContextReplacing<float> pcr(block);

    // Process audio through processor chain
    lowPassFilter_.process(pcr);
}

// Apply a juce reverb effect to the audio buffer
void AudioEffects::juceReverb (juce::AudioBuffer<float>& audioBuffer, std::array<float, 5> reverbParams)
{
    // Use helper function
    specSetup();
    reverb_.prepare (spec_);

     audioBuffer.setSize (numChannels_, bufferSize_);
    
    params_.roomSize = reverbParams[0];
    params_.damping = reverbParams[1];
    params_.width = reverbParams[2];
    params_.wetLevel = reverbParams[3];
    params_.dryLevel = reverbParams[4];
    // params_.freezeMode = freeze->get();

    reverb_.setParameters (params_);

    juce::dsp::AudioBlock<float> block(audioBuffer);
    juce::dsp::ProcessContextReplacing<float> pcr(block);

    reverb_.process (pcr);
    
}

void AudioEffects::juceCompression(juce::AudioBuffer<float>& audioBuffer, std::array<float, 4> compressionParams, std::array<float, 2> waveshaperParams)
{ 
    // Use helper function
    specSetup();
    processorChain_.reset();
    processorChain_.prepare(spec_);
    
    auto& filter = processorChain_.template get<filterIndex>();                   
        filter.state = FilterCoefs::makeFirstOrderLowPass (spec_.sampleRate, 2000.0f);

    auto& preGain = processorChain_.template get<preGainIndex>(); 
    preGain.setGainDecibels(waveshaperParams[1]);

    auto& waveshaper = processorChain_.template get<waveshaperIndex>();
    waveshaper.functionToUse = [] (float x)
    {
        return std::atan(x); 
    };

    auto& postGain = processorChain_.template get<postGainIndex>(); 
    postGain.setGainDecibels(waveshaperParams[0]);

    auto& compressor = processorChain_.template get<compressorIndex>();
    float ratio = std::max(1.0f, compressionParams[1]);
    compressor.setThreshold(compressionParams[0]);
    compressor.setRatio(ratio);
    compressor.setAttack(compressionParams[2]);
    compressor.setRelease(compressionParams[3]);

    juce::dsp::AudioBlock<float> block(audioBuffer);
    juce::dsp::ProcessContextReplacing<float> pcr(block);
}


// Default Deconstructor
AudioEffects::~AudioEffects()
{
}

