#include "SliderDetails.h"

SliderDetails::SliderDetails()
{
    setLookAndFeel (&lf_);
    setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    setPopupDisplayEnabled (true, false, this);
    setTextValueSuffix (" dB");
    setVelocityBasedMode (true);
    setVelocityModeParameters (0.09, 1, 0.5, false);
    setSliderStyle (SliderStyle::LinearHorizontal);
}

SliderDetails::~SliderDetails()
{
    setLookAndFeel (nullptr);
}

bool SliderDetails::hitTest (int x, int y)
{
    float scalingFactor = 0.1f; 
    int dia = std::min (getWidth(), getHeight()) * scalingFactor;
    return getLocalBounds().getCentre().getDistanceFrom ({ x, y }) < dia;
}

void SliderDetails::mouseDoubleClick (const juce::MouseEvent&)
{
    // Set the slider value back to the default value
    setValue (0.0, juce::NotificationType::sendNotification);
}

void SliderDetails::setColour (juce::Colour colourToSet)
{
    lf_.setSliderColour (colourToSet);
}
