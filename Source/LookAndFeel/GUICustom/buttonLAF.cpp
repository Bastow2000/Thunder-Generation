#include"buttonLAF.h"

ButtonLAF::ButtonLAF()
{
   static auto roman = juce::Typeface::createSystemTypefaceFor (
        BinaryData::AvenirRoman_ttf, BinaryData::AvenirRoman_ttfSize);
}

ButtonLAF::~ButtonLAF()
{
}

void ButtonLAF::drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto buttonArea = button.getLocalBounds().toFloat().reduced(0.5f);
    auto edge = 4.0f;

    juce::Colour baseColour = mainColour_;
    if (shouldDrawButtonAsDown)
        baseColour = baseColour.darker(0.2f);
    else if (shouldDrawButtonAsHighlighted)
        baseColour = baseColour.brighter(0.2f);

    g.setColour(baseColour);
    g.fillRoundedRectangle(buttonArea, edge);

    g.setColour(baseColour.contrasting(0.2f));
    g.drawRoundedRectangle(buttonArea, edge, 1.0f);

}

void ButtonLAF::drawButtonText(juce::Graphics& g, juce::TextButton& button, bool /*shouldDrawButtonAsHighlighted*/, bool /*shouldDrawButtonAsDown*/) 
{
    static auto roman = juce::Typeface::createSystemTypefaceFor (
        BinaryData::AvenirRoman_ttf, BinaryData::AvenirRoman_ttfSize);
    g.setFont(roman);
    g.setFont(12);
    g.setColour(juce::Colours::white);

    auto text = button.getButtonText();
    g.drawText(text, button.getLocalBounds(), juce::Justification::centred, true);
}