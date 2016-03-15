#include "../JuceLibraryCode/JuceHeader.h"

class MidiThread : public juce::Thread {
public:
	MidiThread();
	void run() override;

private:
	MidiOutput* midiPort;
	File inputFile;
	MidiFile midi;
	MidiMessageSequence midiSequence;
	MidiMessageCollector messageCollector;
};