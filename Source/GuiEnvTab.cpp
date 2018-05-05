#include "GuiEnvTab.h"

GuiEnvTab::GuiEnvTab (SynthSound* pSynthSound)
    : pSound(pSynthSound)
    , attackLabel("attack", TRANS("Attack Time"))
    , decayLabel("decay", TRANS("Decay Time"))
    , sustainLabel("sustain", TRANS("Sustain Level"))
    , releaseLabel("release", TRANS("Release Time"))
{
    auto initLabel = [this](Label& label)
    {
        addAndMakeVisible(label);
        label.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        label.setJustificationType (Justification::centredRight);
        label.setEditable (false, false, false);
        label.setColour (TextEditor::textColourId, Colours::black);
        label.setColour (TextEditor::backgroundColourId, Colour (0x00000000));
        label.setWantsKeyboardFocus(false);
        label.setMouseClickGrabsKeyboardFocus(false);
    };

    initLabel(attackLabel);
    initLabel(decayLabel);
    initLabel(sustainLabel);
    initLabel(releaseLabel);

    auto initSlider = [this](Slider& slider, String unit)
    {
        addAndMakeVisible(slider);
        slider.setSliderStyle (Slider::LinearHorizontal);
        slider.setPopupDisplayEnabled(true, true, this);
        slider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        slider.setTextValueSuffix (unit);
        slider.setWantsKeyboardFocus(false);
        slider.setMouseClickGrabsKeyboardFocus(false);
        slider.addListener (this);
    };

    initSlider(attackSlider, " sec"); attackSlider.setRange (0, 10, 0.01);
    initSlider(decaySlider, " sec"); decaySlider.setRange (0, 10, 0.01);
    initSlider(sustainSlider, " %"); sustainSlider.setRange (0, 100, 1);
    initSlider(releaseSlider, " sec"); releaseSlider.setRange (0, 10, 0.01);

    notify();
}

void GuiEnvTab::paint (Graphics& g)
{
    Rectangle<int> titleArea (0, 10, getWidth(), 20);
    
    g.fillAll (Colours::black);
    g.setColour(Colours::white);
    g.drawText("Envelope", titleArea, Justification::centredTop);
    
    Rectangle <float> area (10, 28, 380, 128);
    
    g.setColour(Colours::yellow);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void GuiEnvTab::resized()
{
    const int labelLeft = 20;
    const int controlLeft = 130;
    const int labelWidth = 100;
    const int sliderWidth = 250;
    const int controlHeight = 24;
    const int gapHeight = 6;

    int top = 35;
    attackLabel.setBounds (labelLeft, top, labelWidth, controlHeight);
    attackSlider.setBounds (controlLeft, top, sliderWidth, controlHeight);
    top += controlHeight + gapHeight;
    decayLabel.setBounds (labelLeft, top, labelWidth, controlHeight);
    decaySlider.setBounds (controlLeft, top, sliderWidth, controlHeight);
    top += controlHeight + gapHeight;
    sustainLabel.setBounds (labelLeft, top, labelWidth, controlHeight);
    sustainSlider.setBounds (controlLeft, top, sliderWidth, controlHeight);
    top += controlHeight + gapHeight;
    releaseLabel.setBounds (labelLeft, top, labelWidth, controlHeight);
    releaseSlider.setBounds (controlLeft, top, sliderWidth, controlHeight);
}

void GuiEnvTab::sliderValueChanged (Slider* sliderThatWasMoved)
{
    float value = (float)(sliderThatWasMoved->getValue());
    SynthParameters* pParams = pSound->pParams;
    if (sliderThatWasMoved == &attackSlider) pParams->ampEgAttackTimeSeconds = value;
    else if (sliderThatWasMoved == &decaySlider) pParams->ampEgDecayTimeSeconds = value;
    else if (sliderThatWasMoved == &sustainSlider) pParams->ampEgSustainLevel = 0.01f * value;
    else if (sliderThatWasMoved == &releaseSlider) pParams->ampEgReleaseTimeSeconds = value;
    pSound->parameterChanged();
}

void GuiEnvTab::notify()
{
    SynthParameters* pParams = pSound->pParams;
    attackSlider.setValue(pParams->ampEgAttackTimeSeconds);
    decaySlider.setValue(pParams->ampEgDecayTimeSeconds);
    sustainSlider.setValue(100.0f * pParams->ampEgSustainLevel);
    releaseSlider.setValue(pParams->ampEgReleaseTimeSeconds);
}
