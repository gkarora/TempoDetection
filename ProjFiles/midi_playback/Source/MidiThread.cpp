#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiThread.h"
#include <map>
#include <sstream>
#include <iomanip>

using namespace std;

extern int currentBpm;

MidiThread::MidiThread(MainContentComponent *contentComponent,
		MidiMessageSequence sequence,
		short origPpq,
		map<double, double> tempoMap,
		map<double, std::pair<int, int>> timeSigMap) : Thread("MidiThread") {
	setPriority(10);

	content = contentComponent;
	midiSequence = sequence;
	ppq = origPpq;
	tempos = tempoMap;
	timeSigs= timeSigMap;
}

void MidiThread::run() {
	midiPort = MidiOutput::openDevice(0);

	map<double, double>::iterator tempoIterator = tempos.begin();
	double tickLength = tempoIterator->second;
	tempoIterator++;
	map<double, std::pair<int, int>>::iterator timeSigIterator = timeSigs.begin();
	int timeSigNumerator = timeSigIterator->second.first;
	int timeSigDenominator = timeSigIterator->second.second;
	timeSigIterator++;

	MidiMessage message;

	// MIDI click intro
	File clickFile("C:/Users/Nicole/Documents/Click120.mid");
	ScopedPointer<FileInputStream> clickStream = clickFile.createInputStream();
	double eventTick;
	double prevTick = 0;
	double prevDelta = 0;
	int now = Time::getMillisecondCounter();
	if (clickStream) {
		click.readFrom(*clickStream);
		short clickPpq = click.getTimeFormat();
		MidiMessageSequence clickSequence = MidiMessageSequence();
		clickSequence = *click.getTrack(0);
		int i = 0;
		while (i < clickSequence.getNumEvents()) {
			message = clickSequence.getEventPointer(i)->message;
			eventTick = message.getTimeStamp();
			double origBpm = 60000 / (tickLength * clickPpq);
			double delta = prevDelta + ((eventTick - prevTick) * (origBpm / currentBpm)) * tickLength;
			Time::waitForMillisecondCounter(now + delta);
			midiPort->sendMessageNow(message);
			i++;
			prevTick = eventTick;
			prevDelta = delta;
		}
	}

	now = Time::getMillisecondCounter();
	prevTick = 0;
	prevDelta = 0;
	int currentBar = 1;
	int currentBeat = 1;
	int currentTick = 0;
	int quartersPerBar = timeSigNumerator * (timeSigDenominator / 4);
	int tickStartOfBar = 0;
	int tickStartNextBar = quartersPerBar * ppq;
	int i = 0;
	while (i < midiSequence.getNumEvents() && !threadShouldExit()) {
		message = midiSequence.getEventPointer(i)->message;
		eventTick = message.getTimeStamp(); // number of midi ticks until note
		if (tempoIterator != tempos.end() && eventTick >= tempoIterator->first) {
			tickLength = tempoIterator->second;
			tempoIterator++;
		}
		if (timeSigIterator != timeSigs.end() && eventTick >= timeSigIterator->first) {
			timeSigNumerator = timeSigIterator->second.first;
			timeSigDenominator = timeSigIterator->second.second;
			quartersPerBar = timeSigNumerator * (4 / timeSigDenominator);
			timeSigIterator++;
		}
		if (eventTick >= tickStartNextBar) {
			tickStartOfBar = tickStartNextBar;
			tickStartNextBar = tickStartOfBar + quartersPerBar * ppq;
			currentBar = currentBar + static_cast<int> (std::floor((eventTick - tickStartOfBar) / (ppq * quartersPerBar))) + 1;
		}
		currentBeat = static_cast<int> (std::floor((eventTick - tickStartOfBar)/ ppq)) % timeSigNumerator + 1;
		currentTick = static_cast<int> (eventTick) % ppq;
		if (!message.isMetaEvent() && (currentTick == 0 || currentTick == 120 || currentTick == 240 || currentTick == 360)) {
			std::stringstream ss;
			ss << currentBar << ":" << currentBeat << ":" << setfill('0') << setw(3) << currentTick;
			content->updateBbt(ss.str());
		}

		double origBpm = 60000 / (tickLength * ppq);
		double delta = prevDelta + ((eventTick - prevTick) * (origBpm / currentBpm)) * tickLength;
		Time::waitForMillisecondCounter(now + delta);
		midiPort->sendMessageNow(message);
		i++;
		prevTick = eventTick;
		prevDelta = delta;
	}
	delete midiPort;
	return;
}
