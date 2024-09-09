/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ParamIds.h"
#include "ContentComponent.h"
#include "ShapeComponent.h"
#include "EffectsComponent.h"

#include "ThunderComponent.h"
#include "buttonDetails.h"
//==============================================================================
/**
*/
class ThunderGenerationAudioProcessorEditor  :  public  juce::AudioProcessorEditor,   public juce::Button::Listener, private juce::Timer, public juce::Slider::Listener
{
public:
    ThunderGenerationAudioProcessorEditor (ThunderGenerationAudioProcessor&);
    ~ThunderGenerationAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked (juce::Button* button) override;
    
    void updateLabels();

    int counter = 0;
private:
    void updateButtonText();
    void sliderValueChanged (juce::Slider* slider) override;

    // Add this timer callback
    void timerCallback() override 
    {
        if (audioProcessor.bufferHasFinished) 
        {
            startThunder.setToggleState(false, juce::dontSendNotification);
            audioProcessor.bufferHasFinished = false; // Reset the flag
            audioProcessor.bufferIsPlaying = false; // Reset the flag
          
            stopTimer(); // Stop the timer as we only need to reset once
        }
    }

    std::array<float,4> sliderLabelOne_;
    std::array<float,4> sliderLabelTwo_;
    std::array<float,2> sliderLabelThree_;
    std::array<float,2> sliderLabelFour_;
    float sliderLabelFive_;
    float sliderLabelSix_;
  //  float sliderLabelOne_;
   // float sliderLabelTwo_;
    //float sliderLabelThree_;
   // float sliderLabelFour_;
    
    
    ThunderGenerationAudioProcessor& audioProcessor;
    juce::Image background_;
    ButtonDetails startThunder;
    ShapeComponent strikeComponent_;
    ContentComponent rumbleComponent_;
    EffectsComponent crackleComponent_;
    ThunderComponent thunderComponent_;

   

    static const size_t mainNumSliders_ {2};

    std::array<int,numSliders> sliderY_ { 40, 40, 0, 0,0,0};
    std::array<int,numSliders> sliderX_ { 132, 342, 0, 0,0,0};

    std::vector<std::unique_ptr<SliderDetails>> sliderArray_;
    std::vector<SliderDetails> sliders_;

    friend class shapeComponent;

    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>>sliderAttachments_;
    std::array<int,(numSliders+1)*2> textPositions_ { 165, 175, 375, 175, 0,0, 0,0,0,0,0,0,0,0};
    std::array<juce::Colour, numSliders> colourScheme_ { OtherColours::lightGrey, OtherColours::lightGrey, ShapeColours::lime, ShapeColours::cyan, ShapeColours::cyan, ShapeColours::cyan };
    
    std::array<juce::String, numSliders+1> sliderNames_ { "Distance", "Volume", "unused", "unused", "unused","unused","unused" };
    std::array<juce::String, numSliders> sliderId_ {mainSliderOneID,mainSliderTwoID,"unused","unused", "unused","unused"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThunderGenerationAudioProcessorEditor)
};
