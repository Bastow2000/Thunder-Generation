#pragma once
#include <JuceHeader.h>

namespace ShapeColours
{
    // Shape Slider 1
    const juce::Colour red = juce::Colour::fromFloatRGBA (0.773f, 0.318f, 0.318f, 1.0f);

    // Shape Slider 2
    const juce::Colour orange = juce::Colour::fromFloatRGBA (0.886f, 0.639f, 0.353f,1.0f);

    // Shape Slider 3
    const juce::Colour lime = juce::Colour::fromFloatRGBA (0.749f, 0.886f, 0.353f,1.0f);

    // Shape Slider 4
    const juce::Colour cyan = juce::Colour::fromFloatRGBA (0.302f, 0.718f, 0.722f,1.0f);
}

namespace ContentColours
{
    // Content Slider 1
    const juce::Colour pink = juce::Colour::fromFloatRGBA (0.886f, 0.353f, 0.624f, 1.0f);

    // Content Slider 2
    const juce::Colour violet = juce::Colour::fromFloatRGBA (0.647f, 0.353f, 0.886f, 1.0f);

    // Content Slider 3
    const juce::Colour purple = juce::Colour::fromFloatRGBA (0.416f, 0.353f, 0.886f, 1.0f);

    // Content Slider 4
    const juce::Colour blue = juce::Colour::fromFloatRGBA (0.353f, 0.463f, 0.886f, 1.0f);
}

namespace OtherColours
{
    // Slider Effects 
    const juce::Colour grey = juce::Colour::fromFloatRGBA (0.365f, 0.357f, 0.357f, 1.0f);

    // Slider Effects 2
    const juce::Colour lightGrey = juce::Colour::fromFloatRGBA (0.569f, 0.569f, 0.569f, 1.0f);

    // Button colour 
    const juce::Colour darkGrey = juce::Colour::fromFloatRGBA (0.133f, 0.133f, 0.133f, 1.0f);
}

namespace StrikeColours
{
// Strike Main Colour
const juce::Colour crimsonRed = juce::Colour::fromFloatRGBA (0.251f, 0.098f, 0.102f, 1.0f);

// Strike Slider 1
const juce::Colour darkRed = juce::Colour::fromFloatRGBA (0.239f, 0.055f, 0.055f, 1.0f);

// Strike Slider 2
const juce::Colour scarletRed = juce::Colour::fromFloatRGBA(0.322f, 0.141f, 0.051f,1.0f);

//Strike  Slider three
const juce::Colour brownOrange = juce::Colour::fromFloatRGBA (0.322f, 0.231f, 0.051f, 1.0f);

// Crackle Main Colour
const juce::Colour oceanBlue = juce::Colour::fromFloatRGBA (0.024f, 0.157f, 0.271f, 1.0f);

// Crackle Slider 1
const juce::Colour blue = juce::Colour::fromFloatRGBA (0.047f, 0.196f, 0.325f, 1.0f);

// Crackle Slider 2
const juce::Colour shallowBlue = juce::Colour::fromFloatRGBA (0.047f, 0.235f, 0.325f, 1.0f);

// Crackle Slider 3
const juce::Colour greenblue = juce::Colour::fromFloatRGBA (0.047f, 0.318f, 0.325f, 1.0f);

// Rumble Main Colour
const juce::Colour royalPurple = juce::Colour::fromFloatRGBA (0.212f, 0.024f, 0.271f, 1.0f);

// Rumble Slider 1
const juce::Colour deepPurple = juce::Colour::fromFloatRGBA (0.122f, 0.051f, 0.325f, 1.0f);

// Rumble Slider 2
const juce::Colour purple = juce::Colour::fromFloatRGBA (0.176f, 0.051f, 0.325f, 1.0f);

// Rumble Slider 3
const juce::Colour bloodPink = juce::Colour::fromFloatRGBA (0.322f, 0.051f, 0.22f, 1.0f);

// AfterImage Main Colour
const juce::Colour darkGreen = juce::Colour::fromFloatRGBA (0.024f, 0.271f, 0.165f, 1.0f);

// AfterImage Slider 1
const juce::Colour green = juce::Colour::fromFloatRGBA (0.047f, 0.325f, 0.176f, 1.0f);

// AfterImage Slider 2
const juce::Colour royalGreen = juce::Colour::fromFloatRGBA (0.047f, 0.325f, 0.176f, 1.0f);

// AfterImage Slider 3
const juce:: Colour limeGreen = juce::Colour::fromFloatRGBA (0.247f, 0.325f, 0.051f, 1.0f);

// thunder Main Colour
const juce::Colour grey = juce::Colour::fromFloatRGBA (0.18f, 0.18f, 0.188f, 1.0f);

// thunder Slider 1
const juce::Colour lightGrey = juce::Colour::fromFloatRGBA (0.271f, 0.271f, 0.275f, 1.0f);

// visual thunder
const juce::Colour thunderYellow = juce::Colour::fromFloatRGBA (0.714f, 0.686f, 0.557f, 1.0f);
} // namespace C1

