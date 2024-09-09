#include "ContentComponent.h"

// //https://corecoding.com/utilities/rgb-or-hex-to-float.php


ContentComponent::ContentComponent (ThunderGenerationAudioProcessor& p) : 
    ShapeComponent(p),
    audioProcessor_(p),
    sliders_(contentNumSliders_),
    sliderArray_ (contentNumSliders_), 
    sliderAttachments_ (contentNumSliders_)
                                                              
{ 
    // Helper function to set up the sliders
    sliderSetup(contentNumSliders_,sliderAttachments_,sliders_, sliderArray_, colourScheme_,sliderId_);
    
    // Load the background image
    background_ = juce::ImageCache::getFromMemory (BinaryData::Content_png,
                                                   BinaryData::Content_pngSize);

}

ContentComponent::~ContentComponent()
{
  
}

void ContentComponent::paint (juce::Graphics& g)
{
    // Helper function to set up the component
    componentSetup(contentNumSliders_,g, background_,sliderNames_,textPositions_);
}

void ContentComponent::resized()
{
    // Helper function to set up the GUI location
    guiLocation(contentNumSliders_,sliders_, sliderY_,sliderX_);
}

void ContentComponent::updateLabel()
{
    // Set's up slider lables
    sliderNames_[1] = juce::String::formatted("LowPass: %.1f dB", sliderLabelOne_);
    sliderNames_[2] = juce::String::formatted("HighPass: %.1f ", sliderLabelTwo_);
    sliderNames_[3] = juce::String::formatted("Speed: %.8f ", sliderLabelThree_);
    sliderNames_[4] = juce::String::formatted("Second Hit Delay: %.1f ", sliderLabelFour_);
    repaint(); 
}

// Obtains the value of the slider by being set in a different class
void ContentComponent::setLabelSliderOne(float value)
{
    sliderLabelOne_ = value;

    repaint();
    updateLabel();
}
void ContentComponent::setLabelSliderTwo(float value)
{
    sliderLabelTwo_ = value;
    repaint();
    updateLabel();
}

void ContentComponent::setLabelSliderThree(float value)
{
    sliderLabelThree_ = value;
    repaint();
    updateLabel();
}  

void ContentComponent::setLabelSliderFour(float value)
{
    sliderLabelFour_ = value;
    repaint();
    updateLabel();
}  

// when the slider is changed, the value and label is updated
void ContentComponent::sliderValueChanged (juce::Slider* slider)
{
    if (slider == &sliders_[static_cast<std::vector<SliderDetails>::size_type> (0)]) 
    {
        sliderLabelOne_ = static_cast<float>(slider->getValue());
        updateLabel();
    }
    if (slider == &sliders_[static_cast<std::vector<SliderDetails>::size_type> (1)]) 
    {
        sliderLabelTwo_ = static_cast<float>(slider->getValue());
        updateLabel();
    }
    if (slider == &sliders_[static_cast<std::vector<SliderDetails>::size_type> (2)]) 
    {
        sliderLabelThree_ = static_cast<float>(slider->getValue());
        updateLabel();
    }
    if (slider == &sliders_[static_cast<std::vector<SliderDetails>::size_type> (3)]) 
    {
        sliderLabelFour_ = static_cast<float>(slider->getValue());
        updateLabel();
    }
}