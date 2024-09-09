#pragma once
#include <JuceHeader.h>

class CircularBuffer
{
public:
    // Initalise the buffer to the given size
    CircularBuffer(int size, juce::AudioBuffer<float> buffer,  int numChannels);

    // Write the sample to the buffer at the current write position
    void writeSample(int channel,float sample);

    // Read the sample from the buffer at the current read position
    float readSample(int channel);

    // Set the readPosition based on the desired delay in samples.
    void setReadPosition(int channel,float delaySamples);

    void setWritePosition(int channel, float delaySamples);

    // Reset the buffer by setting all its values to 0.
    void reset();

    // Gets the buffer size
    int getBufferSize() const { return bufferSize_; }

    // Sets the buffer size
    void setBufferSize(int size) { bufferSize_ = size; }

private:

    int wrapIndex(int index) const 
    {
        return (index + bufferSize_) % bufferSize_;
    }

    juce::AudioBuffer<float> buffer_;
    int bufferSize_;
    int numChannels_;
    std::vector<int> writePosition_;
    std::vector<int> readPosition_;
    std::vector<float> fractionalDelay_;
    std::vector<std::vector<int>> readPositions_;
    std::vector<std::vector<float>> fractionalDelays_;
    using index_ = std::vector<int>::size_type;
};