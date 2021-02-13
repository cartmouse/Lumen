/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CartMouseLookAndFeel.h"

//==============================================================================
/**
*/

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class LumenAudioProcessorEditor  : public AudioProcessorEditor,
    public Slider::Listener
{
public:
    LumenAudioProcessorEditor (LumenAudioProcessor&, AudioProcessorValueTreeState&);
    ~LumenAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider* sliderThatWasMoved) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LumenAudioProcessor& processor;

    CartMouseLookAndFeel cartMouseLookAndFeel;

    AudioProcessorValueTreeState& valueTreeState;

    Slider dial{ Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow };
    std::unique_ptr<SliderAttachment> dialAttachment;

    Colour backgroundColour{cartMouseLookAndFeel.backgroundColour};
    bool test{ false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LumenAudioProcessorEditor)
};
