#pragma once

#include <JuceHeader.h>
#include "SliderLAF.h"

class SliderDetails : public juce::Slider
{
public:
    SliderDetails();

    ~SliderDetails();

    bool hitTest (int x, int y);

    void setColour (juce::Colour colourToSet);

    void mouseDoubleClick (const juce::MouseEvent&);

private:
    SliderLAF lf_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderDetails)
};

