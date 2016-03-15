#include "../JuceLibraryCode/JuceHeader.h"
#include "BbtThread.h"
#include <iostream>
#include <conio.h>

using namespace std;

extern int currentBpm;
extern std::string bbt;

BbtThread::BbtThread(Label *bbtLabel) : Thread("BbtThread") {
	setPriority(10);
	label = bbtLabel;
}

void BbtThread::run() {
	while (1) {
		const MessageManagerLock mmLock;
		label->setText(bbt, dontSendNotification);
	}
}