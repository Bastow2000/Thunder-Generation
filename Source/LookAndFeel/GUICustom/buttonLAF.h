#pragma once
#include "../../ParamIds.h"

#include <JuceHeader.h>

class ButtonLAF : public juce::LookAndFeel_V4
{
public:
    ButtonLAF();
    ~ButtonLAF() override;

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool /*shouldDrawButtonAsHighlighted*/, bool /*shouldDrawButtonAsDown*/) override;


    void setButtonColour (juce::Colour colourToUse)
    {
        mainColour_ = colourToUse;
    }

    void setButtonText (std::string textToUse)
    {
        mainText_ = textToUse;
    }


    void setButtonFont (juce::Font fontToUse)
    {
        mainFont_ = fontToUse;
    }

private:
 
    std::string mainText_ = " hello ";
    juce::Font mainFont_ = 10.f;
    juce::Colour mainColour_ = juce::Colours::transparentBlack;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ButtonLAF)
};
