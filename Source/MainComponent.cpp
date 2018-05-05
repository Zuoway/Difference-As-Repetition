/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : synthAudioSource (keyboardState, collecter),
keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard), guiTabs(synthAudioSource.getSound()),sequencer(footer),footer(keyboardState), recordDemo(deviceManager)
{
    recordDemo.setWantsKeyboardFocus(false);
    recordDemo.setMouseClickGrabsKeyboardFocus(false);
    footer.setWantsKeyboardFocus(false);
    footer.setMouseClickGrabsKeyboardFocus(false);
    liveAudioScroller.setWantsKeyboardFocus(false);
    liveAudioScroller.setMouseClickGrabsKeyboardFocus(false);
    guiTabs.setWantsKeyboardFocus(false);
    guiTabs.setMouseClickGrabsKeyboardFocus(false);
    sequencer.setWantsKeyboardFocus(false);
    sequencer.setMouseClickGrabsKeyboardFocus(false);
    
    addAndMakeVisible(&keyboardComponent);
    addAndMakeVisible(&recordDemo);
    addAndMakeVisible(&guiTabs);
    addAndMakeVisible(&sequencer);
    addAndMakeVisible(&footer);
    addAndMakeVisible(&liveAudioScroller);
    addAndMakeVisible(scanExternal);
    
    auto initCombo = [this](ComboBox& combo)
    {
        addAndMakeVisible(combo);
        combo.setEditableText(false);
        combo.setJustificationType(Justification::centredLeft);
        combo.setTextWhenNothingSelected("Select Midi Input");
        combo.setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
        combo.setMouseClickGrabsKeyboardFocus(false);
        combo.setWantsKeyboardFocus(false);
    };
    
    initCombo(externalMidi);
    
    scanExternal.setWantsKeyboardFocus(false);
    scanExternal.setMouseClickGrabsKeyboardFocus(false);
    scanExternal.onClick = [this] {
        externalMidi.clear();
        deviceList = MidiInput::getDevices ();
        externalMidi.addItemList(deviceList, 1);
    };
    
    externalMidi.onChange = [this] {
        int selectId = externalMidi.getSelectedId();
        if (selectId > 0){
            inDevice = MidiInput::openDevice(selectId-1, &collecter);
            inDevice -> start();
            started = true;
            deviceManager.addMidiInputCallback(inDevice->getName(), &collecter);
        }
        else{
            if(started){
                started = false;
                deviceManager.removeMidiInputCallback(inDevice->getName(), &collecter);
                inDevice -> stop();
            }
        }
    };
    
    deviceManager.addAudioCallback (&liveAudioScroller);
    
    setSize (900, 660);
    // specify the number of input and output channels that we want to open
    setAudioChannels (2, 2);
    synthAudioSource.addChangeListener(this);
    startTimer(400);
}

MainComponent::~MainComponent()
{
    deviceManager.removeAudioCallback (&liveAudioScroller);
    shutdownAudio();
    if(started){
        started = false;
        inDevice->stop();
    }
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    synthAudioSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    synthAudioSource.getNextAudioBlock (bufferToFill);
}

void MainComponent::releaseResources()
{
    synthAudioSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    ignoreUnused(g);
}

void MainComponent::paintOverChildren(Graphics& g){
    Rectangle <float> area (8, 66, 484, 60);
    
    g.setColour(Colours::yellow);
    g.drawRect(area,2.0f);
}

void MainComponent::resized()
{
    Rectangle<int> area = getLocalBounds();
    keyboardComponent.setBounds(area.removeFromBottom(100));
    guiTabs.setBounds(area.removeFromBottom(480).removeFromRight(400));
    sequencer.setBounds(0, 134, 500, 426);
    footer.setBounds(area.removeFromTop(80).removeFromRight(450));
    recordDemo.setBounds(0, 0, 500, 540);
    liveAudioScroller.setBounds (8, 66, 484, 60);
    scanExternal.setBounds(5, 500, 60, 25);
    externalMidi.setBounds(5, 530, 140, 25);
}
