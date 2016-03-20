#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <map>

//==============================================================================
/*
This component lives inside our window, and this is where you should put all
your controls and content.
*/
class MidiThread;
class MainContentComponent : public AudioAppComponent,
								public Button::Listener {
public:
	MainContentComponent();
	~MainContentComponent();
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;
	void paint(Graphics& g) override;
	void resized() override;
	void buttonClicked(Button* button) override;
	void preprocessMidi(String filename);
	void updateBbt(String bbt);

private:
	String currentSizeAsString;
	TextButton startButton;
	TextButton stopButton;
	Label bpmLabel;
	Label bbtLabel;
	MidiThread *midiThread;

	short ppq;
	MidiMessageSequence sequence;
	std::map<double, double> tempos;
	std::map<double, std::pair<int, int>> timeSigs;
};

#endif  // MAINCOMPONENT_H_INCLUDED
