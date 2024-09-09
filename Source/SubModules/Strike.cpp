#include "Strike.h"

// Default constructor
Strike::Strike() :
    AudioEffects(*(new juce::AudioBuffer<float>()), 0.0, 0, 0),
    bufferSize_(0),
    numChannels_(0),
    numSamples_(0),
    filterOrder_(0),
    fftOrder_(0),
    frequencyCutoff_(0),
    isFiltering_(false),
    buffer_(*(new juce::AudioBuffer<float>())),
    tempBuffer_(*(new juce::AudioBuffer<float>()))  
{}

// Constructor specifying a sample rate and frequency cutoff
Strike::Strike(juce::AudioBuffer<float>& buffer, double sampleRate, int bufferSize, int numChannels, int numSamples) :
    AudioEffects(buffer, sampleRate, bufferSize, numChannels),
    bufferSize_(bufferSize),
    numChannels_(numChannels),
    numSamples_(numSamples),
    filterOrder_(0),
    fftOrder_(0),
    frequencyCutoff_(20000.f),
    isFiltering_(false),
    buffer_(buffer),
    tempBuffer_(buffer)
{
    setup(buffer, sampleRate, bufferSize, numChannels, numSamples);
}

// Setup function to initialise variables and objects
void Strike::setup(juce::AudioBuffer<float>& buffer, double sampleRate, int bufferSize, int numChannels, int numSamples)
{
    // Initialise variables
    bufferSize_ = bufferSize;
    numChannels_ = numChannels;
    numSamples_ = numSamples;
    filterOrder_ = 10;
    sampleRate_ = sampleRate ;
    frequencyCutoff_ = 20000.f;

    fftOrder_ = static_cast<int>(std::log2(bufferSize_));

    buffer.setSize(numChannels, bufferSize);
    

    // Initialise class variables
    ramp_ = std::make_unique<Ramp>(sampleRate);
  

    lowPassFilter_.resize(static_cast<size_type_>(numChannels));
    circularBuffer_.resize(static_cast<size_type_>(numChannels));
    varyingLowPassFilter_.resize(static_cast<size_type_>(numChannels));

    for (int channel = 0; channel < numChannels_; ++channel)
    {
        circularBuffer_[static_cast<size_type_>(channel)] = std::make_unique<CircularBuffer>(bufferSize_, buffer_, numChannels_);
        lowPassFilter_[static_cast<size_type_>(channel)] = std::make_unique<LowPassFilter>(sampleRate_, frequencyCutoff_, filterOrder_);
        lowPassFilter_[static_cast<size_type_>(channel)]->setup(sampleRate_, frequencyCutoff_, filterOrder_);
        varyingLowPassFilter_[static_cast<size_type_>(channel)] = std::make_unique<LowPassFilter>(sampleRate_, frequencyCutoff_, filterOrder_);
        varyingLowPassFilter_[static_cast<size_type_>(channel)]->setup(sampleRate_, frequencyCutoff_, filterOrder_);
    }
}

// Generates White Noise
void Strike::strikeWhiteNoise(float variation)
{
    float alpha = 0.5f; 
    float value = calculateNoise();

    buffer_.clear();
    
    for (int channel = 0; channel < numChannels_; ++channel)
    {
        float* buffer = buffer_.getWritePointer(channel);
        
        // Generate white noise and apply exponential moving average
        float smoothen = 0.0f; 
     
        for (int sample = 0; sample < numSamples_; ++sample)
        {
            float randomValue = ( random_.nextFloat() * value * variation);
           
            // Apply the exponential moving average
            smoothen = (alpha * randomValue) + (1.0f - alpha) * smoothen;
           
            buffer[sample] += (( smoothen )); 
           
        }
     
    }

}

