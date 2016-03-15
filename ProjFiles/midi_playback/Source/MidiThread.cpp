#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiThread.h"
#include <map>
#include <sstream>

using namespace std;

extern int currentBpm;
std::string bbt;

MidiThread::MidiThread() : Thread("MidiThread") {
	setPriority(10);
}

void MidiThread::run() {
	midiPort = MidiOutput::openDevice(0);
	File inputFile("Midi_Test.mid");
	ScopedPointer<FileInputStream> inputStream = inputFile.createInputStream();
	if (inputStream) {
		midi.readFrom(*inputStream);
		// ppq = pulses per quarter
		short origPpq = midi.getTimeFormat();

		double endTick = midi.getLastTimestamp();

		// Pre-process MIDI file
		std::map<double, double> tempos;
		std::map<double, std::pair<int, int>> timeSigs;
		int timeSigNumerator = 4;
		int timeSigDenominator = 4;
		MidiMessage message;
		for (int eventIndex = 0; eventIndex < midi.getTrack(0)->getNumEvents(); eventIndex++) {
			message = midi.getTrack(0)->getEventPointer(eventIndex)->message;
			if (message.isTempoMetaEvent()) {
				tempos[message.getTimeStamp()] = message.getTempoMetaEventTickLength(origPpq) * 1000;
			}
			else if (message.isTimeSignatureMetaEvent()) {
				message.getTimeSignatureInfo(timeSigNumerator, timeSigDenominator);
				timeSigs[message.getTimeStamp()] = std::pair<int, int>(timeSigNumerator, timeSigDenominator);
			}
		}

		// Combine tracks
		MidiMessageSequence sequence = MidiMessageSequence();
		for (int trackIndex = 0; trackIndex < midi.getNumTracks(); trackIndex++) {
			midiSequence = *midi.getTrack(trackIndex);
			sequence.addSequence(midiSequence, 0, 0, endTick);
			sequence.updateMatchedPairs();
		}

		int now = Time::getMillisecondCounter();
		double eventTick;
		double prevTick = 0;
		double prevDelta = 0;
		int currentBar = 1;
		int currentBeat = 1;
		int currentTick = 0;
		map<double, double>::iterator tempoIterator = tempos.begin();
		double tickLength = tempoIterator->second;
		tempoIterator++;
		map<double, std::pair<int, int>>::iterator timeSigIterator = timeSigs.begin();
		timeSigNumerator = timeSigIterator->second.first;
		timeSigDenominator = timeSigIterator->second.second;
		timeSigIterator++;
		int quartersPerBar = timeSigNumerator * (timeSigDenominator / 4);
		int tickStartOfBar = 0;
		int tickStartNextBar = quartersPerBar * origPpq;
		int i = 0;
		while (i < sequence.getNumEvents()) {
			message = sequence.getEventPointer(i)->message;
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
				tickStartNextBar = tickStartOfBar + quartersPerBar * origPpq;
				currentBar = currentBar + static_cast<int> (std::floor((eventTick - tickStartOfBar) / (origPpq * quartersPerBar))) + 1;
			}
			currentBeat = static_cast<int> (std::floor((eventTick - tickStartOfBar) / origPpq)) % timeSigNumerator + 1;
			currentTick = static_cast<int> (eventTick) % origPpq;
			if (!message.isMetaEvent() && (currentTick == 0 || currentTick == 120 || currentTick == 240 || currentTick == 360)) {
				std::stringstream ss;
				ss << currentBar << ":" << currentBeat << ":" << currentTick;
				bbt = ss.str();
			}

			double origBpm = 60000 / (tickLength * origPpq);
			double delta = prevDelta + ((eventTick - prevTick) * (origBpm / currentBpm)) * tickLength;
			Time::waitForMillisecondCounter(now + delta);
			midiPort->sendMessageNow(message);
			i++;
			prevTick = eventTick;
			prevDelta = delta;
		}
	}
}