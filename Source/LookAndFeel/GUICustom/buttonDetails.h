#pragma once

#include <JuceHeader.h>
#include "buttonLAF.h"

class ButtonDetails : public juce::TextButton
{
public:
    ButtonDetails();

    ~ButtonDetails();

    void setColour (juce::Colour colourToSet);
    void setText (juce::String text);

    

private:
    ButtonLAF lf_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ButtonDetails)
};