// Modifies the White noise to have bias
float Strike::calculateNoise()
{
    float value = 0.0f;
    float chances = 0; 
    float chance = random_.nextFloat() * 100.0f;
    
    chances = gaussianCurve();

    // Has a chance out of 100 to apply a bias to the white noise
    if (chances <= 0.1f && chance >= 0.0f)
    {
        value = random_.nextFloat() * 1.75f - 0.75f;
    
        value *= gaussianCurve();
    }
    else if (chances <= 0.3f)
    {
        value = random_.nextFloat() * 1.1f - 0.1f;
      
        
    }
    else if (chances <= 0.7f)
    {
        value = random_.nextFloat() * 1.0005f - 0.0005f;
     
    }
    else if (chances <= 0.99f)
    {
        value = random_.nextFloat() * 1.15f - 0.15f;
        
        value *= gaussianCurve();
    }
    else 
    {
       value = random_.nextFloat() * 1.05f - 0.05f;
       
       
    }
    if(value == 0.0f)
    {
        value = random_.nextFloat() * 1.05f - 0.05f;
       
        value *= gaussianCurve();
    }
    
    return value ;
}

// Reduce the amplitude of the signal
void Strike::modifyAmplitude(float factor)
{
    for (int channel = 0; channel < numChannels_; ++channel)
    {
        float* buffer = buffer_.getWritePointer(channel);
        for (int sample = 0; sample < numSamples_; ++sample)
        {
            buffer[sample] *= factor;
        }
    }
}

// Varying low pass filter
void Strike::varyingLowPassFilter(float steepness, float cutoffFrequencyStart, float CutoffFrequencyEnd)
{
    if (!isFiltering_)
    {   
        // Set the start value of the ramp
        ramp_->setValue(cutoffFrequencyStart);
        ramp_->rampTo(CutoffFrequencyEnd, steepness);
        isFiltering_ = true;
    }

    for (int channel = 0; channel < numChannels_; ++channel)
    {
        float* buffer = buffer_.getWritePointer(channel);

        for (int sample = 0; sample < numSamples_; ++sample)
        {
            float currentCutoff = ramp_->process();

            // Set filter order and frequency cutoff
            varyingLowPassFilter_[static_cast<size_type_>(channel)]->setFilterOrder(filterOrder_);
            varyingLowPassFilter_[static_cast<size_type_>(channel)]->setFrequencyCutoff(currentCutoff);

            // Apply filter to buffer
            buffer[sample] = varyingLowPassFilter_[static_cast<size_type_>(channel)]->process(buffer[sample]);

            if (ramp_->finished())
            {
                isFiltering_ = false;
                buffer_.clear();
                break;
            }
        }
    }
}

// Apply a low pass filter to the signal
void Strike::lowPassFilter(float cutoffFrequency, int filterOrder)
{
    for (int channel = 0; channel < numChannels_; ++channel)
    {
        float* buffer = buffer_.getWritePointer(channel);
        for (int sample = 0; sample < circularBuffer_[static_cast<size_type_>(channel)]->getBufferSize(); ++sample)
        {
            // Set filter order and frequency cutoff
            lowPassFilter_[static_cast<size_type_>(channel)]->setFilterOrder(filterOrder);
            lowPassFilter_[static_cast<size_type_>(channel)]->setFrequencyCutoff(cutoffFrequency);

            // Apply filter to buffer
            buffer[sample] = lowPassFilter_[static_cast<size_type_>(channel)]->process(buffer[sample]);
        }
    }
}

