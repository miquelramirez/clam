#include "MIDIHandler.hxx"

namespace CLAM
{

	void MIDIHandlerConfig::DefaultInit()
	{
		AddAll();	
		UpdateData();
	}

	MIDIHandler::MIDIHandler():
		mVelocityIn("Velocity", this, &MIDIHandler::UpdateVelocity),
		mNoteIn("Note", this, &MIDIHandler::UpdateNote),
		mPitchBendIn( "PitchBend", this, &MIDIHandler::UpdatePitchBend ),
		mAirSpeedIn( "AirSpeed", this, &MIDIHandler::UpdateAirSpeed ),
		mVelocity( 0.0 ),
		mParams(0)
	{
		MIDIHandlerConfig cfg;
 
		Configure( cfg );

		AdoptChildren();
	}

	MIDIHandler::MIDIHandler( const MIDIHandlerConfig& cfg ):
		mVelocityIn("Velocity", this, &MIDIHandler::UpdateVelocity),
		mNoteIn("Note", this, &MIDIHandler::UpdateNote),
		mPitchBendIn( "PitchBend", this, &MIDIHandler::UpdatePitchBend ),
	    mAirSpeedIn( "AirSpeed", this, &MIDIHandler::UpdateAirSpeed ),
		mVelocity( 0.0 ),
		mParams(0)
	{
		Configure( cfg );

		AdoptChildren();
	}

	MIDIHandler::~MIDIHandler()
	{ 
	}

	bool MIDIHandler::ConcreteConfigure( const ProcessingConfig& c)
	{
	
		bool r = true;

		CopyAsConcreteConfig(mConfig, c);
		try {
			if( !ConfigureChildren() )
			{
				std::string aux(mStatus);
				mStatus = "\nFailed to configure children:\n";
				mStatus += aux;
				r = false;
			}
		}
		catch(std::exception &e)
		{
			mStatus += "Failed to configure children:\n";
			mStatus += e.what();
			return false;
		}

		if( mControl.GetExecState() == Processing::Ready )
			InitializeControlValues();

		return r;
	}

	bool MIDIHandler::ConcreteStart()
	{
		ProcessingComposite::ConcreteStart();

		return true;
	}

	bool MIDIHandler::ConcreteStop()
	{
		ProcessingComposite::ConcreteStop();

		return true;
	}

	void MIDIHandler::AdoptChildren()
	{
		mControl.SetParent(this);
	}

	bool MIDIHandler::ConfigureChildren()
	{
		ControllerConfig cfg;

		cfg.SetName("MIDIController");
		cfg.SetNumControls(eNUMControls);

		if(!mControl.Configure(cfg))
		{
			mStatus += "mControl: ";
			mStatus += mControl.GetStatus();
			return false;
		}

		return true;
	}

	void MIDIHandler::InitializeControlValues()
	{
		mControl.OutValues[ eStateControl ] = eIdle;
	}

