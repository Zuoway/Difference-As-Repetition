#include "GuiTabs.h"

GuiTabs::GuiTabs (SynthSound* pSynthSound)
{
    addAndMakeVisible(pMiscTab = new GuiMiscTab(pSynthSound));
    addAndMakeVisible(pOscTab = new GuiOscTab(pSynthSound));
    addAndMakeVisible(pEnvTab = new GuiEnvTab(pSynthSound));
    pEnvTab->setMouseClickGrabsKeyboardFocus(false);
    pEnvTab->setWantsKeyboardFocus(false);
    pOscTab->setMouseClickGrabsKeyboardFocus(false);
    pOscTab->setWantsKeyboardFocus(false);
    pEnvTab->setMouseClickGrabsKeyboardFocus(false);
    pEnvTab->setWantsKeyboardFocus(false);
}

GuiTabs::~GuiTabs()
{
}

//==============================================================================
void GuiTabs::paint (Graphics& g)
{
    g.fillAll (Colour (0xff323e44));
}

void GuiTabs::resized()
{
    Rectangle<int> area = getLocalBounds();
    
    pEnvTab->setBounds(area.removeFromTop(160));
    pOscTab->setBounds(area.removeFromTop(240));
    pMiscTab->setBounds(area.removeFromTop(80));
}

void GuiTabs::notify()
{
    pMiscTab->notify();
    pOscTab->notify();
    pEnvTab->notify();
}
