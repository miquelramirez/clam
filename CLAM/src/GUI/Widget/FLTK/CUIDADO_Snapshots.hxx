#ifndef __CUIDADOSNAPSHOTS__
#define __CUIDADOSNAPSHOTS__

#include "DataTypes.hxx"
#include "Audio.hxx"
#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"

using CLAM::DataArray;
using CLAM::Audio;
using CLAM::Spectrum;
using CLAM::SpectralPeakArray;
using CLAM::TData;

namespace CLAMGUI
{
  void ShowSnapshot( DataArray& arr, const char* label = 0);
  
  void ShowSnapshot( Spectrum& spec, const char* label = 0, bool showmag = true, bool showphase = false );
  
  void ShowSnapshot( Audio& audio, const char* label = 0);

  void ShowSnapshot( Spectrum& sp, SpectralPeakArray& peakarray, const char* label = 0, bool indexed=false );

  void ShowSnapshot( Spectrum& sp, SpectralPeakArray& peakarray, DataArray &select, const char* label = 0, bool indexed=false );

  void ShowSnapshot( DataArray& arr1, DataArray& arr2, const char* label = 0);

  void ShowSnapshot( Audio& arr, CLAM::Array<int> mask, const char* label = 0, unsigned char r = 0, unsigned char g = 0, unsigned char b = 255);

	void ShowSnapshot(Spectrum* sp[]=0, const char* label=0 , int numBands=0, int size=0);

	void ShowSnapshot(Spectrum* sequence[]=0, int at_once = 5, const char* label = 0, int size=0 );
  
	void ShowSnapshot(DataArray &audio, DataArray &desc1, DataArray &desc2, const char* label);
	
	void ShowSnapshot(DataArray &audio, CLAM::Array<int> mask, DataArray &desc1, DataArray&desc2, const char* label);

  void ShowSnapshot( CLAM::Array< Spectrum* >& sequence, int at_once = 5, const char* label = 0 );
  
  void ShowSnapshot( CLAM::Array< DataArray* >& sequence, int at_once = 5, const char* label = 0 );

	void CalculateWndSize( int& x, int& y, int& w, int& h );

	void DrawArray( DataArray& arr, const char* label );
}

#endif // CuidadoSnapshots.hxx
