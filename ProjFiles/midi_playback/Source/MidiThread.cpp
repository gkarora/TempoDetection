#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiThread.h"

MidiThread::MidiThread(int target) : Thread("MidiThread") {
	setPriority(10);
	targetBpm = target;
}

void MidiThread::run() {
	midiPort = MidiOutput::openDevice(0);
	File inputFile("Midi_Test.mid");
	ScopedPointer<FileInputStream> inputStream = inputFile.createInputStream();
	if (inputStream) {
		midi.readFrom(*inputStream);
		short ppq = midi.getTimeFormat();
		midi.convertTimestampTicksToSeconds();
		
		double endTime = midi.getLastTimestamp();
		MidiMessageSequence sequence = MidiMessageSequence();
		for (int trackIndex = 0; trackIndex < midi.getNumTracks(); trackIndex++) {
			midiSequence = *midi.getTrack(trackIndex);
			sequence.addSequence(midiSequence, 0, 0, endTime);
			sequence.updateMatchedPairs();
			/*const MidiMessageSequence *seq = midi.getTrack(trackIndex);
			//midiPort->sendBlockOfMessages();
			int last = seq->getEventPointer(0)->message.getTimeStamp();
			for (int eventIndex = 0; eventIndex < seq->getNumEvents(); eventIndex++) {
			int current = seq->getEventPointer(eventIndex)->message.getTimeStamp();
			_sleep((current - last));
			midiPort->sendMessageNow(seq->getEventPointer(eventIndex)->message);
			last = current;
			}*/
		}
		int now = Time::getMillisecondCounter();
		double eventTime;
		int i = 0;
		//int targetBpm = 80;
		while (i < sequence.getNumEvents()) {
			MidiMessage message = sequence.getEventPointer(i)->message;
			double tick = message.getTempoMetaEventTickLength(ppq);
			double origBpm = 60 / (tick * ppq);
			eventTime = message.getTimeStamp();
			Time::waitForMillisecondCounter(now + eventTime * 1000 * (origBpm/targetBpm));
			midiPort->sendMessageNow(message);
			i++;
		}
	}
}