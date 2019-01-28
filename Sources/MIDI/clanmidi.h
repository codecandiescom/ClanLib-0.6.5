//---------------------------------------------------------------------------
#ifndef ClanMidiH
#define ClanMidiH
//---------------------------------------------------------------------------
#endif
#include <dmusicc.h>
#include <dmusici.h>

//Okay this is the first draft of a WIN32 CLANLIB MIDI PLAYER
//It compiles on my system C++ Borland Builder 3
//Things to do : Look a little clanlibish and add headers
// Some cleanup MUST BE MADE for non Borland users (Read nearly all of you)
// This ONLY works with DX7
//I am not even sure that this code can actually play some music right now
//I need to have a look at those damn string function for unicode first


class ClanMidi
{
    private:
        IDirectMusicPerformance* CL_MIDIPerformance;
        IDirectMusicLoader* CL_MIDILoader;
        IDirectMusicSegment* CL_MIDISegment;
        IDirectMusicSegmentState* CL_MIDISegmentState;
        IDirectMusicPerformance* CreatePerformance();
        IDirectMusicLoader* CreateLoader();

    public:
        ClanMidi();
        ~ClanMidi();

        bool StartPlaying();
        bool StopPlaying();
        bool LoadMIDIFile(wchar_t* MIDIFileName);
};                                    