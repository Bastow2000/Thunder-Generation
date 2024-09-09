#pragma once

#include "../../PluginProcessor.h"
#include "../../ParamIds.h"
#include "../GUICustom/SliderLAF.h"
#include "../GUICustom/SliderDetails.h"
#include "../GUICustom/buttonLAF.h"
#include "../GUICustom/buttonDetails.h"
#include "ShapeComponent.h"

#include <JuceHeader.h>
// Inherits from ShapeComponent to reduce repetitive code
class EffectsComponent : public ShapeComponent
{
public:
    EffectsComponent (ThunderGenerationAudioProcessor& p);
    ~EffectsComponent() override;

    // Class virtual memeber functions
    void paint (juce::Graphics&) override;
    void resized() override;

    // Update the label
    void updateLabel();

    // Set the labels for the slider
    void setLabelSliderOne(float value);
    void setLabelSliderTwo(float value);
    void setLabelSliderThree(float value);
    void setLabelSliderFour(float value);
    void setLabelSliderFive(float value);
    void setLabelSliderSix(float value);


private:
    // When the slider is changed, the value and label is updated
    void sliderValueChanged (juce::Slider* slider) override;

    ThunderGenerationAudioProcessor& audioProcessor_;
    std::vector<SliderDetails> sliders_;
    juce::Image background_;

    std::vector<std::unique_ptr<SliderDetails>> sliderArray_;

    float sliderLabelOne_;
    float sliderLabelTwo_;
    float sliderLabelThree_;
    float sliderLabelFour_;
    float sliderLabelFive_;
    float sliderLabelSix_;
    static const size_t effectNumSliders_ {6};
  
    
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> sliderAttachments_;
    std::array<int,(numSliders+1)*2> textPositions_ { 125, 10, 10, 70, 10, 140, 15, 208, 168, 70, 180, 140, 180, 208};
    std::array<int,numSliders> sliderY_ { -65,5,75, -65,5,75};
    std::array<int,numSliders> sliderX_ { 15, 15, 15, 175,175,175};
    std::array<juce::Colour, numSliders> colourScheme_ { OtherColours::grey, OtherColours::grey, OtherColours::grey, OtherColours::grey, OtherColours::grey, OtherColours::grey };
    
    std::array<juce::String, numSliders+1> sliderNames_ { "Effects", "Reverb: RoomSize", "Reverb: Wet", "Reverb: Dry", "Compression: Threshold","Compression: Ratio","Compression: Attack"};
    std::array<juce::String, numSliders> sliderId_ {effectsSliderOneID,effectsSliderTwoID,effectsSliderThreeID,effectsSliderFourID,effectsSliderFiveID,effectsSliderSixID};


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EffectsComponent)
};