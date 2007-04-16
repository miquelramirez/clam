/*
 * V2MPREditDisplay.hxx
 * Interface for V2MPREditDisplay class
 *
 * Rendering surface for the Piano Roll view.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#ifndef _V2MPREDITDISPLAY_
#define _V2MPREDITDISPLAY_

#include "V2MDataEditDisplay.hxx"

using CLAM::TData;

class V2MPREditDisplay : public V2MDataEditDisplay
{
public:
	V2MPREditDisplay(QWidget* parent);

	void SetNotesInfo(const Array<Point>& points,
		              const Array<int>& octave,
					  const Array<int>& veloc);

	void SetDataSizeAndSampleRate(TSize size,TData sampleRate);

protected:
	void initializeGL();
	void paintGL();

private:
	Array<int> _octave;
	Array<int> _veloc;
	TSize _dataSize;
	TData _sampleRate;

	void DrawGrid();
	void DrawNotes();
	void DrawNote(int pIndex,int mode);
	float* GetColor(int octave,int veloc);

	// available colors for 5 octaves (2..6)
	float* GetReed(int veloc);
	float* GetMagenta(int veloc);
	float* GetCyan(int veloc);
	float* GetBlue(int veloc);
	float* GetGrayScale(int veloc);
};

#endif

