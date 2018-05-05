#include "GuiOscTab.h"

GuiOscTab::GuiOscTab (SynthSound* pSynthSound)
    : pSound(pSynthSound)
    , wfLabel1("waveform label1", TRANS("Osc1 Waveform"))
    , semiLabel1("semitone offset label1", TRANS("Pitch"))
    , detuneLabel1("detune label1", TRANS("Detune"))
    , wfLabel2("waveform label2", TRANS("Osc2 Waveform"))
    , semiLabel2("semitone offset label1", TRANS("Pitch"))
    , detuneLabel2("detune label1", TRANS("Detune"))
    , oscBlendLabel("osc blend label", TRANS("Blend"))
{
    auto initLabel = [this](Label& label)
    {
        addAndMakeVisible(label);
        label.setFont(Font(15.00f, Font::plain).withTypefaceStyle("Regular"));
        label.setJustificationType(Justification::centredRight);
        label.setEditable(false, false, false);
        label.setColour(TextEditor::textColourId, Colours::black);
        label.setColour(TextEditor::backgroundColourId, Colour(0x00000000));
        label.setWantsKeyboardFocus(false);
        label.setMouseClickGrabsKeyboardFocus(false);
    };

    initLabel(wfLabel1);
    initLabel(semiLabel1);
    initLabel(detuneLabel1);
    initLabel(wfLabel2);
    initLabel(semiLabel2);
    initLabel(detuneLabel2);
    initLabel(oscBlendLabel);
    oscBlendLabel.setJustificationType(Justification::centredLeft);

    auto initCombo = [this](ComboBox& combo)
    {
        addAndMakeVisible(combo);
        combo.setEditableText(false);
        combo.setJustificationType(Justification::centredLeft);
        combo.setTextWhenNothingSelected("");
        combo.setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
        combo.addListener(this);
        combo.setMouseClickGrabsKeyboardFocus(false);
        combo.setWantsKeyboardFocus(false);
    };

    initCombo(waveformCB1); 
    SynthWaveform::setupComboBox(waveformCB1);
    waveformCB1.addListener(this);
    initCombo(waveformCB2);
    SynthWaveform::setupComboBox(waveformCB2);
    waveformCB2.addListener(this);

    auto initSlider = [this](Slider& slider, String unit)
    {
        addAndMakeVisible(slider);
        slider.setPopupDisplayEnabled(true, true, this);
        slider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        slider.setTextValueSuffix (unit);
        slider.setSliderStyle(Slider::LinearHorizontal);
        slider.addListener(this);
        slider.setMouseClickGrabsKeyboardFocus(false);
        slider.setWantsKeyboardFocus(false);
    };

    initSlider(semiSlider1, " semi"); semiSlider1.setRange(-24, 24, 1);
    initSlider(detuneSlider1, " cents"); detuneSlider1.setRange(-50, 50, 0.001);
    initSlider(semiSlider2, " semi"); semiSlider2.setRange(-24, 24, 1);
    initSlider(detuneSlider2, " cents"); detuneSlider2.setRange(-50, 50, 0.001);

    addAndMakeVisible(oscBlendSlider);
    oscBlendSlider.setRange(0, 100, 1);
    oscBlendSlider.setSliderStyle(Slider::LinearVertical);
    oscBlendSlider.setTextBoxStyle(Slider::TextBoxRight, false, 30, 20);
    oscBlendSlider.setMouseClickGrabsKeyboardFocus(false);
    oscBlendSlider.setWantsKeyboardFocus(false);
    oscBlendSlider.addListener(this);

    notify();
}

GuiOscTab::~GuiOscTab()
{
}

