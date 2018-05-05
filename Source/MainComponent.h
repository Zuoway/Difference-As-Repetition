/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthAudioSource.h"
#include "GuiTabs.h"
#include "GuiSequencer.h"
#include "GuiFooter.h"
#include "RecordingUtil.h"
#include "AudioLiveScrollingDisplay.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent, private Timer, public ChangeListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();
    
    //==============================================================================
    virtual void changeListenerCallback (ChangeBroadcaster* source) override {};

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void paintOverChildren(Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    void timerCallback() override
    {
        keyboardComponent.grabKeyboardFocus();
        stopTimer();
    }
    
    SynthAudioSource synthAudioSource;
    MidiKeyboardState keyboardState;
    MidiKeyboardComponent keyboardComponent;
    MidiMessageCollector collecter;
    
    static const int kNumberOfPrograms = 128;
    static const int kNumberOfVoices = 16;
    
    GuiTabs guiTabs;
    GuiSequencer sequencer;
    GuiFooter footer;
    ComboBox externalMidi;
    TextButton scanExternal { "Scan External Midi Input" };
    StringArray deviceList;
    AudioRecordingDemo recordDemo;
    
    bool started = false;
    
    ScopedPointer<MidiInput> inDevice;
    
    LiveScrollingAudioDisplay liveAudioScroller;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
