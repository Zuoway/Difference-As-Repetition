/*
 ==============================================================================
 
 This file is part of the JUCE examples.
 Copyright (c) 2017 - ROLI Ltd.
 
 The code included in this file is provided under the terms of the ISC license
 http://www.isc.org/downloads/software-support-policy/isc-license. Permission
 To use, copy, modify, and/or distribute this software for any purpose with or
 without fee is hereby granted provided that the above copyright notice and
 this permission notice appear in all copies.
 
 THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
 WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
 PURPOSE, ARE DISCLAIMED.
 
 ==============================================================================
 */

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.
 
 BEGIN_JUCE_PIP_METADATA
 
 name:             AudioRecordingDemo
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Records audio to a file.
 
 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
 juce_audio_processors, juce_audio_utils, juce_core,
 juce_data_structures, juce_events, juce_graphics,
 juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2017, linux_make, androidstudio, xcode_iphone
 
 type:             Component
 mainClass:        AudioRecordingDemo
 
 useLocalCopy:     1
 
 END_JUCE_PIP_METADATA
 
 *******************************************************************************/

#pragma once

#include "MainComponent.h"

//==============================================================================
/** A simple class that acts as an AudioIODeviceCallback and writes the
 incoming audio data to a WAV file.
 */
class AudioRecorder  : public AudioIODeviceCallback
{
public:
    AudioRecorder (AudioThumbnail& thumbnailToUpdate)
    : thumbnail (thumbnailToUpdate)
    {
        backgroundThread.startThread();
    }
    
    ~AudioRecorder()
    {
        stop();
    }
    
    //==============================================================================
    void startRecording (const File& file)
    {
        stop();
        
        if (sampleRate > 0)
        {
            // Create an OutputStream to write to our destination file...
            file.deleteFile();
            ScopedPointer<FileOutputStream> fileStream (file.createOutputStream());
            
            if (fileStream.get() != nullptr)
            {
                // Now create a WAV writer object that writes to our output stream...
                WavAudioFormat wavFormat;
                auto* writer = wavFormat.createWriterFor (fileStream.get(), sampleRate, 1, 16, {}, 0);
                
                if (writer != nullptr)
                {
                    fileStream.release(); // (passes responsibility for deleting the stream to the writer object that is now using it)
                    
                    // Now we'll create one of these helper objects which will act as a FIFO buffer, and will
                    // write the data to disk on our background thread.
                    threadedWriter.reset (new AudioFormatWriter::ThreadedWriter (writer, backgroundThread, 32768));
                    
                    // Reset our recording thumbnail
                    thumbnail.reset (writer->getNumChannels(), writer->getSampleRate());
                    nextSampleNum = 0;
                    
                    // And now, swap over our active writer pointer so that the audio callback will start using it..
                    const ScopedLock sl (writerLock);
                    activeWriter = threadedWriter.get();
                }
            }
        }
    }
    
    void stop()
    {
        // First, clear this pointer to stop the audio callback from using our writer object..
        {
            const ScopedLock sl (writerLock);
            activeWriter = nullptr;
        }
        
        // Now we can delete the writer object. It's done in this order because the deletion could
        // take a little time while remaining data gets flushed to disk, so it's best to avoid blocking
        // the audio callback while this happens.
        threadedWriter.reset();
    }
    
    bool isRecording() const
    {
        return activeWriter != nullptr;
    }
    
    //==============================================================================
    void audioDeviceAboutToStart (AudioIODevice* device) override
    {
        sampleRate = device->getCurrentSampleRate();
    }
    
    void audioDeviceStopped() override
    {
        sampleRate = 0;
    }
    
    void audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                float** outputChannelData, int numOutputChannels,
                                int numSamples) override
    {
        const ScopedLock sl (writerLock);
        
        if (activeWriter != nullptr && numInputChannels >= thumbnail.getNumChannels())
        {
            activeWriter->write (outputChannelData, numSamples);
            
            // Create an AudioBuffer to wrap our incoming data, note that this does no allocations or copies, it simply references our input data
            AudioBuffer<float> buffer (const_cast<float**> (outputChannelData), thumbnail.getNumChannels(), numSamples);
            thumbnail.addBlock (nextSampleNum, buffer, 0, numSamples);
            nextSampleNum += numSamples;
        }
        
        // We need to clear the output buffers, in case they're full of junk..
        for (int i = 0; i < numOutputChannels; ++i)
            if (outputChannelData[i] != nullptr)
                FloatVectorOperations::clear (outputChannelData[i], numSamples);
    }
    