// Generates a burst exponentially decayed white noise burst
void Strike::fftModifications(float decayRate, std::vector<std::tuple<float, float, float>> freqModifications)
{
    juce::dsp::FFT fft(fftOrder_);
    std::vector<std::complex<float>> fftBuffer(static_cast<size_t>(bufferSize_));

    // Finding the maximum frequency which is the nyquistFrequency
    double maxFreq = sampleRate_ / 2.0;

    // Finding the smallest frequency difference which is the frequency resolution
    // Finding the spacing of the bins
    double minFreqDiff = sampleRate_ / numSamples_;

    std::vector<float> frequencies(static_cast<size_t>(numSamples_));

    // Maps frequency bins to frequencies, negative and positive
    for (int freqBin = 0; freqBin < static_cast<int>(frequencies.size()); ++freqBin)
    {
        /*
        If the frequency bin is less than half the size of the frequencies vector
        then the frequency is the frequency bin multiplied by the minimum frequency difference
        */
        frequencies[static_cast<size_t>(freqBin)] = (freqBin < static_cast<int>(frequencies.size())) ? (freqBin * static_cast<float>(minFreqDiff)) : (freqBin - static_cast<int>(frequencies.size())) * static_cast<float>(minFreqDiff);
    }

    auto expDecay = [decayRate, maxFreq](float freq)
    {
        return std::exp(-decayRate * std::log1p(std::abs(freq) / maxFreq) / std::log(20));
    };

    for (int channel = 0; channel < numChannels_; ++channel)
    {
        float* buffer = buffer_.getWritePointer(channel);

        // each sample is converted into a complex number
        for (size_t sample = 0; sample < static_cast<size_t>(bufferSize_); ++sample)
        {
            fftBuffer[sample] = std::complex<float>(buffer[sample], 0.0f);
        }

        fft.perform(fftBuffer.data(), fftBuffer.data(), false);

        // Apply decay factor to each frequency component
        for (size_t sample = 0; sample < static_cast<size_t>(bufferSize_); ++sample)
        {
            fftBuffer[sample] *= static_cast<float>(expDecay(frequencies[sample]));
        }

        // Apply frequency modifications
        for (const auto& modification : freqModifications)
        {
            float startFreq = std::get<0>(modification);
            float endFreq = std::get<1>(modification);
            float factor = std::get<2>(modification);

            for (size_t sample = 0; sample < static_cast<size_t>(bufferSize_); ++sample)
            {
                float frequency = std::abs(frequencies[sample]);
                if (frequency >= startFreq && frequency <= endFreq)
                {
                    fftBuffer[sample] *= factor;
                }
            }
        }
        
        // Convert frequency-domain data back to time-domain data
        fft.perform(fftBuffer.data(), fftBuffer.data(), true);

        // Copy real part of the transformed data back to the buffer
        for (size_t sample = 0; sample < static_cast<size_t>(bufferSize_); ++sample)
        {
            buffer[sample] = std::real(fftBuffer[sample]);
        }
    }
    lowPassFilter(500, 10);
   
}

// Generate N wave with the specified number of segments
std::vector<double> Strike::NWave(int numSegments) 
{
    double length = bufferSize_ / sampleRate_;
    // Getting the duration of the total length 
    std::vector<double> fullNWave(static_cast<size_t>(sampleRate_ * length * numSegments));
    std::vector<double> n_wave(static_cast<size_t>(sampleRate_ * length));
    std::vector<double> segment(static_cast<size_t>(sampleRate_ * length));
    for (size_t sample = 0; sample < static_cast<size_t>(numSamples_); ++sample) 
    {
        // Slider three is the speed 
        double t = (static_cast<float>(sample) / sampleRate_) * static_cast<float>(sliderThree_[1]); 
        float T = 1 / static_cast<float>(sampleRate_);
        // centre of the wave
        float time = 0.010155f;
        
        if (t < time) 
        {
            segment[sample] = -t/T;
        } 
        else if (t > time) 
        {
            segment[sample] = 0;
        }
    }
    for (size_t i = 0; i < static_cast<size_t>(numSegments); ++i) 
    {
        
        // Copy the data from the segment to the fullNWave
        for (size_t sample = 0; sample < static_cast<size_t>(numSamples_); ++sample) 
        {
            // Provides an N Wave with the specified number of segments
            fullNWave[sample + (i * static_cast<size_t>(numSamples_))] = segment[sample];
        }
    }
    for (int channel = 0; channel < numChannels_; ++channel)
    {
        auto* buffer = buffer_.getWritePointer(channel);
    
        // Move the data back to the normal buffer
        for (size_t sample = 0; sample < static_cast<size_t>(bufferSize_); ++sample)
        {
            // Multiply the N wave with the buffer
            buffer[sample] *= fullNWave[sample];
        }
    }
    
    return fullNWave;
}

