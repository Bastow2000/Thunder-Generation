#include "CircularBuffer.h"
#include <iostream>

// Initalise the buffer to the given size
CircularBuffer::CircularBuffer(int bufferSize, juce::AudioBuffer<float> buffer, int numChannels)
: 
buffer_(numChannels, bufferSize),
bufferSize_(bufferSize), 
numChannels_(numChannels),
writePosition_(static_cast<index_>(numChannels), 0), 
readPosition_(static_cast<index_>(numChannels), 0),
fractionalDelay_(static_cast<index_>(numChannels), 0),
readPositions_(static_cast<index_>(numChannels), std::vector<int>(10, 0)),
fractionalDelays_(static_cast<index_>(numChannels), std::vector<float>(10, 0.0f))
{
    buffer_ = buffer; 
    buffer_.clear();
}

// Write the sample to the buffer at the current write position
// Write a sample to the circular buffer: x[n]
void CircularBuffer::writeSample(int channel, float sample)
{
    // Calculate the next write position
    int nextWritePos = (writePosition_[static_cast<index_>(channel)] + 1) % bufferSize_;

    // Check for overrun: if the next write position is the same as the read position, skip writing
    if (nextWritePos == readPosition_[static_cast<index_>(channel)]) 
    {
        // Optionally handle buffer overrun (e.g., log an error or handle it some other way)
        return; // Skip writing to avoid overwriting unread data
    }

    // Write the sample to the buffer
    float* channelData = buffer_.getWritePointer(channel);
    channelData[writePosition_[static_cast<index_>(channel)]] = sample;

    // Move write position forward
    writePosition_[static_cast<index_>(channel)] = nextWritePos;
}

// Set delay for the write position
void CircularBuffer::setWritePosition(int channel, float delaySamples)
{
    
    int intDelay = static_cast<int>(delaySamples);

    writePosition_[static_cast<index_>(channel)] = (intDelay + writePosition_[static_cast<index_>(channel)] ) % bufferSize_;

}

// Read the sample from the buffer at the current read position
float CircularBuffer::readSample(int channel)
{
    // Check for underrun: if the read position is the same as the write position, return 0 or a default value
    if (readPosition_[static_cast<index_>(channel)] == writePosition_[static_cast<index_>(channel)]) 
    {
        // Optionally handle buffer underrun (e.g., log an error or handle it some other way)
        return 0.0f; // Return default value if no data is available to read
    }

    // Get the current sample at the read position
    const float* channelData = buffer_.getReadPointer(channel);
    float sample1 = channelData[readPosition_[static_cast<index_>(channel)]];

    // Get the next sample for interpolation
    float sample2 = channelData[(readPosition_[static_cast<index_>(channel)] + 1) % bufferSize_];

    // Interpolate between the two samples
    float interpolatedSample = sample1 + fractionalDelay_[static_cast<index_>(channel)] * (sample2 - sample1);

    // Move read position forward
    readPosition_[static_cast<index_>(channel)] = (readPosition_[static_cast<index_>(channel)] + 1) % bufferSize_;

    return interpolatedSample;
}

// Set the readPosition based on the desired delay in samples.
// This creates the delay effect.
void CircularBuffer::setReadPosition(int channel, float delaySamples)
{
    int intDelay = static_cast<int>(delaySamples);
    float fracDelay = delaySamples - intDelay;

    readPosition_[static_cast<index_>(channel)] = (writePosition_[static_cast<index_>(channel)] - intDelay + bufferSize_) % bufferSize_;
    fractionalDelay_[static_cast<index_>(channel)] = fracDelay;

}

// Reset the buffer by setting all its values to 0.
// This is where x[n] samples are cleared/reset in the circular buffer
void CircularBuffer::reset()
{
    buffer_.clear();
    for (int channel = 0; channel < numChannels_; ++channel)
    {
        writePosition_[static_cast<index_>(channel)] = 0;
        readPosition_[static_cast<index_>(channel)] = 0;
    }
}

