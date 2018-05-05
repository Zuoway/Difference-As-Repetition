/*
  ==============================================================================

    GuiSequencer.h
    Created: 11 Apr 2018 1:49:18am
    Author:  Zhuowei Huang

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "LookAndFeels.h"
#include "GuiFooter.h"

//==============================================================================
/*
*/
class GuiSequencer  : public AnimatedAppComponent, public Slider::Listener
{
public:
    GuiSequencer(GuiFooter&);
    ~GuiSequencer();

    void paint (Graphics&) override;
    void resized() override;
    void update() override;

    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    
private:
    GuiFooter& footer;
    
    Slider sliders[16];
    DrawableText notevals[16];
    DefaultLookAndFeel defaultLookAndFeel;
    TickedLookAndFeel tickedLookAndFeel;
    
    TextButton randomize { "Randomize" };
    Random rand;
    Range<int> noteRange;
    
    const double pi = std::atan(1)*4;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuiSequencer)
};

//==============================================================================
