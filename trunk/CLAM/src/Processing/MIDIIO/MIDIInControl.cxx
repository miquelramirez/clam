#include "MIDIInControl.hxx"

namespace CLAM {

	MIDIInControl::MIDIInControl():MIDIIn(false)
	{ 
		mpDevice = 0;
		Configure(MIDIInConfig());
	}

	MIDIInControl::MIDIInControl(const MIDIInConfig &c):MIDIIn(false)
	{
		mpDevice = 0;
		Configure(c);
	}

	bool MIDIInControl::ConcreteConfigure(const ProcessingConfig& c)
		throw(ErrProcessingObj)
	{
		bool ret = MIDIIn::ConcreteConfigure(c);
		if (ret==false) return false;

#ifdef DEBUGGING_MIDIIO
		printf("MIDIInControl::ConcreteConfigure\n");
#endif

		for (int status = 0;status<8;status++)
		{
#ifdef DEBUGGING_MIDIIO
			printf("receiving %s [%x %x]? ",MIDI::GetMessageInfo(status).name,
				mConfig.GetChannelMask(),mConfig.GetMessageMask()
			);
#endif
			if (mConfig.GetMessageMask()&MIDI::GetMessageInfo(status).mask)
			{
#ifdef DEBUGGING_MIDIIO
				printf("YES\n");
#endif
				int firstRelevant = 0;
				if (mConfig.GetFilter()!=0xFF) firstRelevant = 1;

				if ((MIDI::Message)status == MIDI::ePitchbend)
				{
					/* we make an exception for pitchbend: instead of putting
					   out to values (LSB, MSB), we prefer 1 14bit value.
						 see also the code in Handle
					*/
#ifdef DEBUGGING_MIDIIO
					printf("adding %s:Value port\n",
						MIDI::GetMessageInfo(status).name);
#endif
					char tmp[256];
					sprintf(tmp,"%s:Value",
						MIDI::GetMessageInfo(status).name);
					mOutControls.AddElem(new OutControl(tmp,this));
					mOutControlInfos.AddElem(new OutControlInfo((MIDI::Message)status,0));
				}
				else
				{
					if(MIDI::GetMessageInfo(status).length<=1)
				    {
						mOutControls.AddElem(new OutControl("non-data byte",this));
						mOutControlInfos.AddElem(new OutControlInfo((MIDI::Message)status,0));
					}
					else
					{
						for (int i=firstRelevant;i<MIDI::GetMessageInfo(status).length-1;i++)
						{
							if (MIDI::GetMessageInfo(status).field[i])
							{
	#ifdef DEBUGGING_MIDIIO
								printf("adding %s:%s port\n",
									   MIDI::GetMessageInfo(status).name,
									   MIDI::GetMessageInfo(status).field[i]);
	#endif							
								char tmp[256];
								sprintf(tmp,"%s:%s",
										MIDI::GetMessageInfo(status).name,
										MIDI::GetMessageInfo(status).field[i]);
								mOutControls.AddElem(new OutControl(tmp,this));
								mOutControlInfos.AddElem(new OutControlInfo((MIDI::Message)status,i));
							}
						}
					}
				}
			}else{
#ifdef DEBUGGING_MIDIIO
				printf("NO\n");
#endif
			}
		}

		return true;
	}

	void MIDIInControl::Handle(unsigned char* msg,int size)
	{
		int n = mOutControls.Size();
		for (int i=n-1;i>=0;i--)
		{
			if (MIDI::StatusByteToMessage(msg[0])==mOutControlInfos[i]->mMessage)
			{
#ifdef DEBUGGING_MIDIIO
				printf("%20s %20s %2d %02x\n",GetName().c_str(),
					mOutControls[i]->GetName().c_str(),
					mOutControlInfos[i]->mField,
					msg[mOutControlInfos[i]->mField+1]);
				fflush(stdout);
#endif
				if (mOutControlInfos[i]->mMessage == MIDI::ePitchbend)
				{
					/* we make an exception for pitchbend: instead of putting
					   out to values (LSB, MSB), we prefer 1 14bit value.
						 see also the code in ConcreteConfigure
					*/
					GetOutControls().GetByNumber(i).SendControl(
						msg[mOutControlInfos[i]->mField+1]+
						(msg[mOutControlInfos[i]->mField+2]<<7)
					);
				}
				else if (mOutControlInfos[i]->mMessage == MIDI::eSystem)
				{
					GetOutControls().GetByNumber(i).SendControl(1);
				}
				else
				{
					GetOutControls().GetByNumber(i).SendControl(msg[mOutControlInfos[i]->mField+1]);
				}
			}
		}
	}


}
