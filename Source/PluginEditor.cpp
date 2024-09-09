/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ThunderGenerationAudioProcessorEditor::ThunderGenerationAudioProcessorEditor (ThunderGenerationAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), strikeComponent_(audioProcessor), rumbleComponent_(audioProcessor), 
    crackleComponent_(audioProcessor), thunderComponent_(audioProcessor), sliderArray_ (mainNumSliders_), sliders_(mainNumSliders_), sliderAttachments_ (mainNumSliders_)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 500);
    startTimer(100);
    addAndMakeVisible(startThunder); 
    addAndMakeVisible(strikeComponent_);
    addAndMakeVisible(rumbleComponent_);
    addAndMakeVisible(crackleComponent_);
    
    addAndMakeVisible(thunderComponent_);

    
    sliderLabelOne_[0] = audioProcessor.sliderControl(sliderLabelOne_[0], shapeSliderOneID);
    sliderLabelTwo_[0]= audioProcessor.sliderControl(sliderLabelTwo_[0], shapeSliderTwoID);
    sliderLabelThree_[0] = audioProcessor.sliderControl(sliderLabelThree_[0], shapeSliderThreeID);
    sliderLabelFour_[0] = audioProcessor.sliderControl(sliderLabelFour_[0], shapeSliderFourID);
    
    sliderLabelOne_[1] = audioProcessor.sliderControl(sliderLabelOne_[1], contentSliderOneID);
    sliderLabelTwo_[1]= audioProcessor.sliderControl(sliderLabelTwo_[1], contentSliderTwoID);
    sliderLabelThree_[1] = audioProcessor.sliderControl(sliderLabelThree_[1], contentSliderThreeID);
    sliderLabelFour_[1] = audioProcessor.sliderControl(sliderLabelFour_[1], contentSliderFourID);

    sliderLabelOne_[2] = audioProcessor.sliderControl(sliderLabelOne_[2], effectsSliderOneID);
    sliderLabelTwo_[2]= audioProcessor.sliderControl(sliderLabelTwo_[2], effectsSliderTwoID);
    sliderLabelThree_[2] = audioProcessor.sliderControl(sliderLabelThree_[2], effectsSliderThreeID);
    sliderLabelFour_[2] = audioProcessor.sliderControl(sliderLabelFour_[2], effectsSliderFourID);
    sliderLabelFive_ = audioProcessor.sliderControl(sliderLabelFive_, effectsSliderFiveID);
    sliderLabelSix_ = audioProcessor.sliderControl(sliderLabelSix_, effectsSliderSixID);

    sliderLabelOne_[3]= audioProcessor.sliderControl(sliderLabelOne_[3], mainSliderOneID);
    sliderLabelTwo_[3]= audioProcessor.sliderControl(sliderLabelTwo_[3], mainSliderTwoID);

    strikeComponent_.setLabelSliderOne(sliderLabelOne_[0]);
    strikeComponent_.setLabelSliderTwo(sliderLabelTwo_[0]);
    strikeComponent_.setLabelSliderThree(sliderLabelThree_[0]);
    strikeComponent_.setLabelSliderFour(sliderLabelFour_[0]);

    rumbleComponent_.setLabelSliderOne(sliderLabelOne_[1]);
    rumbleComponent_.setLabelSliderTwo(sliderLabelTwo_[1]);
    rumbleComponent_.setLabelSliderThree(sliderLabelThree_[1]);
    rumbleComponent_.setLabelSliderFour(sliderLabelFour_[1]);

    crackleComponent_.setLabelSliderOne(sliderLabelOne_[2]);
    crackleComponent_.setLabelSliderTwo(sliderLabelTwo_[2]);
    crackleComponent_.setLabelSliderThree(sliderLabelThree_[2]);
    crackleComponent_.setLabelSliderFour(sliderLabelFour_[2]);
    crackleComponent_.setLabelSliderFive(sliderLabelFive_);
    crackleComponent_.setLabelSliderSix(sliderLabelSix_);
    
    startThunder.setButtonText("Thunder");
    startThunder.setColour(StrikeColours::grey);
    startThunder.addListener(this);    
    startThunder.isAlwaysOnTop();
    startThunder.setAlwaysOnTop (true);

    
    for (std::size_t i = 0; i < mainNumSliders_; ++i)
    {
        sliderArray_[i] = std::make_unique<SliderDetails>();
    }

    const auto getGainSliderColour = [&] (int index)
    {
        return colourScheme_[static_cast<size_t>(index)];
    };

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    static const auto createSliderAttachment =
        [] (juce::AudioProcessorValueTreeState& tree,
            std::unique_ptr<SliderAttachment>& attachment,
            juce::String paramID,
            juce::Slider& slider)
    {
        attachment = std::make_unique<SliderAttachment> (tree, paramID, slider);
    };

    for (auto i = 0; i < static_cast<int>(mainNumSliders_); ++i)
    {
        sliders_[static_cast<std::vector<SliderDetails>::size_type> (i)]
            .addListener (this);
        sliders_[static_cast<std::vector<SliderDetails>::size_type> (i)]
            .setColour (getGainSliderColour (static_cast<int> (i)));

        createSliderAttachment(audioProcessor.tree, sliderAttachments_[static_cast<std::vector<SliderDetails>::size_type> (i)], sliderId_[static_cast<std::vector<juce::String>::size_type>(i)], sliders_[static_cast<std::vector<SliderDetails>::size_type> (i)]);

        addAndMakeVisible (sliders_[static_cast<std::vector<SliderDetails>::size_type> (i)]);
    }
    
    
}

