/*
  ==============================================================================

    GuiFooter.cpp
    Created: 10 Apr 2018 6:21:35pm
    Author:  Zhuowei Huang

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "GuiFooter.h"

//==============================================================================
GuiFooter::GuiFooter(MidiKeyboardState& keyState) : cur_step(0),playbackInterval(1000),playing(true),
keyboardState (keyState),
play("Play",Colours::white,Colours::slategrey,Colours::olive), stop("Stop",Colours::white,Colours::slategrey,Colours::olive)
{
    Path playPath;
    Path stopPath;
    stopPath.addRectangle(0, 0, 26, 26);
    playPath.addTriangle(0, 0, 0, 30, 26, 15);
    stop.setShape(stopPath, false, true, true);
    play.setShape(playPath, false, true, true);
    play.setWantsKeyboardFocus(false);
    play.setMouseClickGrabsKeyboardFocus(false);
    stop.setWantsKeyboardFocus(false);
    stop.setMouseClickGrabsKeyboardFocus(false);
    addAndMakeVisible(play);
    addAndMakeVisible(stop);
    play.onClick = [this] {
        playing = true;
        timerCallback();
    };
    stop.onClick = [this] {
        stopTimer();
        cur_step = 0;
        keyboardState.allNotesOff(2);
    };
    
    auto initSlider = [this](Slider& slider)
    {
        addAndMakeVisible(slider);
        slider.setSliderStyle(Slider::LinearHorizontal);
        slider.setTextBoxStyle(Slider::TextBoxRight, false, 100, 20);
        slider.setValue(playbackInterval);
        slider.addListener(this);
        slider.setWantsKeyboardFocus(false);
        slider.setMouseClickGrabsKeyboardFocus(false);
    };
    interval.setRange(1, 10000, 1);
    interval.setTextValueSuffix(" millisec");
    interval.setSkewFactor(0.7);
    initSlider(interval);
    
    auto initLabel = [this](Label& label)
    {
        addAndMakeVisible(label);
        label.setFont(Font(14.00f, Font::plain).withTypefaceStyle("Regular"));
        label.setJustificationType(Justification::centredRight);
        label.setEditable(false, false, false);
        label.setColour(TextEditor::textColourId, Colours::black);
        label.setColour(TextEditor::backgroundColourId, Colour(0x00000000));
        label.setText("BPM: " + std::to_string(60000/playbackInterval), NotificationType::dontSendNotification);
        label.setWantsKeyboardFocus(false);
        label.setMouseClickGrabsKeyboardFocus(false);
    };
    initLabel(bpm);
}

GuiFooter::~GuiFooter()
{
    keyboardState.allNotesOff(2);
}


void GuiFooter::timerCallback(){
    if(playing){
        keyboardState.noteOn(2, notes[cur_step], 1.0f);
        playing = !playing;
        startTimer(playbackInterval);
    }else{
//        keyboardState.noteOff(2, notes[cur_step], 1.0f);
        keyboardState.allNotesOff(2);
        cur_step = (++cur_step)%16;
        playing = !playing;
        startTimer(0);
    }
}


void GuiFooter::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if(sliderThatWasMoved == &interval){
        playbackInterval = interval.getValue();
        bpm.setText("BPM: " + std::to_string(60000/playbackInterval), NotificationType::dontSendNotification);
    }
}

void GuiFooter::paint (Graphics& g)
{
    Rectangle<int> titleArea (90, 20, 350, 20);
    
    g.fillAll (Colours::black);
    g.setColour(Colours::white);
    g.drawText("Note Length", titleArea, Justification::centredTop);
    
    Rectangle <float> area (90, 38, 350, 40);
    
    g.setColour(Colours::yellow);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void GuiFooter::resized()
{
    Rectangle <int> area (90, 38, 350, 40);
    play.setBounds(5, 12, 41, 41);
    stop.setBounds(48, 14, 38, 38);
    interval.setBounds(area.reduced(10,10));
    bpm.setBounds(280, 18, 150, 20);
}
