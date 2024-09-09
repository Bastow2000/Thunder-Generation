#include "ShapeComponent.h"

// //https://corecoding.com/utilities/rgb-or-hex-to-float.php

// Self Referencing Code taken from previous project Bastow Synth to set up components

// Base class for all components for helper functions that reduce repetitive code
ShapeComponent::ShapeComponent (ThunderGenerationAudioProcessor& p) : 
    audioProcessor_(p),
    sliderArray_ (shapeNumSliders_), 
    sliders(shapeNumSliders_),
    sliderAttachments_ (shapeNumSliders_)
                                                              
{
    // Helper function to set up the sliders
    sliderSetup(shapeNumSliders_,sliderAttachments_, sliders, sliderArray_, colourScheme_, sliderId_);
    background_ = juce::ImageCache::getFromMemory (BinaryData::Shape_png,
                                                   BinaryData::Shape_pngSize);
}

// Destructor
ShapeComponent::~ShapeComponent()
{
}

// Helper function to set up the sliders
void ShapeComponent::sliderSetup(int numSlidersChoice,std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>>& sliderAttachment, std::vector<SliderDetails>& sliderS, std::vector<std::unique_ptr<SliderDetails>>& sliderArray,std::array<juce::Colour, numSliders>& colourScheme, std::array<juce::String, numSliders>& sliderId )
{
    std::vector<juce::String> sliderIds;
    std::vector<juce::Colour> sliderColours;
    int localNumSliders = static_cast<int>(numSlidersChoice);

    sliderIds.resize(static_cast<size_t>(localNumSliders));
    sliderColours.resize(static_cast<size_t>(localNumSliders));

    // Sets the slider ID and colour for each slider
    for (size_t i = 0; i < static_cast<size_t>(localNumSliders); ++i) 
    {
        sliderIds[i] = sliderId[i];
        sliderColours[i] = colourScheme[i];
    }

    // Creates a unique pointer for each slider
    for (std::size_t i = 0; i < static_cast<size_t>(localNumSliders); ++i)
    {
        sliderArray[i] = std::make_unique<SliderDetails>();
    }

    // Helper lambda function to get the colour of the slider
    const auto getGainSliderColour = [&] (int index)
    {
        return sliderColours[static_cast<size_t>(index)];
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

     for (auto i = 0; i < localNumSliders; ++i)
    {
        // Adds listeners to individual sliders
        sliderS[static_cast<std::vector<SliderDetails>::size_type> (i)]
            .addListener (this);

        // Sets colour for individual sliders
        sliderS[static_cast<std::vector<SliderDetails>::size_type> (i)]
            .setColour (getGainSliderColour (static_cast<int> (i)));

        // Adds sliders to the audio processor
        createSliderAttachment(audioProcessor_.tree, sliderAttachment[static_cast<std::vector<SliderDetails>::size_type> (i)], sliderIds[static_cast<std::vector<juce::String>::size_type>(i)], sliderS[static_cast<std::vector<SliderDetails>::size_type> (i)]);

        // Makes sliders visible
        addAndMakeVisible (sliderS[static_cast<std::vector<SliderDetails>::size_type> (i)]);
    }
}

// Sets up helper function for component
void ShapeComponent::componentSetup(int numSlidersChoice,juce::Graphics& g, juce::Image& background,std::array<juce::String, numSliders +1>& sliderNames, std::array<int,((numSliders +1) *2)>& textPositions)
{
    // Gets background for this component
    g.drawImageWithin (background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);

    std::vector<juce::String> sliderName;
    std::vector<int> textPosition;
    int localNumSliders = static_cast<int>(numSlidersChoice);

    sliderName.clear();
    textPosition.clear();

    sliderName.resize(static_cast<size_t>(localNumSliders+1));
    textPosition.resize(static_cast<size_t>((localNumSliders+1)*2));


    // Allows for customiseability on the number of sliders and text positions
    for (size_t i = 0; i < static_cast<size_t>(localNumSliders)+1 ; ++i) 
    {
        sliderName[i] = sliderNames[i];
    }
    for (size_t i = 0; i < (static_cast<size_t>(localNumSliders)+1)*2 ; ++i) 
    {
        textPosition[i] = textPositions[i];
    }
   
    static auto black = juce::Typeface::createSystemTypefaceFor (
        BinaryData::AvenirBlack_ttf, BinaryData::AvenirBlack_ttfSize);
    g.setColour (juce::Colours::white);
    g.setFont (black);
    g.setFont (20);

    juce::Rectangle<int> a = getLocalBounds().withPosition ({ textPosition[0], textPosition[1] });
    g.drawFittedText (sliderName[0], a, juce::Justification::topLeft, false);

    static auto book = juce::Typeface::createSystemTypefaceFor (
        BinaryData::AvenirBook_ttf, BinaryData::AvenirBook_ttfSize);
    g.setColour (juce::Colours::white);
    g.setFont (book);
    g.setFont (11.5f);
    
    juce::Rectangle<int> b = getLocalBounds().withPosition ({ textPosition[2], textPosition[3] });
    g.drawFittedText (sliderName[1], b, juce::Justification::topLeft, false);
    

    juce::Rectangle<int> c = getLocalBounds().withPosition ({ textPosition[4], textPosition[5] });
    g.drawFittedText (sliderName[2], c, juce::Justification::topLeft, false);

    if(localNumSliders > 2)
    {
        juce::Rectangle<int> d = getLocalBounds().withPosition ({ textPosition[6], textPosition[7] });
        g.drawFittedText (sliderName[3], d, juce::Justification::topLeft, false);

        juce::Rectangle<int> e = getLocalBounds().withPosition ({ textPosition[8], textPosition[9] });
        g.drawFittedText (sliderName[4], e, juce::Justification::topLeft, false);
    }
    else
    {

    }

    if(localNumSliders > 4)
    {

        juce::Rectangle<int> f = getLocalBounds().withPosition ({ textPosition[10], textPosition[11] });
        g.drawFittedText (sliderName[5], f, juce::Justification::topLeft, false);
        

        juce::Rectangle<int> k = getLocalBounds().withPosition ({ textPosition[12], textPosition[13] });
        g.drawFittedText (sliderName[6], k, juce::Justification::topLeft, false);
      
    }
    else
    {

    }
}


void ShapeComponent::paint (juce::Graphics& g)
{
   componentSetup(shapeNumSliders_,g, background_,sliderNames_,textPositions_);
}

// Updates the labels for the sliders
void ShapeComponent::updateLabels()
{
    sliderNames_[1] = juce::String::formatted("Length: %.0f ", sliderLabelOne_);
    sliderNames_[2] = juce::String::formatted("PitchType: %.0f ", sliderLabelTwo_);
    sliderNames_[3] = juce::String::formatted("Noise Strength: %.0f", sliderLabelThree_);
    sliderNames_[4] = juce::String::formatted("Segments: %.0f ", sliderLabelFour_);
    repaint();
}

// Obtains the value of the slider by being set in a different class
void ShapeComponent::setLabelSliderOne(float value)
{
    sliderLabelOne_ = value;
    
    repaint();
    updateLabels();
}
void ShapeComponent::setLabelSliderTwo(float value)
{
    sliderLabelTwo_ = value;
    repaint();
    updateLabels();
}

void ShapeComponent::setLabelSliderThree(float value)
{
    sliderLabelThree_ = value;
    repaint();
    updateLabels();
}

void ShapeComponent::setLabelSliderFour(float value)
{
    sliderLabelFour_ = value;
    repaint();
    updateLabels();
}

// Helper function to set up the GUI location
void ShapeComponent::guiLocation(int numSlidersChoice,std::vector<SliderDetails>& slider, std::array<int, numSliders> sliderY, std::array<int, numSliders> sliderX)
{
    std::vector<int> posY;
    std::vector<int> posX;
    int localNumSliders = (numSlidersChoice);

    posY.resize(static_cast<size_t>(localNumSliders));
    posX.resize(static_cast<size_t>(localNumSliders));

    for (size_t i = 0; i < static_cast<size_t>(localNumSliders); ++i) 
    {
        posY[i] = sliderY[i];
        posX[i] = sliderX[i];
    }

    for (unsigned int i = 0; i < static_cast<size_t>(localNumSliders); ++i)
    {
        auto a = getLocalBounds().withWidth (130).withHeight (325).withX (
            static_cast<int> (posX[i]));
        slider[i].setBounds (a.withY (posY[i]));
       
    }
}

void ShapeComponent::resized()
{
    guiLocation(shapeNumSliders_,sliders, sliderY_,sliderX_);
}

// When the slider is changed, the value and label is updated
void ShapeComponent::sliderValueChanged (juce::Slider* slider)
{
    if (slider == &sliders[static_cast<std::vector<SliderDetails>::size_type> (0)]) 
    {
        sliderLabelOne_ = static_cast<float>(slider->getValue());
        updateLabels();
    }
    if (slider == &sliders[static_cast<std::vector<SliderDetails>::size_type> (1)]) 
    {
        sliderLabelTwo_ = static_cast<float>(slider->getValue());
        updateLabels();
    }
    if (slider == &sliders[static_cast<std::vector<SliderDetails>::size_type> (2)]) 
    {
        sliderLabelThree_ = static_cast<float>(slider->getValue());
        updateLabels();
    }

    if (slider == &sliders[static_cast<std::vector<SliderDetails>::size_type> (3)]) 
    {
        sliderLabelFour_ = static_cast<float>(slider->getValue());
        updateLabels();
    }
}
