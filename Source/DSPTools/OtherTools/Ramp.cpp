#include <cmath>
#include "Ramp.h"

// Qmul Lecture Class code, Module Audio Programming bela C++
// Constructor
Ramp::Ramp()
{
	currentValue_ = 0;
	increment_ = 0;
	counter_ = 0;
	sampleRate_ = 1;
	active_ = true;
}
	
// Constructor specifying a sample rate
Ramp::Ramp(float sampleRate)
{
	currentValue_ = 0;
	increment_ = 0;
	counter_ = 0;
	sampleRate_ = sampleRate;
	active_ = true;
}
	
// Set the sample rate, used for all calculations
void Ramp::setSampleRate(float rate)
{
	sampleRate_ = rate;	
}
	
// Jump to a value
void Ramp::setValue(float value)
{
	currentValue_ = value;
	increment_ = 0;
	counter_ = 0;
	active_ = false; 
}
	
// Ramp to a value over a period of time
void Ramp::rampTo(float value, float time)
{
	if (time > 0) {
        increment_ = (value - currentValue_) / (sampleRate_ * time);
        counter_ = static_cast<int>(sampleRate_ * time);
        active_ = true;
    } else {
        // Directly set the value if time is zero to avoid clicks
        setValue(value);
    }

}
	
// Generate and return the next ramp output
float Ramp::process()
{
	if (active_ && counter_ > 0) 
	{
        counter_--;
        currentValue_ += increment_;
        if (counter_ == 0) {
            active_ = false;  // Stop updating when ramping is finished
        }
    }
	return currentValue_;
}
	
// Return whether the ramp is finished
bool Ramp::finished()
{
	// The ramp is finished when the counter has counted down to 0
	return (counter_ == 0);
}	

float Ramp::getDuration() 
{
        return counter_ / sampleRate_;
}

float Ramp::getCurrentValue() const 
{
    return currentValue_;
}


// Destructor
Ramp::~Ramp()
{
}