ThunderGenerationAudioProcessorEditor::~ThunderGenerationAudioProcessorEditor()
{
    startThunder.removeListener(this);
    for (auto i = 0; i < static_cast<int>(mainNumSliders_); ++i)
    {
        sliders_[static_cast<std::vector<SliderDetails>::size_type> (i)]
            .removeListener (this);
    }
}

//==============================================================================
void ThunderGenerationAudioProcessorEditor::paint (juce::Graphics& g)
{
     background_ = juce::ImageCache::getFromMemory (BinaryData::Background_png,
                                                   BinaryData::Background_pngSize);
    g.drawImageWithin (background_, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);

    
   static auto book = juce::Typeface::createSystemTypefaceFor (
        BinaryData::AvenirBook_ttf, BinaryData::AvenirBook_ttfSize);
    g.setColour (juce::Colours::white);
    g.setFont (book);
    g.setFont (11.5f);

    juce::Rectangle<int> a = getLocalBounds().withPosition ({ textPositions_[0], textPositions_[1] });
    g.drawFittedText (sliderNames_[0], a, juce::Justification::topLeft, false);

    juce::Rectangle<int> b = getLocalBounds().withPosition ({ textPositions_[2], textPositions_[3] });
    g.drawFittedText (sliderNames_[1], b, juce::Justification::topLeft, false);
    
}

void ThunderGenerationAudioProcessorEditor::resized()
{
   
    auto a = getLocalBounds().withWidth (115).withHeight (460).withX (22.f);
    strikeComponent_.setBounds (a.withY (20));
    auto b = getLocalBounds().withWidth (115).withHeight (460).withX (465.f);
    rumbleComponent_.setBounds (b.withY (20));
    
    auto c = getLocalBounds().withWidth (320).withHeight (265).withX (142.f);
    crackleComponent_.setBounds (c.withY (224));
    
    auto e = getLocalBounds().withWidth (320).withHeight (150).withX (142);
    thunderComponent_.setBounds (e.withY (20));
    auto f = getLocalBounds().withWidth (75).withHeight (25).withX (265);
    startThunder.setBounds (f.withY (185));

    
    for (unsigned int i = 0; i < static_cast<size_t>(mainNumSliders_); ++i)
    {
        auto g = getLocalBounds().withWidth (130).withHeight (325).withX (
            static_cast<int> (sliderX_[i]));
        sliders_[i].setBounds (g.withY (sliderY_[i]));
       
    }
    
   
}

void ThunderGenerationAudioProcessorEditor::buttonClicked (juce::Button* button)
{

    if (button == &startThunder) 
    {
        if (!audioProcessor.generateNoise)
        {
            audioProcessor.startNoise();
            audioProcessor.thunderOnly = true;
            thunderComponent_.visualThunder = true;
        }
        else
        {
            audioProcessor.stopNoise();
            audioProcessor.thunderOnly = false;
            thunderComponent_.visualThunder = false;
            audioProcessor.resetNoise();
        }
    }

}

void ThunderGenerationAudioProcessorEditor::updateLabels()
{
    sliderNames_[0] = juce::String::formatted("Volume: %.0f ", sliderLabelOne_[3]);
    sliderNames_[1] = juce::String::formatted("Distance: %.0f ", sliderLabelTwo_[3]);
 
    repaint(); 
}


void ThunderGenerationAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    
    if (slider == &sliders_[static_cast<std::vector<SliderDetails>::size_type> (0)]) 
    {
        sliderLabelOne_[3] = static_cast<float>(slider->getValue());
        updateLabels();
    }
    if (slider == &sliders_[static_cast<std::vector<SliderDetails>::size_type> (1)]) 
    {
        sliderLabelTwo_[3] = static_cast<float>(slider->getValue());
        updateLabels();
    }
    
}



void ThunderGenerationAudioProcessorEditor::updateButtonText() 
{
   
}