#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include <map>

class MidiThread : public juce::Thread {
public:
	MidiThread(MainContentComponent *contentComponent,
		MidiMessageSequence midiSequence,
		short origPpq,
		std::map<double, double> tempoMap,
		std::map<double, std::pair<int, int>> timeSigMap);
	void run() override;
    void stopAudio(MidiOutput* port);
    MidiOutput* getMidiPort();

private:
	MidiOutput* midiPort;
	File inputFile;
	MidiFile midi;
	MidiFile click;
	short ppq;
	MidiMessageSequence midiSequence;
	std::map<double, double> tempos;
	std::map<double, std::pair<int, int>> timeSigs;
	MainContentComponent *content;
};
