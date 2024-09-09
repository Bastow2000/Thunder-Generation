
#pragma once
#include "../../ParamIds.h"

#include <JuceHeader.h>

class SliderLAF : public juce::LookAndFeel_V4
{
public:
    SliderLAF();
    ~SliderLAF() override;

    void drawLinearSlider (juce::Graphics&, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider&) override;

    void setSliderColour (juce::Colour colourToUse)
    {
        mainSliderColour_ = colourToUse;
    }
    float thumbPosition (int x);

private:
    float maxPoint_ = 0;
    float b_;
    float minimum_;
    float maximum_;
    float trackWidthGainBackground1_;
    float trackWidthGain1_;
    float brightTrackWidthGain1_;
    float minThumbWidth_;
    float maxThumbWidth_;
    juce::Colour mainSliderColour_ = juce::Colours::transparentBlack;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderLAF)
};
