#pragma once

#include "../../PluginProcessor.h"
#include "../../ParamIds.h"
#include "../GUICustom/SliderLAF.h"
#include "../GUICustom/SliderDetails.h"
#include "../GUICustom/buttonLAF.h"
#include "../GUICustom/buttonDetails.h"

#include "ShapeComponent.h"

#include <JuceHeader.h>

class ThunderGenerationAudioProcessorEditor;

class ThunderComponent : public ShapeComponent, private juce::Timer
{
public:
    ThunderComponent(ThunderGenerationAudioProcessor& p);
    ~ThunderComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    bool visualThunder = false;

private:
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;

    void createBranch(float startX, float startY);
    

    juce::Path lightningPath_;
    std::vector<juce::Path> branches_;
    float startX_ = 0.0f;
    float startY_ = 0.0f;
    float currentX_ = 0.0f;
    float currentY_ = 0.0f;
    int segmentsAdded_ = 0;
    const int maxSegments_ = 100;
    juce::Rectangle<int> boundingBox_;

    ThunderGenerationAudioProcessor& audioProcessor_;

    juce::Image background_;

    juce::Random random_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThunderComponent)
};
