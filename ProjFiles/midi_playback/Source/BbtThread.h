#include "../JuceLibraryCode/JuceHeader.h"

class BbtThread : public juce::Thread {
public:
	BbtThread(Label *bbtLabel);
	void run() override;

private:
	Label *label;
};