#include "GuiMiscTab.h"

//==============================================================================
GuiMiscTab::GuiMiscTab (SynthSound* pSynthSound)
    : pSound(pSynthSound)
    , masterLevelLabel("master level", TRANS("Volume"))
{
    auto initLabel = [this](Label& label)
    {
        addAndMakeVisible(label);
        label.setFont(Font(15.00f, Font::plain).withTypefaceStyle("Regular"));
        label.setJustificationType(Justification::centredRight);
        label.setEditable(false, false, false);
        label.setColour(TextEditor::textColourId, Colours::black);
        label.setColour(TextEditor::backgroundColourId, Colour(0x00000000));
    };

    initLabel(masterLevelLabel);

    auto initSlider = [this](Slider& slider, String unit)
    {
        addAndMakeVisible(slider);
        slider.setSliderStyle(Slider::LinearHorizontal);
        slider.setTextBoxStyle(Slider::TextBoxRight, false, 60, 20);
        slider.setTextValueSuffix (unit);
        slider.addListener(this);
        slider.setMouseClickGrabsKeyboardFocus(false);
        slider.setWantsKeyboardFocus(false);
    };

    initSlider(masterLevelSlider, ""); masterLevelSlider.setRange(0, 10, 0.001);

    notify();
}

GuiMiscTab::~GuiMiscTab()
{
}

//==============================================================================
void GuiMiscTab::paint (Graphics& g)
{
    Rectangle<int> titleArea (0, 10, getWidth(), 20);
    
    g.fillAll (Colours::black);
    g.setColour(Colours::white);
    g.drawText("Master", titleArea, Justification::centredTop);
    
    Rectangle <float> area (10, 28, 380, 40);
    
    g.setColour(Colours::yellow);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void GuiMiscTab::resized()
{
    const int labelLeft = 20;
    const int controlLeft = 130;
    const int labelWidth = 100;
    const int sliderWidth = 230;
    const int controlHeight = 24;
    const int gapHeight = 6;

    int top = 35;
    masterLevelLabel.setBounds(labelLeft, top, labelWidth, controlHeight);
    masterLevelSlider.setBounds(controlLeft, top, sliderWidth, controlHeight);
    top += controlHeight + gapHeight;
    top += controlHeight + gapHeight;
}

void GuiMiscTab::sliderValueChanged (Slider* sliderThatWasMoved)
{
    float value = (float)(sliderThatWasMoved->getValue());
    SynthParameters* pParams = pSound->pParams;
    if (sliderThatWasMoved == &masterLevelSlider)
    {
        pParams->masterLevel = 0.1f * value;
    }
    pSound->parameterChanged();
}

void GuiMiscTab::notify()
{
    SynthParameters* pParams = pSound->pParams;
    masterLevelSlider.setValue(10.0 * pParams->masterLevel);
}
