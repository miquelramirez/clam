#include "DummyGlobalData.hxx"

namespace CLAM
{

void DummyGlobalData::SetStaticResponseSpectrums(Spectrums * spectrums)
{
	_responseSpectrums = spectrums;
}

DummyGlobalData::Spectrums & DummyGlobalData::GetStaticResponseSpectrums()
{
	return *_responseSpectrums;

}
DummyGlobalData::Spectrums* DummyGlobalData::_responseSpectrums = 0;

}

