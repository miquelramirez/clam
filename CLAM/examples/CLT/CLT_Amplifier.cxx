#include "CLT_Amplifier.hxx"
#include <stdlib.h>
#include "ErrProcessingObj.hxx"
#include <iostream>

void CLT_Amplifier::ConnectPort(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation)
{
	CLAM_ASSERT(sizeof(TData) == sizeof(float), "TData must be float.");
	switch (Port)
	{
	case AMP_CONTROL:
		((CLT_Amplifier *)Instance)->mControl = (TData*)DataLocation;
		break;
	case AMP_INPUT:
		((CLT_Amplifier *)Instance)->sIBuffer.GetBuffer().SetPtr((TData*)DataLocation,2048);
		break;
	case AMP_OUTPUT:
		((CLT_Amplifier *)Instance)->sOBuffer.GetBuffer().SetPtr((TData*)DataLocation,2048);
		break;
	}
}

LADSPA_Handle CLT_Amplifier::Instantiate
( const struct _LADSPA_Descriptor* Descriptor, 
  unsigned long SampleRate )
{
	return new CLT_Amplifier;
}

void CLT_Amplifier::Run(LADSPA_Handle Instance, unsigned long SampleCount)
{
	
	CLT_Amplifier* amp;
	amp = (CLT_Amplifier*) Instance;
	sIBuffer.GetBuffer().SetSize(SampleCount);
	sOBuffer.GetBuffer().SetSize(SampleCount);
	std::cout << SampleCount << std::endl;
	amp->Do(sIBuffer, sOBuffer);
}

bool CLT_Amplifier::Do(Audio &in, Audio &out)
{
	TData size     = out.GetSize();
	TData* outp    = &(out.GetBuffer()[0]);
	TData* inp     = &(in.GetBuffer()[0]);
	TData value    = *mControl;
	TData* endoutp = &(out.GetBuffer()[size-1]);
	
	while( outp!=endoutp )
	{
		*outp++ = value * (*inp++);
	}
	
	return true;
}

void initialise_CLT_amplifier()
{
	CLT_Descriptor * g_psDescriptor;
	
	g_psDescriptor = new CLT_Descriptor
		(1100,
		 "amp_mono_class",
		 "CLAM Mono Amplifier",
		 "Xavi Rubio, MTG (LADSPA test)",
		 "None",
		 3);
	
	g_psDescriptor->SetProperties(0,0,1);
	
	CLT_Port * ampControl = new CLT_Port(1,0,0,1,"Gain");
	CLT_Port * ampInput   = new CLT_Port(1,0,1,0,"Input");
	CLT_Port * ampOutput  = new CLT_Port(0,1,1,0,"Output");

	ampControl->CreateHint(1,0,0,0,1,0);
	ampControl->SetLowerBound(0);
	
	g_psDescriptor->ConnectPort(ampControl);
	g_psDescriptor->ConnectPort(ampInput);
	g_psDescriptor->ConnectPort(ampOutput);
	
	CONNECT( g_psDescriptor , CLT_Amplifier );
	
	registerNewPluginDescriptor(g_psDescriptor);
}

