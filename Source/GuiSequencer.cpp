/*
  ==============================================================================

    GuiSequencer.cpp
    Created: 11 Apr 2018 1:49:18am
    Author:  Zhuowei Huang

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "GuiSequencer.h"

//==============================================================================
GuiSequencer::GuiSequencer(GuiFooter& f) : footer(f), noteRange(1, 127)
{
    setFramesPerSecond (60);
    addAndMakeVisible (randomize);
    randomize.setWantsKeyboardFocus(false);
    randomize.setMouseClickGrabsKeyboardFocus(false);
    rand = Random(99*Time::getCurrentTime().getSeconds());
    
    auto initSlider = [this](Slider& slider)
    {
        addAndMakeVisible(slider);
        slider.setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        slider.addListener (this);
        slider.setLookAndFeel(&defaultLookAndFeel);
        slider.setRange (1, 127, 1);
        slider.setValue(64);
        slider.setMouseClickGrabsKeyboardFocus(false);
        slider.setWantsKeyboardFocus(false);
    };
    
    auto initText = [this](DrawableText& val, String text){
        addAndMakeVisible(val);
        val.setText(text);
        val.setColour(Colours::white);
        val.setFont(18.0f, true);
        val.setJustification(Justification::centred);
    };
    
    randomize.onClick = [this] {
        for(int i = 0; i < 16; i++){
            sliders[i].setValue(rand.nextInt(noteRange));
        }
    };
    
    for(int i = 0; i < 16; i++){
        initSlider(sliders[i]);
        footer.notes[i] = sliders[i].getValue();
        initText(notevals[i], sliders[i].getTextFromValue(footer.notes[i]));
    }
    setSize (500, 540);
}

GuiSequencer::~GuiSequencer()
{
    for(int i = 0; i < 16; i++){
        sliders[i].setLookAndFeel(nullptr);
    }
}

void GuiSequencer::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void GuiSequencer::resized()
{
    int radius = 180;
    for(int i = 0; i < 16; i++){
        Point<float> p (getWidth()/2 + radius * std::cos((-90+i*22.5)*pi/180),
                        getHeight()/2 + radius * std::sin((-90+i*22.5)*pi/180));
        sliders[i].setBounds(p.x-30, p.y-30,60,60);
        Rectangle<float> area(p.x-30, p.y-30,60,60);
        notevals[i].setBoundingBox(area);
    }
    randomize.setBounds(getWidth()/2-50, getHeight()/2-20, 100, 40);
}

void GuiSequencer::update(){
    sliders[(footer.cur_step+15)%16].setLookAndFeel(&defaultLookAndFeel);
    sliders[footer.cur_step].setLookAndFeel(&tickedLookAndFeel);
    if(!footer.isTimerRunning()){
        for(int i = 0; i<16; i++){
            sliders[i].setLookAndFeel(&defaultLookAndFeel);
        }
    }
}

void GuiSequencer::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if(sliderThatWasMoved == &sliders[0]){
        footer.notes[0] = sliders[0].getValue();
        notevals[0].setText(sliders[0].getTextFromValue(footer.notes[0]));
    }else if(sliderThatWasMoved == &sliders[1]){
        footer.notes[1] = sliders[1].getValue();
        notevals[1].setText(sliders[1].getTextFromValue(footer.notes[1]));
    }else if(sliderThatWasMoved == &sliders[2]){
        footer.notes[2] = sliders[2].getValue();
        notevals[2].setText(sliders[2].getTextFromValue(footer.notes[2]));
    }else if(sliderThatWasMoved == &sliders[3]){
        footer.notes[3] = sliders[3].getValue();
        notevals[3].setText(sliders[3].getTextFromValue(footer.notes[3]));
    }else if(sliderThatWasMoved == &sliders[4]){
        footer.notes[4] = sliders[4].getValue();
        notevals[4].setText(sliders[4].getTextFromValue(footer.notes[4]));
    }else if(sliderThatWasMoved == &sliders[5]){
        footer.notes[5] = sliders[5].getValue();
        notevals[5].setText(sliders[5].getTextFromValue(footer.notes[5]));
    }else if(sliderThatWasMoved == &sliders[6]){
        footer.notes[6] = sliders[6].getValue();
        notevals[6].setText(sliders[6].getTextFromValue(footer.notes[6]));
    }else if(sliderThatWasMoved == &sliders[7]){
        footer.notes[7] = sliders[7].getValue();
        notevals[7].setText(sliders[7].getTextFromValue(footer.notes[7]));
    }else if(sliderThatWasMoved == &sliders[8]){
        footer.notes[8] = sliders[8].getValue();
        notevals[8].setText(sliders[8].getTextFromValue(footer.notes[8]));
    }else if(sliderThatWasMoved == &sliders[9]){
        footer.notes[9] = sliders[9].getValue();
        notevals[9].setText(sliders[9].getTextFromValue(footer.notes[9]));
    }else if(sliderThatWasMoved == &sliders[10]){
        footer.notes[10] = sliders[10].getValue();
        notevals[10].setText(sliders[10].getTextFromValue(footer.notes[10]));
    }else if(sliderThatWasMoved == &sliders[11]){
        footer.notes[11] = sliders[11].getValue();
        notevals[11].setText(sliders[11].getTextFromValue(footer.notes[11]));
    }else if(sliderThatWasMoved == &sliders[12]){
        footer.notes[12] = sliders[12].getValue();
        notevals[12].setText(sliders[12].getTextFromValue(footer.notes[12]));
    }else if(sliderThatWasMoved == &sliders[13]){
        footer.notes[13] = sliders[13].getValue();
        notevals[13].setText(sliders[13].getTextFromValue(footer.notes[13]));
    }else if(sliderThatWasMoved == &sliders[14]){
        footer.notes[14] = sliders[14].getValue();
        notevals[14].setText(sliders[14].getTextFromValue(footer.notes[14]));
    }else if(sliderThatWasMoved == &sliders[15]){
        footer.notes[15] = sliders[15].getValue();
        notevals[15].setText(sliders[15].getTextFromValue(footer.notes[15]));
    }
}