static const int numSliders = 6;

// Id's  
const static juce::String shapeSliderOneID = "shapeOne";
const static juce::String shapeSliderOneName = "ShapeOne";

const static juce::String shapeSliderTwoID = "shapeTwo";
const static juce::String shapeSliderTwoName = "ShapeTwo";

const static juce::String shapeSliderThreeID = "shapeThree";
const static juce::String shapeSliderThreeName = "ShapeThree";

const static juce::String shapeSliderFourID = "shapeFour";
const static juce::String shapeSliderFourName = "ShapeFour";

const static juce::String contentSliderOneID = "contentOne";
const static juce::String contentSliderOneName = "ContentOne";

const static juce::String contentSliderTwoID = "contentTwo";
const static juce::String contentSliderTwoName = "ContentTwo";

const static juce::String contentSliderThreeID = "contentThree";
const static juce::String contentSliderThreeName = "ContentThree";

const static juce::String contentSliderFourID = "contentFour";
const static juce::String contentSliderFourName = "ContentFour";

const static juce::String effectsSliderOneID = "effectsOne";
const static juce::String effectsSliderOneName = "EffectsOne";

const static juce::String effectsSliderTwoID = "effectsTwo";
const static juce::String effectsSliderTwoName = "EffectsTwo";

const static juce::String effectsSliderThreeID = "effectsThree";
const static juce::String effectsSliderThreeName = "EffectsThree";

const static juce::String effectsSliderFourID = "effectsFour";
const static juce::String effectsSliderFourName = "EffectsFour";

const static juce::String effectsSliderFiveID = "effectsFive";
const static juce::String effectsSliderFiveName = "EffectsFive";

const static juce::String effectsSliderSixID = "effectsSix";
const static juce::String effectsSliderSixName = "EffectsSix";

const static juce::String mainSliderOneID = "mainOne";
const static juce::String mainSliderOneName = "MainOne";

const static juce::String mainSliderTwoID = "mainTwo";
const static juce::String mainSliderTwoName = "MainTwo";








const static juce::String strikeLengthId = "strikeLength";
const static juce::String strikeLengthName = "StrikeLength";

const static juce::String strikeIntensityId = "strikeIntensity";
const static juce::String strikeIntensityName = "StrikeIntensity";

const static juce::String rumbleAmplitudeId = "rumbleAmplitude";
const static juce::String rumbleAmplitudeName = "RumbleAmplitude";

const static juce::String rumbleLengthId = "rumbleLength";
const static juce::String rumbleLengthName = "RumbleLength";

const static juce::String rumbleIntensityId = "rumbleIntensity";
const static juce::String rumbleIntensityName = "RumbleIntensity";



constexpr static int versionHint = 1;
constexpr static std::string_view baseSliderId = "sliderId";
constexpr static std::string_view baseSliderName = "SliderName";

