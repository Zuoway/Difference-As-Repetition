/*
  ==============================================================================

    SynthAudioSource.h
    Created: 26 Apr 2018 4:11:57pm
    Author:  Zhuowei Huang

  ==============================================================================
*/
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthParameters.h"
#include "SynthVoice.h"
#include "Synth.h"
#include "MainComponent.h"

class SynthAudioSource  : public AudioSource, public ChangeBroadcaster
{
public:
    SynthAudioSource (MidiKeyboardState& keyState, MidiMessageCollector& coll)
        : keyboardState (keyState), collecter(coll), currentProgram(0)
    {
        initializePrograms();
    
        for (int i = 0; i < kNumberOfVoices; ++i)
            synth.addVoice(new SynthVoice());
        
        pSound = new SynthSound(synth);
        pSound->pParams = &programBank[currentProgram];
        synth.addSound(pSound);
    }
    
    void initializePrograms()
    {
        for (int i = 0; i < kNumberOfPrograms; i++)
            programBank[i].setDefaultValues();
    }
    
    double getTailLengthSeconds() const
    {
        return 0.0;
    }
    
    int getNumPrograms()
    {
        return kNumberOfPrograms;
    }
    
    int getCurrentProgram()
    {
        return currentProgram;
    }
    
    //------------------------------------------------------------------------
    
    void setUsingSineWaveSound()
    {
        synth.clearSounds();
    }

    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        synth.setCurrentPlaybackSampleRate (sampleRate);
        collecter.reset(sampleRate);
    }

    void releaseResources() override {}

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();

        MidiBuffer incomingMidi;
        collecter.removeNextBlockOfMessages (incomingMidi, bufferToFill.numSamples);
        keyboardState.processNextMidiBuffer (incomingMidi, bufferToFill.startSample,
                                             bufferToFill.numSamples, true);

        synth.renderNextBlock (*bufferToFill.buffer, incomingMidi,
                               bufferToFill.startSample, bufferToFill.numSamples);
    }
    
    SynthSound* getSound() { return pSound; }
    
private:
    MidiKeyboardState& keyboardState;
    MidiMessageCollector& collecter;
    
    static const int kNumberOfPrograms = 128;
    static const int kNumberOfVoices = 16;
    
    Synth synth;
    SynthSound* pSound;
    SynthParameters programBank[kNumberOfPrograms];
    int currentProgram;
    
};
