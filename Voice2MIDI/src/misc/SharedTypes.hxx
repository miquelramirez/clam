/*
*	SharedTypes.hxx
*	Types shared for some classes.
*	
*	Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#ifndef _SHAREDTYPES_
#define _SHAREDTYPES_

#include <CLAM/Array.hxx>

using CLAM::DataArray;
using CLAM::TData;

typedef struct
{
    DataArray data;
    TData left;
    TData right;
    TData bottom;
    TData top;
    TData sampleRate;
} DisplayData;

typedef struct
{
	TData left;
	TData right;
} HBounds;

#endif

