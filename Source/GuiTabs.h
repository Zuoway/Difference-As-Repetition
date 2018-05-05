#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "GuiMiscTab.h"
#include "GuiOscTab.h"
#include "GuiEnvTab.h"

class GuiTabs  : public Component
{
public:
    GuiTabs (SynthSound* pSynthSound);
    ~GuiTabs();

    void paint (Graphics& g) override;
    void resized() override;

    void notify();

private:
    ScopedPointer<GuiMiscTab> pMiscTab;
    ScopedPointer<GuiOscTab> pOscTab;
    ScopedPointer<GuiEnvTab> pEnvTab;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuiTabs)
};
