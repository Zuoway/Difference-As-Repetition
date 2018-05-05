/*
  ==============================================================================

    GuiFooter.h
    Created: 10 Apr 2018 6:21:35pm
    Author:  Zhuowei Huang

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class GuiFooter  : public Component, public Timer, public Slider::Listener
{
public:
    GuiFooter(MidiKeyboardState&);
    ~GuiFooter();

    void paint (Graphics&) override;
    void resized() override;
    
    void timerCallback() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;

    int cur_step;
    int last_step;
    int notes[16];
    int playbackInterval;
    bool playing;
    
private:
    MidiKeyboardState& keyboardState;
    
    ShapeButton play;
    ShapeButton stop;
    
    Slider interval;
    Label bpm;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuiFooter)
};
