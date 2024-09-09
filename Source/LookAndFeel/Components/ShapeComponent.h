#pragma once

#include "../../PluginProcessor.h"
#include "../../ParamIds.h"
#include "../GUICustom/SliderLAF.h"
#include "../GUICustom/SliderDetails.h"
#include "../GUICustom/buttonLAF.h"
#include "../GUICustom/buttonDetails.h"
#include "../SubModules/Strike.h"
#include <JuceHeader.h>

class ShapeComponent : virtual public juce::Component, public juce::Slider::Listener
{
public:
    ShapeComponent (ThunderGenerationAudioProcessor& p);
    ~ShapeComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    // Helper Functions
    void sliderSetup(int numSlidersChoice,std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>>& sliderAttachment, std::vector<SliderDetails>& slider, std::vector<std::unique_ptr<SliderDetails>>& sliderArray,std::array<juce::Colour, numSliders>& colourScheme, std::array<juce::String, numSliders>& sliderId);
    void componentSetup(int numSlidersChoice, juce::Graphics& g,juce::Image& background,std::array<juce::String, numSliders +1>& sliderNames,std::array<int,(numSliders +1) *2>& textPositions );
    void guiLocation(int numSlidersChoice, std::vector<SliderDetails>& slider,  std::array<int, numSliders> sliderY, std::array<int, numSliders> sliderX);
 
    // Update the label
    void updateLabels();

    // Set the labels for the slider
    void setLabelSliderOne(float value);
    void setLabelSliderTwo(float value);
    void setLabelSliderThree(float value);
    void setLabelSliderFour(float value);

    
    
   

private:
    // When the slider is changed, the value and label is updated
    void sliderValueChanged (juce::Slider* slider) override;
    ThunderGenerationAudioProcessor& audioProcessor_;
    juce::Image background_;

    static const size_t shapeNumSliders_ {4};

    std::array<int,numSliders> sliderY_ { -40, 50, 145, 240,0,0};
    std::array<int,numSliders> sliderX_ { -8, -8, -8, -8,0,0};

    std::vector<std::unique_ptr<SliderDetails>> sliderArray_;
    std::vector<SliderDetails> sliders;

    float sliderLabelOne_;
    float sliderLabelTwo_;
    float sliderLabelThree_;
    float sliderLabelFour_;

    

   
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>>sliderAttachments_;
    std::array<int,(numSliders+1)*2> textPositions_ { 25, 30, 25, 92, 30, 180, 15, 277,20,372,0,0,0,0};
    std::array<juce::Colour, numSliders> colourScheme_ { ShapeColours::red, ShapeColours::orange, ShapeColours::lime, ShapeColours::cyan, ShapeColours::cyan, ShapeColours::cyan };
    
    std::array<juce::String, numSliders+1> sliderNames_ { "Shape", "Length", "Pitch Type", "Noise Stength", "Segments","unused","unused" };
    std::array<juce::String, numSliders> sliderId_ {shapeSliderOneID,shapeSliderTwoID,shapeSliderThreeID,shapeSliderFourID, "unused","unused"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShapeComponent)
};