private:
    AudioThumbnail& thumbnail;
    TimeSliceThread backgroundThread  { "Audio Recorder Thread" }; // the thread that will write our audio data to disk
    ScopedPointer<AudioFormatWriter::ThreadedWriter> threadedWriter; // the FIFO used to buffer the incoming data
    double sampleRate   = 0.0;
    int64 nextSampleNum = 0;
    
    CriticalSection writerLock;
    AudioFormatWriter::ThreadedWriter* volatile activeWriter = nullptr;
};

//==============================================================================
class RecordingThumbnail  : public Component,
private ChangeListener
{
public:
    RecordingThumbnail()
    {
        formatManager.registerBasicFormats();
        thumbnail.addChangeListener (this);
    }
    
    ~RecordingThumbnail()
    {
        thumbnail.removeChangeListener (this);
    }
    
    AudioThumbnail& getAudioThumbnail()     { return thumbnail; }
    
    void setDisplayFullThumbnail (bool displayFull)
    {
        displayFullThumb = displayFull;
        repaint();
    }
    
    void paint (Graphics& g) override
    {
        g.fillAll (Colours::darkgrey);
        g.setColour (Colours::lightgrey);
        
        if (thumbnail.getTotalLength() > 0.0)
        {
            auto endTime = displayFullThumb ? thumbnail.getTotalLength()
            : jmax (30.0, thumbnail.getTotalLength());
            
            auto thumbArea = getLocalBounds();
            thumbnail.drawChannels (g, thumbArea.reduced (2), 0.0, endTime, 1.0f);
        }
        else
        {
            g.setFont (14.0f);
            g.drawFittedText ("(No file recorded)", getLocalBounds(), Justification::centred, 2);
        }
    }
    
private:
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbnailCache  { 10 };
    AudioThumbnail thumbnail            { 512, formatManager, thumbnailCache };
    
    bool displayFullThumb = false;
    
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        if (source == &thumbnail)
            repaint();
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RecordingThumbnail)
};

//==============================================================================
class AudioRecordingDemo  : public Component
{
public:
    AudioRecordingDemo(AudioDeviceManager& manager) : audioDeviceManager (manager)
    {
        setOpaque (true);
        
        addAndMakeVisible (recordButton);
        recordButton.setColour (TextButton::buttonColourId, Colour (0xffff5c5c));
        recordButton.setColour (TextButton::textColourOnId, Colours::black);
        recordButton.setWantsKeyboardFocus(false);
        recordButton.setMouseClickGrabsKeyboardFocus(false);
        recordButton.onClick = [this]
        {
            if (recorder.isRecording())
                stopRecording();
            else
                startRecording();
        };
        
        addAndMakeVisible (recordingThumbnail);
        
        audioDeviceManager.addAudioCallback (&recorder);
        setSize (500, 560);
    }
    
    ~AudioRecordingDemo()
    {
        audioDeviceManager.removeAudioCallback (&recorder);
    }
    
    void paint (Graphics& g) override
    {
        g.fillAll (Colours::black);
    }
    
    void resized() override
    {
        recordingThumbnail.setBounds (8, 8, 384, 50);
        recordButton.setBounds (400, 13, 50, 40);
    }
    
private:
    AudioDeviceManager& audioDeviceManager;
    
    RecordingThumbnail recordingThumbnail;
    AudioRecorder recorder  { recordingThumbnail.getAudioThumbnail() };
    
    TextButton recordButton { "REC" };
    File lastRecording;
    
    void startRecording()
    {
        if (! RuntimePermissions::isGranted (RuntimePermissions::writeExternalStorage))
        {
            SafePointer<AudioRecordingDemo> safeThis (this);
            
            RuntimePermissions::request (RuntimePermissions::writeExternalStorage,
                                         [safeThis] (bool granted) mutable
                                         {
                                             if (granted)
                                                 safeThis->startRecording();
                                         });
            return;
        }
        
        lastRecording = File::getSpecialLocation(File::currentApplicationFile).getParentDirectory()
        .getNonexistentChildFile ("Difference As Repetition", ".wav");
        std::cout<<lastRecording.getFullPathName()<<std::endl;
        recorder.startRecording (lastRecording);
        
        recordButton.setButtonText ("Stop");
        recordingThumbnail.setDisplayFullThumbnail (false);
    }
    
    void stopRecording()
    {
        recorder.stop();
        
        lastRecording = File();
        recordButton.setButtonText ("REC");
        recordingThumbnail.setDisplayFullThumbnail (true);
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioRecordingDemo)
};
