#include "CMTGErr.hxx"
#include "CMTGMIDIIn.hxx"
#include "CMTGMIDITarget.hxx"
#include <stdio.h>
#include <string.h>

CMTGMIDISettings::CMTGMIDISettings(const CMTGMIDICard& card, const CMTGMIDIDevice& device, bool blockmode)
:mCard(card), mDevice(device), mBlockmode(blockmode)
{
}

void CMTGMIDITarget::Handle(TMIDITime timestamp, const TMIDIByte bytes[], TInt32 size)
{
	printf("[%3d] %08d ", size, timestamp);
	for (int i = 0; i < size; i++)
	{
		printf("%02x ", bytes[i]);
	}
	printf("\n");
}
