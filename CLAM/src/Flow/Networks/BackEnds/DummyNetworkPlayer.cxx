
#include "DummyNetworkPlayer.hxx"
#include "AudioSink.hxx"
#include "AudioSource.hxx"

#include <iostream>
#include <cmath>

namespace CLAM
{

void DummyNetworkPlayer::Start()
{
	if ( IsPlaying() )
		return;

	BePlaying();
	CacheSourcesAndSinks();
	const int frameSize = 512;

	TData idata[frameSize];
	TData odata[frameSize];

	for(int i=0;i<frameSize;i++)
		idata[i]=sin(i);

	for(unsigned i = 0; i < GetNSources(); ++i)
		SetSourceBuffer(i, &idata[0], frameSize);

	for( unsigned i = 0; i < GetNSinks(); ++i)
		SetSinkBuffer  (i, &odata[0], frameSize);

	GetNetwork().Do();
}

void DummyNetworkPlayer::Stop()
{
	if ( IsStopped() )
		return;
	BeStopped();
}

} //namespace