	int MIDIHandler::UpdateNote ( TControlData note )
	{
		int	pitch = int(note+Params().GetTranspose());
		int velocity = int(mVelocity);

		pitch -= 50;
		if (pitch<0) pitch=0;
		else if (pitch>24) pitch=24;
			
		if( Params().GetUseBreathController() )
		{
			
			if (velocity)
			{
				// MidiNote to Frequency :: F=440*2^((P-69)/12)
				Params().SetStatResFreq(440.0*pow(2,(pitch+48-69)/12.0));

				if(Params().GetPlay()               // this means transition !
					&& (Params().GetAttackTimbre().GetLevel()>0)
					&& (!Params().GetTransitionInit() ) ) 
				{
					mControl.EnqueueControl( eStateControl ,(TControlData( eTransition )));
					Params().SetTransitionInit(true); // step into trans init process
					Params().SetTransitionMode(true);
					Params().SetDisplayedValuesChanged(true); // display needs update 

					TIndex val = 30-velocity;
					if (TRUMPET)
						val = CLIP(val,1,10);
					if (ALTOSAX)
					val = CLIP(val,2,10);
					//Params().SetAttackTimbreTransFrames(val);    

					if (pitch>=Params().GetAttackTimbre().GetPitch())   // check if transition up or down
						Params().SetTransitionUpwards(true);
					else
						Params().SetTransitionUpwards(false);
						Params().SetTransitionInterval(abs(pitch-Params().GetAttackTimbre().GetPitch()));

					// set only transition specific parameters
					// we dont need a new attack
					Params().SetAttackTimbrePitch(pitch);
					Params().SetDisplayedValuesChanged(true);
				}
				else // play note from beginning, use attack and set timbre vektor
				{
					mControl.EnqueueControl( eStateControl ,(TControlData( eAttack )));
					Params().SetAttackTimbrePitch(pitch);
					int attack_reduction = 25;
					velocity -= attack_reduction; //decrase attack velocity, because always is bigger than stationary part
					if (velocity>=(125-attack_reduction)) //110
					{
						Params().SetAttackTimbreAttack(VHARD);
						Params().SetBreathOnlySound(false);
					}
					else if (velocity>=(85-attack_reduction))//(velocity>=60) //80
					{
						Params().SetAttackTimbreAttack(HARD);
						Params().SetBreathOnlySound(false);
					}
					else if (velocity>=(50-attack_reduction))
					{
						Params().SetAttackTimbreAttack(SOFT);
						Params().SetBreathOnlySound(false);
					}
					else if (velocity>=(25-attack_reduction))
					{
						Params().SetAttackTimbreAttack(VSOFT);
						Params().SetBreathOnlySound(false);
					}
					else
					{
						Params().SetAttackTimbreAttack(VSOFT);
						Params().SetBreathOnlySound(true);
					//              Params().SetAttackTimbreAttack(BREATHONLY);
					}
					Params().SetDisplayedValuesChanged(true); // display needs update
					/* activate play */
					Params().SetPlay(true);
				}
			}
			else 
			{
				// note off

				if(Params().GetAttackTimbre().GetLevel()==0)//Params().GetAttackTimbre().GetPitch())
				{
					Params().SetPlay(false);
					Params().SetTransitionMode(false);
					Params().SetTransitionInit(false);
					mControl.EnqueueControl( eStateControl ,(TControlData( eRelease )));
					// else ignore note off
				}
			}
		}
		else
		{
			if(velocity)
			{
			 
			// MidiNote to Frequency :: F=440*2^((P-69)/12)
			 Params().SetStatResFreq(440.0*pow(2,(pitch+48-69)/12.0));
 			 
			 if( Params().GetPlay() )// this means transition !
			 {
			   mControl.EnqueueControl( eStateControl ,(TControlData( eTransition )));
			   Params().SetTransitionInit(true); // step into trans init process
			   Params().SetTransitionMode(true);
			   Params().SetDisplayedValuesChanged(true);
			   //XA clutch
			   /*TIndex val = 50-velocity;
			   val = CLIP(val,3,30);*/

			   //TIndex val = 5; // too much noise in the transition

			   TIndex val=2;
			   Params().SetAttackTimbreTransFrames(val);
     
			   if (pitch>=Params().GetAttackTimbre().GetPitch())   // check if transition up or down
				Params().SetTransitionUpwards(true);
			   else
				Params().SetTransitionUpwards(false);
			   Params().SetTransitionInterval(abs(pitch-Params().GetAttackTimbre().GetPitch()));

			   				 
				// set only transition specific parameters
				// we dont need a new attack
				Params().SetAttackTimbrePitch(pitch);
				/*
				double ipFactor = velocity/60.0;
				if (ipFactor<0.1) 
				  ipFactor=0.1;
				Params().SetInterPolFactor(ipFactor); 
				*/
				Params().SetDisplayedValuesChanged(true);

			 } 
			 else
			 {
			   mControl.EnqueueControl( eStateControl ,(TControlData( eAttack )));
			   Params().SetAttackTimbrePitch(pitch);
       
			   //XA: changed to meet breathcontroller Params()
			   if (velocity>=100)          
				  Params().SetAttackTimbreAttack(VHARD);
			   else if (velocity>=60)
				  Params().SetAttackTimbreAttack(HARD);
			   else if (velocity>=25)
				  Params().SetAttackTimbreAttack(SOFT);
			   else
				  Params().SetAttackTimbreAttack(VSOFT);       
        
       
			   if (velocity>=50) // use forte stat templates
			   {
				  // select current stat template depending on pitch region
				  if (pitch > 9)
					Params().SetCurrentStatTemplate(7);
				  else if (pitch > 6)
					Params().SetCurrentStatTemplate(6);
				  else if (pitch > 3)
					Params().SetCurrentStatTemplate(5);
				  else if (pitch >= 0)
					Params().SetCurrentStatTemplate(4);
       
				  // map breath volume to stationary timbre template 
				  double ipFactor = (velocity-50)/140.0;
				  Params().SetInterPolFactor(ipFactor);
				}
				else // use piano stat templates
				{
				  // select current stat template depending on pitch region
				  if (pitch > 9)
					Params().SetCurrentStatTemplate(3);
				  else if (pitch > 6)
					Params().SetCurrentStatTemplate(2);
				  else if (pitch > 3)
					Params().SetCurrentStatTemplate(1);
				  else if (pitch >= 0)
					Params().SetCurrentStatTemplate(0);
         
				 // map breath volume to stationary timbre template 
				  double ipFactor =velocity/50.0;
				  if (ipFactor<0.1) 
					ipFactor=0.1;
				  else if (ipFactor>=1)
					ipFactor=1;
				  Params().SetInterPolFactor(1-ipFactor);
				} 
				Params().SetAttackTimbreLevel(velocity*0.5+63);  
				Params().SetDisplayedValuesChanged(true); // display needs update
				Params().SetPlay(true); // activate play			 
			 }

			}
			else 
			{
			  if(pitch==Params().GetAttackTimbre().GetPitch())
			  {
				  Params().SetPlay(false);
				  mControl.EnqueueControl( eStateControl ,(TControlData( eRelease )));
			  }	
			}
		}

		return 0;
	}