void Strike::afterimage()
{
    // Buffer, delay 1, delay2, amplitude, feedback
    diffusion(buffer_, 10, 10, 1.0f, 0.96f);
    diffusion(buffer_, 20, 20, 0.9f, 0.00f);
    diffusion(buffer_, 30, 30, 0.8f, 0.00f);
    diffusion(buffer_, 40, 40, 0.7f, 0.00f);
    diffusion(buffer_, 50, 50, 0.6f, 0.00f);
    diffusion(buffer_, 60, 60, 0.5f, 0.00f);
    diffusion(buffer_, 70, 70, 0.4f, 0.00f);
    diffusion(buffer_, 80, 80, 0.2f, 0.00f);
    lowPassFilter(3000, 1);

}
// Perform the strike operation
void Strike::strike(float transitionRate, float expDecay, float cutoffFrequencyStart, float cutoffFrequencyEnd)
{
    buffer_.clear();

    float rand  = random_.nextFloat() * 100.0f;
    rand = juce::jlimit(0.f, 100.f, rand);

    float secondTransitionRate = transitionRate / 2.0f;
    std::array<float,4> compParams = {sliderFour_[2],sliderFive_,sliderSix_,sliderSix_ *1.2f};
 
    std::array<float,2> waveshaperParams = {sliderOne_[2],sliderTwo_[2]};
    
    // Set's the filter parameters
    setFilterParams(sliderOne_[1],sliderTwo_[1]);
    
    float intensity= juce::jmap(sliderThree_[0], -64.0f, 6.0f, 0.0005f, 0.5f);

    // Generate Modified white noise white noise
    strikeWhiteNoise(intensity);
    
    
    // Generate energy burst choosing the type of frequency modification
    // If it's close to 1 then use the first 
    if(std::abs(sliderTwo_[0] - 1) < 0.0001)
    {
        fftModifications(expDecay, freqModifications2_);
    }
    // If it's close to 2 then use the second
    else if (std::abs(sliderTwo_[0] - 2) < 0.0001)
    {
        fftModifications(expDecay, freqModifications_);
    }
    // If it's close to 3 then use the third
    else if (std::abs(sliderTwo_[0] - 3) < 0.0001)
    {
        fftModifications(expDecay, freqModifications1_);
    }
    
    // Generate energy burst with N wave
    NWave(sliderFour_[0]);
    
    // Apply varying low pass filter
    varyingLowPassFilter(secondTransitionRate, cutoffFrequencyStart, 3048);
    varyingLowPassFilter(transitionRate, cutoffFrequencyStart, cutoffFrequencyEnd);
    
    lowPassFilter(4000, 1);
    
    
    // Generate afterimage
    afterimage();

    juceLowPassFilter(buffer_, 2000, 0.1f);


    
}

// Box Muller Transformation
float Strike::gaussianCurve()
{
    float rand1  = random_.nextFloat();
    rand1 = juce::jlimit(0.01f, 1.0f, rand1);
    float rand2  = random_.nextFloat();
    rand2 = juce::jlimit(0.01f, 1.0f, rand2);
    float random = static_cast<float>(std::sqrt(-2.0 * std::log(rand1)) * std::cos(2.0 * M_PI * rand2));
    return random;
}

// Process the strike operation with additional parameters
void Strike::process(float expDecay, float cutoffFrequencyEnd)
{

    float filterCutoff = 10000;
    strike(sliderOne_[0] , expDecay, filterCutoff, cutoffFrequencyEnd);
    
    for (int channel = 0; channel < numChannels_; ++channel)
    {
        for (int samples = 0; samples < numSamples_; ++samples)
        {
            float writeSample = buffer_.getSample(channel, samples);
            circularBuffer_[static_cast<size_type_>(channel)]->reset();
            circularBuffer_[static_cast<size_type_>(channel)]->writeSample(channel, writeSample);
            float sample = circularBuffer_[static_cast<size_type_>(channel)]->readSample(channel);
            
            buffer_.setSample(channel, samples, sample);
        }
    }
    

}

void Strike::setSliders(std::array<float,numSliders> value, float slider)
{
    // Set the slider values
    sliderOne_[static_cast<size_t>(slider)] = value[0];
    sliderTwo_[static_cast<size_t>(slider)] = value[1];
    sliderThree_[static_cast<size_t>(slider)] = value[2];
    sliderFour_[static_cast<size_t>(slider)] = value[3];
    
    sliderFive_ = value[4];

    sliderSix_ = value[5];
    
}

// Reset previous history of filter
void Strike::reset()
{
    buffer_.clear();
    buffer_.setSize(numChannels_, bufferSize_);
}

// Destructor
Strike::~Strike()
{

}