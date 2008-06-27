#ifndef VumeterMonitor_hxx
#define VumeterMonitor_hxx

#include "Oscilloscope.hxx"
#include "OscilloscopeMonitor.hxx"

class VumeterMonitor : public OscilloscopeMonitor
{
	const char* GetClassName() const { return "Vumeter"; };
};


#endif//VumeterMonitor_hxx
