#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
This component lives inside our window, and this is where you should put all
your controls and content.
*/
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
	//static void changeBpm(Label &label);

private:
	String currentSizeAsString;
	TextButton incButton;
	TextButton decButton;
	Label bpmLabel;
	Label bbtLabel;
};

#endif  // MAINCOMPONENT_H_INCLUDED
