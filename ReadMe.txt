========================================================================
    CONSOLE APPLICATION : TempoDetection Project Overview
========================================================================


/////////////////////////////////////////////////////////////////////////////
TempoDetection.cpp
    This is the main application source file.
StdAfx.h
    We are using this as our main header file.
/////////////////////////////////////////////////////////////////////////////
For Windows we need:

StdAfx.cpp: Just have this include StdAfx.h file
targetver.h: Just: `#pragma once` and `#include <SDKDDKVer.h>`
/////////////////////////////////////////////////////////////////////////////
First Time Setup for XCode:

Prereq: Jucer

1. Open Jucer file. It will open in Jucer, click 'Open in IDE'.
2. In XCode, Add files in `ProjFiles/midi_playback/Source` into the xcode project group folder (the yellow one which has the same name as the project).
3. Add the files in the 'ProjectFiles` (not in midi_playback folder) into XCode project at the blue folder level.
4. Save
5. Set up OpenCV

Mac To Run:

1. Download Simple Synth, and have it open.
2. Open Midi Studio.
3. Open IAC Driver, make sure the 'Device is online' box is checked off.
4. Leave this window open.
5. Add a midi file to the project in XCode 
   (Products -> <project_name>.app->right click-> show in finder)
6. Add conducting video in the same location.
7. Check naming of all media (the video and midi file)
8. Run.
Side note: there might be 2 breakpoints in JUCE code, just ignore it and run it anyways)


/////////////////////////////////////////////////////////////////////////////
