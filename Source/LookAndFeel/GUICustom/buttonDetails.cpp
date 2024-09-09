#include "buttonDetails.h"

ButtonDetails::ButtonDetails()
{
    setLookAndFeel(&lf_);
    
    setToggleState(true, juce::NotificationType::dontSendNotification);
    setClickingTogglesState(true);
}

ButtonDetails::~ButtonDetails()
{
    setLookAndFeel(nullptr);
}

void ButtonDetails::setText(juce::String text)
{
    setButtonText(text);
}

void ButtonDetails::setColour(juce::Colour colourToSet)
{
    lf_.setButtonColour(colourToSet);
    repaint();
}