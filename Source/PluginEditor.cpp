/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LumenAudioProcessorEditor::LumenAudioProcessorEditor (LumenAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState(vts)
{
    // ========== Feedback ==========
    // Slider
    addAndMakeVisible(dial);
    dial.addListener(this);
    dial.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    dialAttachment.reset(new SliderAttachment(valueTreeState, "dial", dial));

    setLookAndFeel(&cartMouseLookAndFeel);

    setSize (300, 300);
}

LumenAudioProcessorEditor::~LumenAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void LumenAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll(backgroundColour);  
}

void LumenAudioProcessorEditor::resized()
{
    auto winHeight = getLocalBounds().getHeight();
    auto winWidth = getLocalBounds().getWidth();

    auto dialBox = getLocalBounds();
    dialBox.removeFromTop(winHeight * 0.1);
    dialBox.removeFromBottom(winHeight * 0.1);
    dialBox.removeFromLeft(winWidth * 0.05);
    dialBox.removeFromRight(winWidth * 0.05);

    const auto width = dialBox.getWidth();
    dial.setBounds(dialBox.removeFromLeft(width));
}

void LumenAudioProcessorEditor::sliderValueChanged(Slider* sliderThatWasMoved)
{
    auto dialValue = dial.getValue();

    if (dialValue < 0.0f)
        backgroundColour = cartMouseLookAndFeel.backgroundColour.darker(abs(dialValue));
    else
        backgroundColour = cartMouseLookAndFeel.backgroundColour.brighter(dialValue);

    repaint();
}
