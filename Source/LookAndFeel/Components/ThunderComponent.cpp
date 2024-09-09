#include "ThunderComponent.h"

// //https://corecoding.com/utilities/rgb-or-hex-to-float.php

ThunderComponent::ThunderComponent (ThunderGenerationAudioProcessor& p) : 
    ShapeComponent(p),
    audioProcessor_(p)
                                                              
{
    // Set the starting point for the lightning
    float rand = random_.nextFloat() * 330;
    rand = juce::jlimit(50.0f, 200.0f, rand);
    startX_= rand;
    
    // Set the bounding box for the lightning
    boundingBox_.setBounds(0, 10, 320, 368);

    //  Set the starting point for the lightning
    startY_ = 15.5f;
    currentX_ = startX_;
    currentY_ = startY_;
    
    startTimer(100);

    // Load the background image
    background_ = juce::ImageCache::getFromMemory (BinaryData::Thunder_png,
                                                   BinaryData::Thunder_pngSize);

}

ThunderComponent::~ThunderComponent()
{
   

}

void ThunderComponent::paint (juce::Graphics& g)
{
    // Draw the background image
    g.drawImageWithin (background_, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
   
    // Clip the region to the bounding box
    g.reduceClipRegion(boundingBox_);

    // Stroke settings
    float thunderBrightness = random_.nextFloat() * 3.0f;
    thunderBrightness = juce::jlimit(2.0f, 3.0f, thunderBrightness);
    float branchBrightness = random_.nextFloat() * 2.0f;
    branchBrightness = juce::jlimit(0.5f, 1.9f, branchBrightness);

    // Draw the lightning
    g.setColour(StrikeColours::thunderYellow.darker(0.5f));
    if(visualThunder == true)
    {
        g.strokePath(lightningPath_, juce::PathStrokeType(thunderBrightness)); 
        for (const auto& branch : branches_)
        {   
            g.setColour(StrikeColours::thunderYellow.darker(0.2f));
            g.strokePath(branch, juce::PathStrokeType(branchBrightness)); // Adjust the stroke width for branches
        }
    }
}

void ThunderComponent::resized()
{
 
    
}

void ThunderComponent::sliderValueChanged (juce::Slider* /*slider*/)
{
}

void ThunderComponent::timerCallback() 
{
    // Randomly move the lightning
    float movement = random_.nextFloat() * 200.f;
    movement = juce::jlimit(5.0f, 200.0f, movement);
    if(visualThunder == true)
    {
        
    // Move the starting point until it reaches x = 350
        if (currentX_ < startX_)
        {
            currentX_ += movement; // Move by 10 pixels each time
            repaint();
            return;
        }

        // Check if we need to add more points to the path
        if (segmentsAdded_ == 0)
        {
            lightningPath_.startNewSubPath(currentX_, currentY_); 
            segmentsAdded_++;
            repaint();
            return;
        }

        if (segmentsAdded_ < maxSegments_)
        {
            // Random offsets for x and y to create jagged effect
            float rand = random_.nextFloat() * 100;
            rand = juce::jlimit(5.0f, 100.0f, rand);
            float offsetX = (std::rand() % static_cast<int>(rand)) - 10.0f; 
            float offsetY = (std::rand() % static_cast<int>(rand)) + 10.0f; 

        
            currentX_ += offsetX;
            currentY_ += offsetY;
            

            lightningPath_.lineTo(currentX_, currentY_);
            segmentsAdded_++;
            // 20% chance to create a branch
            if (random_.nextFloat() < 2.0f) 
            {
            createBranch(currentX_, currentY_);
            }
            repaint();
        }
    }
}

void ThunderComponent::createBranch(float startX, float startY)
{
    juce::Path branchPath;
    branchPath.startNewSubPath(startX, startY);
    float rand = random_.nextFloat() * 200.0f;

    // Number of segments in the branch
    int branchSegments = random_.nextInt({ 10, 200 }); 
    float branchX = startX;
    float branchY = startY;

    for (int i = 0; i < branchSegments; ++i)
    {
        float offsetX = (random_.nextFloat() * 2.0f - 1.0f) * rand; 
        float offsetY = random_.nextFloat() * rand; 

        branchX += offsetX;
        branchY += offsetY;

        branchPath.lineTo(branchX, branchY);
    }

    branches_.push_back(branchPath);
}
