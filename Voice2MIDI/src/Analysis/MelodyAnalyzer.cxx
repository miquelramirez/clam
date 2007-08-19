/*
 * MelodyAnalyzer.cxx
 * Implementation for MelodyAnalyzer class.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#include "MelodyAnalyzer.hxx"
#include <CLAM/Segmentator.hxx>
#include <CLAM/SegmentatorConfig.hxx>
#include <CLAM/XMLStorage.hxx>

using namespace CLAM;

MelodyAnalyzer::MelodyAnalyzer()
{
    Init();
}    

MelodyAnalyzer::~MelodyAnalyzer(){} 

void MelodyAnalyzer::Init()
{
    // init table of frequencies
    for(int i=0;i < 85;i++) frequencies[i]=TData(32.703*pow(2.0,double(i/12.0)));   
    // init pairs (pitch,octave)
    pitch.Resize(85);
    pitch.SetSize(85);
    int k,l;
    for(l=0; l < 7;l++)
    {
	for(k=0; k < 12; k++)
	{
	    pitch[12*l+k].SetPitch((EPitch)k);
	    pitch[12*l+k].SetOctave(l+1);
	}
    }

    melody.AddNumberOfNotes();
    melody.UpdateData();
    midiMelody.AddNumberOfNotes();
    midiMelody.UpdateData();
}

void MelodyAnalyzer::AnalyzeMelody(const Segment& segment, const SegmentDescriptors& descriptors)
{
    Segment mSegment(segment);
    SegmentDescriptors mSegmentDescriptors(descriptors);
    DataArray eData = GetEnergy(segment,descriptors);

    CLAM_ACTIVATE_FAST_ROUNDING;
    TData ePercentil, eThr, fPercentil, fThr, minLength;
    ePercentil = 50;
    eThr = TData(0.0032);
    fPercentil = 3;
    fThr = 0;
    minLength = 2;
    int i;

    //Configuring Segmentator to work only with fundamental and energy
    SegmentatorConfig sgConfig;
    TDescriptorsParams tmpParams;
    tmpParams.id=SpectralEnergyId;
    tmpParams.percentil=ePercentil;
    tmpParams.threshold=eThr;
    sgConfig.AddDescParams(tmpParams);
    tmpParams.id=FundamentalId;
    tmpParams.percentil=fPercentil;
    tmpParams.threshold=fThr;
    sgConfig.AddDescParams(tmpParams);
    sgConfig.SetMinSegmentLength(int(minLength));
    Segmentator segmentator(sgConfig);
    segmentator.Start();
    //Segmentate
    segmentator.Do(mSegment,mSegmentDescriptors);

    List<Note> array;
    List<MIDINote> midiNotes;
    Array<TData> fund;
    Array<TData> energy;

    // Compute Melody Description
    int nFrames=mSegment.GetnFrames();
    for(i=0; i < nFrames; i++)
    {
	fund.AddElem(mSegment.GetFrame(i).GetFundamental().GetFreq());
	if(mSegmentDescriptors.GetFrameD(i).GetSpectrumD().GetEnergy() > eThr)
	    energy.AddElem(mSegmentDescriptors.GetFrameD(i).GetSpectrumD().GetEnergy());
	else
	    energy.AddElem(0);
    }

    TData ff,aux,number,noteEnergy,lastFF=0,lastEnergy=0;
    TSize wSize  =  mSegment.GetFrame(0).GetAudioFrame().GetSize();
    int nChildren=mSegment.GetChildren().Size();
    for(i=0;i < nChildren;i++)
    {
	ff=aux=number=noteEnergy=0;
	mSegment.GetChildren()[i].SetHoldsData(false);
	MediaTime time;
	time.SetBegin(mSegment.GetChildren()[i].GetBeginTime());
	time.SetEnd(mSegment.GetChildren()[i].GetEndTime());

	Note myNote;
	MIDINote midiNote;
	myNote.AddPitchNote();
	myNote.UpdateData();

        // Compute Fundamental frequency mean
	TIndex b=Round(2*mSegment.GetChildren()[i].GetBeginTime()*mSegment.GetSamplingRate()/wSize);
	TIndex e;
	if(mSegment.GetChildren()[i].GetEndTime() < mSegment.GetEndTime())
	    e=Round(2*mSegment.GetChildren()[i].GetEndTime()*mSegment.GetSamplingRate()/wSize);
	else
	    e=Round(2*mSegment.GetEndTime()*mSegment.GetSamplingRate()/wSize);

	int j;
        // Compute mean
	int count=0;
	TData offsetTh=10;
	bool onset=false;
	for(j=b; j < e; j++)
	{
	    if(fund[j] > 87 && fund[j] < 1000)
	    {
		if(j < e-3)
		{
		    if(Abs(fund[j]-fund[j+1]) < offsetTh && Abs(fund[j]-fund[j+2]) < offsetTh)  //offset detected
		    {
			aux+=fund[j];
			count++;
			if(!onset)
			{
			    onset=true;
			    b=j;
			}
		    }
		}
	    }
	    if(j < e-3)
	    {
		if(Abs(fund[j]-fund[j+1]) > offsetTh && Abs(fund[j]-fund[j+2]) > offsetTh)  //offset detected
		{
		    if(onset)
		    {
			e=j;
			time.SetEnd(TData(.5*j*wSize/mSegment.GetSamplingRate()));
			break;
		    }
		}
	    }
	}
	aux/=count;
	if((e-b)<=0) break;
	Audio testEnergyAudio;
	testEnergyAudio.SetSize(e-b);
	DataArray& testEnergy=testEnergyAudio.GetBuffer();
	TData maxEnergy=0;
	for(j=b; j<e; j++)
	{
	    noteEnergy+=energy[j];
	    if(energy[j]>maxEnergy) maxEnergy=energy[j];
	    testEnergy.AddElem(energy[j]);
	    if((fund[j] < (aux+60))
	       &&(fund[j] > (std::max(TData(0.0),(aux-TData(60)))))
	       &&fund[j]>TData(60)
	       &&fund[j] < TData(1000))
	    {
		ff+=fund[j];
		number++;
	    }
	}
	if(number)
	    ff/=number;
	else 
	    ff=0;
	noteEnergy/=(e-b);
	if(ff < 60) continue;
	if(ff > 1000) continue;
	if(noteEnergy==0) noteEnergy=lastEnergy;
	lastFF=ff;
	lastEnergy=noteEnergy;

	if(IsValid(eData,time,mSegment.GetSamplingRate()))
	{
	    myNote.SetFundFreq(ff);
	    myNote.SetEnergy(noteEnergy);
	    myNote.SetTime(time);
	    myNote.SetPitchNote(pitch[GetPitchIndex(ff)]);

	    midiNote.SetKey( myNote.GetNoteNumber() );
	    midiNote.SetVelocity( GetVelocity(myNote.GetVelocity()) );
	    midiNote.SetTime(myNote.GetTime());

	    array.AddElem(myNote);
	    midiNotes.AddElem(midiNote);
	}
    }

    melody.SetNoteArray(array);
    melody.SetNumberOfNotes(array.Size());
    midiMelody.SetNoteArray(midiNotes);
    midiMelody.SetNumberOfNotes(midiNotes.Size()); 	

    CLAM_DEACTIVATE_FAST_ROUNDING;
}

int MelodyAnalyzer::GetPitchIndex(TData ff)
{
    TData min = 10000;
    TIndex m=0,n=0;
    for(m=0; m < 84; m++)
    {
	if(fabsf(frequencies[m]-ff) < min)
	{
	    n = m;
	    min = fabsf(frequencies[m]-ff);
	}
    }
    return n;
} 

Melody MelodyAnalyzer::GetMelody()
{
    return melody;
} 

void MelodyAnalyzer::SetMelody(const Melody& m)
{
    melody = m;
    UpdateMidi();
}

MIDIMelody MelodyAnalyzer::GetMIDIMelody()
{
    return midiMelody;
}

void MelodyAnalyzer::UpdateMidi()
{
    List<MIDINote> midiList;
    MIDINote midi_note;
    for(int i = 0;i < melody.GetNoteArray().Size();i++)
    {
	midi_note.SetKey(melody.GetNoteArray()[i].GetNoteNumber());
	midi_note.SetVelocity( GetVelocity(melody.GetNoteArray()[i].GetVelocity()) );
	midi_note.SetTime(melody.GetNoteArray()[i].GetTime());
	midiList.AddElem(midi_note);
    } 	
    midiMelody.SetNoteArray(midiList);
    midiMelody.SetNumberOfNotes(midiList.Size());
}

void MelodyAnalyzer::StoreMelody(const std::string& filename)
{
    XMLStorage x;
    x.UseIndentation(true);
    x.Dump(melody,"Analyzed_Melody",filename);
}

void MelodyAnalyzer::StoreMIDIMelody(const std::string& filename)
{
    XMLStorage x;
    x.UseIndentation(true);
    x.Dump(midiMelody,"Analyzed_MIDIMelody",filename);
}

int MelodyAnalyzer::GetVelocity(int v)
{
    return 50+int(v*77.0/127.0);
}

DataArray MelodyAnalyzer::GetEnergy(const Segment& segment, const SegmentDescriptors& descriptors)
{
    Segment seg(segment);
    SegmentDescriptors desc(descriptors);

    int nFrames = seg.GetnFrames();
    TSize nSamples = seg.GetAudio().GetSize();
    TSize step = nSamples/nFrames;
	
    // energy
    TData energy;
    DataArray values;
    values.Resize(nSamples);
    values.SetSize(nSamples);
    int k=0;
    for(int i=0;i < nFrames;i++)
    {
	energy = desc.GetFrameD(i).GetSpectrumD().GetEnergy();
	for(int j = 0; j < step;j++)
	{
	    values[k++]=energy;
	}
    }
    return values;
}

bool MelodyAnalyzer::IsValid(const DataArray& energy,
			     const MediaTime& time,
			     TData sr)
{
    DataArray data(energy);
    MediaTime t(time);
    TData dur = TData(TData(data.Size())/sr);
    bool ok = (t.GetBegin() > 0.07 && t.GetEnd() <= dur);
    if(!ok) return false;
    int bucket = int((t.GetBegin()+((t.GetEnd()-t.GetBegin())/2.0))*sr);

    return (data[bucket] > 0.007);
}

// END


