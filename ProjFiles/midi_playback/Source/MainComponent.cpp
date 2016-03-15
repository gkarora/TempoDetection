/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/



#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include "BbtThread.h"
#include "Windows.h"
#include <conio.h>
#include <thread>
using namespace std;

extern int currentBpm;
extern std::string bbt;

MainContentComponent::MainContentComponent() {
	setSize(800, 600);
	bpmLabel.setText(String(currentBpm), dontSendNotification);
	bpmLabel.setColour(Label::backgroundColourId, Colours::black);
	bpmLabel.setColour(Label::textColourId, Colours::white);
	bpmLabel.setJustificationType(Justification::centred);
	bpmLabel.setBounds(0, 0, 800, 200);
	addAndMakeVisible(bpmLabel);

	bbtLabel.setText(String("00:00:000"), dontSendNotification);
	bbtLabel.setColour(Label::backgroundColourId, Colours::black);
	bbtLabel.setColour(Label::textColourId, Colours::white);
	bbtLabel.setJustificationType(Justification::centred);
	bbtLabel.setBounds(0, 200, 800, 200);
	addAndMakeVisible(bbtLabel);
	BbtThread *bbtThread = new BbtThread(&bbtLabel);
	bbtThread->startThread();

	addAndMakeVisible(decButton);
	decButton.setButtonText("-");
	decButton.setColour(TextButton::textColourOffId, Colours::black);
	decButton.setBounds(0, 400, 400, 200);
	decButton.addListener(this);
	addAndMakeVisible(incButton);
	incButton.setButtonText("+");
	incButton.setColour(TextButton::textColourOffId, Colours::black);
	incButton.setBounds(400, 400, 400, 200);
	incButton.addListener(this);

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
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	//g.fillAll (Colours::black);
	//g.setColour(Colours::white);
	//g.drawText(String(currentBpm), getLocalBounds(), Justification::centred);

	// You can add your drawing code here!
}

void MainContentComponent::resized() {
	// This is called when the MainContentComponent is resized.
	// If you add any child components, this is where you should
	// update their positions.
	currentSizeAsString = String(getWidth()) + "x" + String(getHeight());
}

void MainContentComponent::buttonClicked(Button* button) {
	if (button == &incButton) {
		currentBpm = currentBpm + 5;
		bpmLabel.setText(String(currentBpm), dontSendNotification);
	}
	else if (button == &decButton) {
		currentBpm = currentBpm - 5;
		bpmLabel.setText(String(currentBpm), dontSendNotification);
	}
}

// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent() {
	return new MainContentComponent();
}