//==============================================================================
void GuiOscTab::paint (Graphics& g)
{
    Rectangle<int> titleArea (0, 10, getWidth(), 20);
    
    g.fillAll (Colours::black);
    g.setColour(Colours::white);
    g.drawText("Oscillator", titleArea, Justification::centredTop);
    
    Rectangle <float> area (10, 28, 380, 208);
    
    g.setColour(Colours::yellow);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void GuiOscTab::resized()
{
    const int labelLeft = 20;
    const int controlLeft = 130;
    const int labelWidth = 100;
    const int cboxWidth = 100;
    const int sliderWidth = 180;
    const int controlHeight = 24;
    const int gapHeight = 6;
    const int blendSliderLeft = 300;
    const int blendSliderWidth = 80;
    const int blendSliderHeight = 180;

    int top = 35;
    wfLabel1.setBounds (labelLeft, top, labelWidth, controlHeight);
    waveformCB1.setBounds (controlLeft, top, cboxWidth, controlHeight);
    top += controlHeight + gapHeight;
    semiLabel1.setBounds (labelLeft, top, labelWidth, controlHeight);
    semiSlider1.setBounds (controlLeft, top, sliderWidth, controlHeight);
    top += controlHeight + gapHeight;
    detuneLabel1.setBounds (labelLeft, top, labelWidth, controlHeight);
    detuneSlider1.setBounds (controlLeft, top, sliderWidth, controlHeight);

    top += controlHeight + 4 * gapHeight;
    wfLabel2.setBounds(labelLeft, top, labelWidth, controlHeight);
    waveformCB2.setBounds(controlLeft, top, cboxWidth, controlHeight);
    top += controlHeight + gapHeight;
    semiLabel2.setBounds(labelLeft, top, labelWidth, controlHeight);
    semiSlider2.setBounds(controlLeft, top, sliderWidth, controlHeight);
    top += controlHeight + gapHeight;
    detuneLabel2.setBounds(labelLeft, top, labelWidth, controlHeight);
    detuneSlider2.setBounds(controlLeft, top, sliderWidth, controlHeight);

    top = 35;
    oscBlendLabel.setBounds(blendSliderLeft, top, labelWidth, controlHeight);
    top += controlHeight;
    oscBlendSlider.setBounds(blendSliderLeft, top, blendSliderWidth, blendSliderHeight);
}

void GuiOscTab::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    SynthParameters* pParams = pSound->pParams;
    if (comboBoxThatHasChanged == &waveformCB1)
    {
        pParams->osc1Waveform.fromComboBox(*comboBoxThatHasChanged);
    }
    else if (comboBoxThatHasChanged == &waveformCB2)
    {
        pParams->osc2Waveform.fromComboBox(*comboBoxThatHasChanged);
    }
    pSound->parameterChanged();
}

void GuiOscTab::sliderValueChanged (Slider* sliderThatWasMoved)
{
    float value = (float)(sliderThatWasMoved->getValue());
    SynthParameters* pParams = pSound->pParams;
    if (sliderThatWasMoved == &semiSlider1)
    {
        pParams->osc1PitchOffsetSemitones = int(value);
    }
    else if (sliderThatWasMoved == &semiSlider2)
    {
        pParams->osc2PitchOffsetSemitones = int(value);
    }
    else if (sliderThatWasMoved == &detuneSlider1)
    {
        pParams->osc1DetuneOffsetCents = value;
    }
    else if (sliderThatWasMoved == &detuneSlider2)
    {
        pParams->osc2DetuneOffsetCents = value;
    }
    else if (sliderThatWasMoved == &oscBlendSlider)
    {
        pParams->oscBlend = 0.01f * value;
    }
    pSound->parameterChanged();
}

void GuiOscTab::notify()
{
    SynthParameters* pParams = pSound->pParams;

    pParams->osc1Waveform.toComboBox(waveformCB1);
    semiSlider1.setValue(pParams->osc1PitchOffsetSemitones);
    detuneSlider1.setValue(pParams->osc1DetuneOffsetCents);

    pParams->osc2Waveform.toComboBox(waveformCB2);
    semiSlider2.setValue(pParams->osc2PitchOffsetSemitones);
    detuneSlider2.setValue(pParams->osc2DetuneOffsetCents);

    oscBlendSlider.setValue(100.0 * pParams->oscBlend);
}
