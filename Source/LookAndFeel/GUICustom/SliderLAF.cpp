#include "SliderLAF.h"

// Self Referencing Code taken from previous project Bastow Synth to set up components modified for this project
SliderLAF::SliderLAF()
{
}

SliderLAF::~SliderLAF()
{
}

void SliderLAF::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle /*style*/, juce::Slider& slider)
{
    auto trackWidthGain1 = juce::jmin(10.5f, static_cast<float>(height));

    juce::Point<float> startPoint(slider.isHorizontal() ? static_cast<float>(x) : static_cast<float>(x) + static_cast<float>(width) * 0.5f,
                                  slider.isHorizontal() ? static_cast<float>(y) + static_cast<float>(height) * 0.5f : static_cast<float>(height + y));

    juce::Point<float> endPoint(slider.isHorizontal() ? static_cast<float>(width + x) : startPoint.x,
                                slider.isHorizontal() ? startPoint.y : static_cast<float>(y));

    juce::Path track;
    track.startNewSubPath(startPoint);
    track.lineTo(endPoint);

    g.setColour(mainSliderColour_.brighter(0.5f));
    g.strokePath(track, juce::PathStrokeType(trackWidthGain1));

    auto brightTrackWidthGain1 = juce::jmin(10.5f, static_cast<float>(height));
    juce::Path brightTrack;
    juce::Point<float> minPoint, maxPoint, thumbPoint;

    minPoint = { minSliderPos, static_cast<float>(height) * 0.5f };
    maxPoint = { maxSliderPos, static_cast<float>(height) * 0.5f };
    thumbPoint = { sliderPos, static_cast<float>(height) * 0.5f };

    auto kx = slider.isHorizontal() ? sliderPos : (static_cast<float>(x) + static_cast<float>(width) * 0.5f);
    auto ky = slider.isHorizontal() ? (static_cast<float>(y) + static_cast<float>(height) * 0.5f) : sliderPos;

    minPoint = startPoint;
    maxPoint = { kx, ky };

    auto minThumbWidth = getSliderThumbRadius(slider) - 7.8;
    auto maxThumbWidth = getSliderThumbRadius(slider) - 8.6f;

    brightTrack.startNewSubPath(minPoint);
    brightTrack.lineTo(maxPoint);

    // Set a fallback color for grey
    g.setColour(mainSliderColour_.brighter(0.05f));

    g.strokePath(brightTrack, juce::PathStrokeType(brightTrackWidthGain1));

    float lineOffset = 15.0f; 

    // Upper static line
    g.setColour(mainSliderColour_.brighter(0.01));
    g.drawLine(x, y + height / 2 - lineOffset, x + width, y + height / 2 - lineOffset, 1.0f);

    // Lower static line
    g.setColour(mainSliderColour_.brighter(0.01));
    g.drawLine(x, y + height / 2 + lineOffset, x + width, y + height / 2 + lineOffset, 1.0f);
}

float SliderLAF::thumbPosition(int x)
{
    return x ? maxPoint_ : maxPoint_;
}