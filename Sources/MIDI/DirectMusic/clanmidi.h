//---------------------------------------------------------------------------
#ifndef ClanMidiH
#define ClanMidiH
//---------------------------------------------------------------------------
#endif
#include "API/MIDI/cl_midi.h"
#include <dmusicc.h>
#include <dmusici.h>

//Okay this is the first draft of a WIN32 CLANLIB MIDI PLAYER
//It compiles on my system C++ Borland Builder 3
//Things to do : Look a little clanlibish and add headers
// Some cleanup MUST BE MADE for non Borland users (Read nearly all of you)
// This ONLY works with DX7
//I am not even sure that this code can actually play some music right now
//I need to have a look at those damn string function for unicode first


class CL_MIDI::CL_MIDI_DirectMusic
{
    private:
        IDirectMusicPerformance* CL_MIDIPerformance;
        IDirectMusicLoader* CL_MIDILoader;
        IDirectMusicSegment* CL_MIDISegment;
        IDirectMusicSegmentState* CL_MIDISegmentState;
        IDirectMusicPerformance* CreatePerformance();
        IDirectMusicLoader* CreateLoader();

    public:
        CL_MIDI_DirectMusic();
        ~CL_MIDI_DirectMusic();

        bool start_playing();
        bool stop_Playing();
        bool load_midi_file(wchar_t* midifileName);
};                                    