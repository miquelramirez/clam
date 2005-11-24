/*
 * MelodyAnalyzer.hxx
 * Interface for MelodyAnalyzer class.
 * Used to extract the melody based on the characteristics obtained
 * during the analysis. MIDI melody is obtained from the extracted
 * melody.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#ifndef _MELODYANALYZER_
#define _MELODYANALYZER_

#include <CLAM/Melody.hxx>
#include <CLAM/MIDIMelody.hxx>
#include <CLAM/Segment.hxx>
#include <CLAM/SegmentDescriptors.hxx>
#include <CLAM/Array.hxx>

using CLAM::Melody;
using CLAM::Segment;
using CLAM::SegmentDescriptors;
using CLAM::TData;
using CLAM::Array;
using CLAM::DataArray;
using CLAM::PitchNote;
using CLAM::MIDIMelody;
using CLAM::MediaTime;

class MelodyAnalyzer
{
public:
    MelodyAnalyzer();
    ~MelodyAnalyzer();

    void AnalyzeMelody(const Segment& segment, const SegmentDescriptors& descriptors);

    Melody GetMelody();
    void SetMelody(const Melody& m);
    MIDIMelody GetMIDIMelody();

    void StoreMelody(const std::string& filename);
    void StoreMIDIMelody(const std::string& filename);

private:
    Melody melody;
    MIDIMelody midiMelody;
    TData frequencies[85];
    Array<PitchNote> pitch;

    void Init();
    int GetPitchIndex(TData ff);
    void UpdateMidi();
    int GetVelocity(int v);
    
    DataArray GetEnergy(const Segment& segment, const SegmentDescriptors& descriptors);

    bool IsValid(const DataArray& energy,const MediaTime& time,TData sr);
};

#endif


