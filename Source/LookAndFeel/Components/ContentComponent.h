#pragma once

#include "../../PluginProcessor.h"
#include "../../ParamIds.h"
#include "../GUICustom/SliderLAF.h"
#include "../GUICustom/SliderDetails.h"
#include "../GUICustom/buttonLAF.h"
#include "../GUICustom/buttonDetails.h"
#include "ShapeComponent.h"

#include <JuceHeader.h>

class ContentComponent : public ShapeComponent
{
public:
    ContentComponent (ThunderGenerationAudioProcessor& p);
    ~ContentComponent() override;

    // Class virtual memeber functions
    void paint (juce::Graphics&) override;
    void resized() override;

    // Update the label
    void updateLabel();

    // Set the label for the slider
    void setLabelSliderOne(float value);
    void setLabelSliderTwo(float value);
    void setLabelSliderThree(float value);
    void setLabelSliderFour(float value);

private:
    // When the slider is changed, the value and label is updated
    void sliderValueChanged (juce::Slider* slider) override;
  
    ThunderGenerationAudioProcessor& audioProcessor_;
    std::vector<SliderDetails> sliders_;
    juce::Image background_;

    float sliderLabelOne_;
    float sliderLabelTwo_;
    float sliderLabelThree_;
    float sliderLabelFour_;

    static const size_t contentNumSliders_ {4};


    std::vector<std::unique_ptr<SliderDetails>> sliderArray_;
    std::array<int,numSliders> sliderY_ { -40, 50, 145, 240,0,0};
    std::array<int,numSliders> sliderX_ { -8, -8, -8, -8,0,0};

    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>>sliderAttachments_;

    std::array<int,(numSliders+1)*2> textPositions_ {18, 30, 10, 92, 15, 180, 15, 277,0,372,0,0,0,0};
    std::array<juce::Colour, numSliders> colourScheme_ { ContentColours::pink, ContentColours::violet, ContentColours::purple, ContentColours::blue, ContentColours::blue, ContentColours::blue };
    std::array<juce::String, numSliders+1> sliderNames_ { "Content", "LowPass", "HighPass", "Proximity", "FeedbackDelay","unused","unused" };
    std::array<juce::String, numSliders> sliderId_ {contentSliderOneID,contentSliderTwoID,contentSliderThreeID,contentSliderFourID,"unused","unused"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ContentComponent)
};