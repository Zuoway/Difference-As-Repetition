#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"

class GuiMiscTab : public Component, public Slider::Listener
{
public:
    GuiMiscTab (SynthSound* pSynthSound);
    ~GuiMiscTab();

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;

    void notify();

private:
    SynthSound* pSound;

    Label masterLevelLabel;
    Slider masterLevelSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuiMiscTab)
};
