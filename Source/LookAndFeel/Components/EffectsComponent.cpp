#include "EffectsComponent.h"


EffectsComponent::EffectsComponent (ThunderGenerationAudioProcessor& p) : 
    ShapeComponent(p),  
    audioProcessor_(p),
    sliders_(effectNumSliders_),
    sliderArray_ (effectNumSliders_), 
    sliderAttachments_ (effectNumSliders_)
                                                              
{
    
    // Set Background Image
    background_ = juce::ImageCache::getFromMemory (BinaryData::Effects_png,
                                                   BinaryData::Effects_pngSize);

    // Helper function to set up the sliders
    sliderSetup(effectNumSliders_,sliderAttachments_,sliders_, sliderArray_, colourScheme_,sliderId_);
}

EffectsComponent::~EffectsComponent()
{
  

}

void EffectsComponent::paint (juce::Graphics& g)
{
    //  Helper function to set up the component
    componentSetup(effectNumSliders_,g, background_,sliderNames_,textPositions_);
}

void EffectsComponent::resized()
{
    // Helper function to set up the GUI location
    guiLocation(effectNumSliders_,sliders_, sliderY_,sliderX_);
   
}


void EffectsComponent::updateLabel()
{
    // Set's up slider lables
    // "Reverb: RoomSize", "Reverb: Wet", "Reverb: Dry", "Compression: Threshold","Compression: Ratio","Compression: Attack"
    sliderNames_[1] = juce::String::formatted("WaveShaper PostGain: %.1f", sliderLabelOne_);
    sliderNames_[2] = juce::String::formatted("WaveShaper PreGain: %.1f ", sliderLabelTwo_);
    sliderNames_[3] = juce::String::formatted("Compression Release: %.0f", sliderLabelThree_);
    sliderNames_[4] = juce::String::formatted("Compression Threshold: %.0f ", sliderLabelFour_);
    sliderNames_[5] = juce::String::formatted("Compression Ratio: %.0f ", sliderLabelFive_);
    sliderNames_[6] = juce::String::formatted("Compression Attack: %.0f", sliderLabelSix_);
    repaint(); 
}

// Obtains the value of the slider by being set in a different class
void EffectsComponent::setLabelSliderOne(float value)
{
    sliderLabelOne_ = value;

    repaint();
    updateLabel();
}
void EffectsComponent::setLabelSliderTwo(float value)
{
    sliderLabelTwo_ = value;
    repaint();
    updateLabel();
}

void EffectsComponent::setLabelSliderThree(float value)
{
    sliderLabelThree_ = value;
    repaint();
    updateLabel();
}  

void EffectsComponent::setLabelSliderFour(float value)
{
    sliderLabelFour_ = value;
    repaint();
    updateLabel();
}  

void EffectsComponent::setLabelSliderFive(float value)
{
    sliderLabelFive_ = value;
    repaint();
    updateLabel();
}  
        
void EffectsComponent::setLabelSliderSix(float value)
{
    sliderLabelSix_ = value;
    repaint();
    updateLabel();
}  
             
// When the slider is changed, the value and label is updated
void EffectsComponent::sliderValueChanged (juce::Slider* slider)
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
    if (slider == &sliders_[static_cast<std::vector<SliderDetails>::size_type> (4)]) 
    {
        sliderLabelFive_ = static_cast<float>(slider->getValue());
        updateLabel();
    }
    if (slider == &sliders_[static_cast<std::vector<SliderDetails>::size_type> (5)]) 
    {
        sliderLabelSix_ = static_cast<float>(slider->getValue());
        updateLabel();
    }
}