	int MIDIHandler::UpdatePitchBend( TControlData value )
	{
		value *= TControlData(127)/TControlData(16384);

	  	Params().SetPitchModFactor(
    	((double)value-70)/70.0*mConfig.GetPitchModRange()+1.0);

		return 0;
	}

	int MIDIHandler::UpdateAirSpeed( TControlData airSpeed )
	{
		int volume = int(airSpeed);

		Params().SetAttackTimbreLevel(volume);

		int pitch = Params().GetAttackTimbre().GetPitch();

		if (volume>=100) // use forte stat templates
		{
			// select current stat template depending on pitch region
			if (pitch > 9)
				Params().SetCurrentStatTemplate(7);
			else if (pitch > 6)
				Params().SetCurrentStatTemplate(6);
			else if (pitch > 3)
				Params().SetCurrentStatTemplate(5);
			else if (pitch >= 0)
				Params().SetCurrentStatTemplate(4);
			// map breath volume to stationary timbre template 
			double ipFactor = (volume-100)/200.0;
			Params().SetInterPolFactor(ipFactor);
		}
		else // use piano stat templates
		{
			// select current stat template depending on pitch region
			if (pitch > 9)
				Params().SetCurrentStatTemplate(3);
			else if (pitch > 6)
				Params().SetCurrentStatTemplate(2);
			else if (pitch > 3)
				Params().SetCurrentStatTemplate(1);
			else if (pitch >= 0)
				Params().SetCurrentStatTemplate(0);

			// map breath volume to stationary timbre template 
			double ipFactor =volume/100.0;
			if (ipFactor<0.02) 
				ipFactor=0.02;
			else if (ipFactor>=1)
				ipFactor=1;
			Params().SetInterPolFactor(1-ipFactor);
		}

		if(Params().GetAttackTimbre().GetLevel()==0)//Params().GetAttackTimbre().GetPitch())
		{
			Params().SetPlay(false);
			Params().SetTransitionMode(false);
			Params().SetTransitionInit(false);
		} 

		Params().SetDisplayedValuesChanged(true); // display needs update
		return 0;
	}
}
