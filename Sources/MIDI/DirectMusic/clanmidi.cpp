//---------------------------------------------------------------------------
#pragma hdrstop

#include "clanmidi.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//Okay this is the first draft of a WIN32 CLANLIB MIDI PLAYER
//It compiles on my system C++ Borland Builder 3
//Things to do : Look a little clanlibish and add headers
// Some cleanup MUST BE MADE for non Borland users (Read nearly all of you)
// This ONLY works with DX7
//I am not even sure that this code can actually play some music right now
//I need to have a look at those damn string function for unicode first

ClanMidi::ClanMidi()
{
    if (SUCCEEDED(CoInitialize(NULL)))
    {
        CL_MIDIPerformance = CreatePerformance();
        if (CL_MIDIPerformance != NULL)
        {
            if (SUCCEEDED(CL_MIDIPerformance->Init(NULL, NULL, NULL)))
            {
                if (FAILED(CL_MIDIPerformance->AddPort(NULL)))
                {
                    CL_MIDIPerformance->CloseDown();
                    CL_MIDIPerformance->Release();
                };
            };
        };
        CL_MIDILoader = CreateLoader();
        CL_MIDISegment = NULL;
        CL_MIDISegmentState = NULL;
    };
};

ClanMidi::~ClanMidi()
{
    CL_MIDIPerformance->CloseDown();
    CL_MIDIPerformance->Release();
    CL_MIDILoader->Release();
    CL_MIDISegment = NULL;
    CL_MIDISegmentState = NULL;
    CoUninitialize();
};

IDirectMusicPerformance* ClanMidi::CreatePerformance()
{
    IDirectMusicPerformance* pPerf;

    if (FAILED(CoCreateInstance(
            CLSID_DirectMusicPerformance,
            NULL,
            CLSCTX_INPROC,
            IID_IDirectMusicPerformance2,
            (void**)&pPerf
        )))
    {
        pPerf = NULL;
    }
    return pPerf;
};

IDirectMusicLoader* ClanMidi::CreateLoader()
{
    IDirectMusicLoader* pLoader;

    if (FAILED(CoCreateInstance(
            CLSID_DirectMusicLoader,
            NULL,
            CLSCTX_INPROC,
            IID_IDirectMusicLoader,
            (void**)&pLoader
        )))
    {
        pLoader = NULL;
    }
    return pLoader;
};

bool ClanMidi::StartPlaying()
{
    if (FAILED(CL_MIDIPerformance->PlaySegment(CL_MIDISegment, 0, 0,
                &CL_MIDISegmentState)))
        return false;
    return true;
};

bool ClanMidi::StopPlaying()
{
    if (FAILED(CL_MIDIPerformance->Stop( NULL, NULL, 0, 0 )))
        return false;
    return true;
};

bool ClanMidi::LoadMIDIFile(wchar_t* MIDIFilename)
{

    DMUS_OBJECTDESC ObjDesc;

    ObjDesc.guidClass = CLSID_DirectMusicSegment;
    ObjDesc.dwSize = sizeof(DMUS_OBJECTDESC);
    wcscpy( ObjDesc.wszFileName, MIDIFilename );    
    ObjDesc.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_FILENAME;

    CL_MIDILoader->GetObject(&ObjDesc,
            IID_IDirectMusicSegment2, (void**) &CL_MIDISegment);
    CL_MIDISegment->SetParam(GUID_StandardMIDIFile,
            -1, 0, 0, (void*)CL_MIDIPerformance);
    CL_MIDISegment->SetParam(GUID_Download, -1, 0, 0, (void*)CL_MIDIPerformance);

    return true;
};                                        

