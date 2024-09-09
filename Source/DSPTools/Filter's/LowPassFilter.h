#pragma once
#include <array>
class LowPassFilter 
{
public:
	// Default Constructor
    LowPassFilter();

	// Constructor with samplerate & frequency cutoff
    LowPassFilter(double sampleRate, float frequencyCutoff, int filterOrder);

	// Default Deconstructor 
    ~LowPassFilter();

	// Setup private variables 
	void setup(double sampleRate, float frequencyCutoff, int filterOrder);

	// Set frequencyCutoff
    void setFrequencyCutoff(float frequency);

    // Set Filter Order
    void setFilterOrder(float filterOrder);

	// Set transitionRate for filter cutoff
    void setTransitionRate(float rate);

	// Obtain Cutoff Frequency
    float getCutoffFrequency() const;

	// Process lower pass filter
    float process(float input);

	// Reset filter 
    void reset();

	// Public Variables
	float lastCutoffFrequency_ = -1;

	// Default transition rate
	float transitionRate_ = 1.0;  

private:
    void calculateCoefficients(float frequency);

    double sampleRate_;
	float inverseSampleRate_;
    float cutoffFrequency_;
    int filterOrder_ ; 


    bool ready_;
    
    std::array<float, 10> b0_;
    std::array<float, 10> b1_;
    std::array<float, 10> a1_;

    std::array<float, 10> lastX_;
    std::array<float, 10> lastY_;

    float lastX_1st;
    float lastY_1st;

    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LowPassFilter)
};