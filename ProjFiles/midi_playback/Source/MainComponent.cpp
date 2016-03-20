/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/



#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include "MidiThread.h"
#include <thread>
#include <map>

using namespace std;

extern int currentBpm;

MainContentComponent::MainContentComponent() {
	setSize(200, 400);
	bpmLabel.setColour(Label::backgroundColourId, Colours::black);
	bpmLabel.setColour(Label::textColourId, Colours::white);
	bpmLabel.setJustificationType(Justification::centred);
	bpmLabel.setBounds(0, 0, 200, 100);
	addAndMakeVisible(bpmLabel);

    bbtLabel.setText(juce::String("00:00:000"), dontSendNotification);
	bbtLabel.setColour(Label::backgroundColourId, Colours::black);
	bbtLabel.setColour(Label::textColourId, Colours::white);
	bbtLabel.setJustificationType(Justification::centred);
	bbtLabel.setBounds(0, 100, 200, 100);
	addAndMakeVisible(bbtLabel);

	addAndMakeVisible(startButton);
	startButton.setButtonText("Start");
	startButton.setColour(TextButton::textColourOffId, Colours::black);
	startButton.setBounds(0, 200, 200, 100);
	startButton.addListener(this);
	addAndMakeVisible(stopButton);
	stopButton.setButtonText("Stop");
	stopButton.setColour(TextButton::textColourOffId, Colours::black);
	stopButton.setBounds(0, 300, 200, 100);
	stopButton.addListener(this);
	
	// specify the number of input and output channels that we want to open
	setAudioChannels(2, 2);
}

MainContentComponent::~MainContentComponent() {
	shutdownAudio();
}

void MainContentComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
	// This function will be called when the audio device is started, or when
	// its settings (i.e. sample rate, block size, etc) are changed.

	// You can use this function to initialise any resources you might need,
	// but be careful - it will be called on the audio thread, not the GUI thread.

	// For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainContentComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) {
	// Your audio-processing code goes here!

	// For more details, see the help for AudioProcessor::getNextAudioBlock()

	// Right now we are not producing any data, in which case we need to clear the buffer
	// (to prevent the output of random noise)
	bufferToFill.clearActiveBufferRegion();
}

void MainContentComponent::releaseResources() {
	// This will be called when the audio device stops, or when it is being
	// restarted due to a setting change.

	// For more details, see the help for AudioProcessor::releaseResources()
}

//=======================================================================
void MainContentComponent::paint(Graphics& g) {
	// You can add your drawing code here!
}

void MainContentComponent::resized() {
	// This is called when the MainContentComponent is resized.
	// If you add any child components, this is where you should
	// update their positions.
}

void MainContentComponent::buttonClicked(Button* button) {
	if (button == &startButton) {
		preprocessMidi("Shut_Up_And_Dance.mid");
        bpmLabel.setText(juce::String(currentBpm), dontSendNotification);
        
        //std::thread t(get_binary_image);
        //t.detach();
        
		midiThread = new MidiThread(this, sequence, ppq, tempos, timeSigs);
		midiThread->startThread();
	}
	else if (button == &stopButton) {
        midiThread->stopAudio(midiThread->getMidiPort());
		midiThread->stopThread(1000);
        
	}
}

void MainContentComponent::updateBbt(juce::String bbt) {
	MessageManagerLock mmLock;
	bbtLabel.setText(bbt, dontSendNotification);
}

void MainContentComponent::preprocessMidi(juce::String filename) {
	File inputFile(filename);
	ScopedPointer<FileInputStream> inputStream = inputFile.createInputStream();
	if (inputStream) {
		MidiFile midi;
		midi.readFrom(*inputStream);
		// ppq = pulses per quarter
		ppq = midi.getTimeFormat();

		double endTick = midi.getLastTimestamp();

		// Pre-process MIDI file
		int timeSigNumerator = 4;
		int timeSigDenominator = 4;
		MidiMessage message;
		for (int eventIndex = 0; eventIndex < midi.getTrack(0)->getNumEvents(); eventIndex++) {
			message = midi.getTrack(0)->getEventPointer(eventIndex)->message;
			if (message.isTempoMetaEvent()) {
				tempos[message.getTimeStamp()] = message.getTempoMetaEventTickLength(ppq) * 1000;
			}
			else if (message.isTimeSignatureMetaEvent()) {
				message.getTimeSignatureInfo(timeSigNumerator, timeSigDenominator);
				timeSigs[message.getTimeStamp()] = std::pair<int, int>(timeSigNumerator, timeSigDenominator);
			}
		}
		map<double, double>::iterator tempoIterator = tempos.begin();
		double tickLength = tempoIterator->second;

		currentBpm = 60000 / (tickLength * ppq);

		// Combine tracks
		MidiMessageSequence midiSequence;
		sequence = MidiMessageSequence();
		for (int trackIndex = 0; trackIndex < midi.getNumTracks(); trackIndex++) {
			midiSequence = *midi.getTrack(trackIndex);
			sequence.addSequence(midiSequence, 0, 0, endTick);
			sequence.updateMatchedPairs();
		}
	}
}

// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent() {
	return new MainContentComponent();
}
