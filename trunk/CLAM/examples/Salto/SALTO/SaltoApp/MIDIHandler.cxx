#include "MIDIHandler.hxx"

#ifdef LINUX
  #include <pthread.h>
  extern pthread_mutex_t DSP_MIDI_mutex;
#endif //LINUX

namespace CLAM
{

	void MIDIHandlerConfig::DefaultInit()
	{
		AddAll();	
		UpdateData();
	}

	//----------------------------------------------------------------------------//
	//CSaltoHandleParamChanges::CSaltoHandleParamChanges(CSaltoParameter* pParams)
	  MIDIHandler::MIDIHandler():
	  mVelocityIn("Velocity", this, &MIDIHandler::UpdateVelocity),
	  mNoteIn("Note", this, &MIDIHandler::UpdateNote),
	  mPitchBendIn( "PitchBend", this, &MIDIHandler::UpdatePitchBend ),
	  mAirSpeedIn( "AirSpeed", this, &MIDIHandler::UpdateAirSpeed ),
	  mStatusOut( "Status", this ),
	  mLastPitch( 0 ),
	  mVelocity( 0.0 ),
	  mNote( 0.0 ),
	  mNoteOff( 0.0 ),
	  mPitchBend( 1.0 ),
	  mAirSpeed( 0.0 )
	{
		MIDIHandlerConfig cfg;
 
		Configure( cfg );
	}

	MIDIHandler::MIDIHandler( const MIDIHandlerConfig& cfg ):
		mVelocityIn("Velocity", this, &MIDIHandler::UpdateVelocity),
		mNoteIn("Note", this, &MIDIHandler::UpdateNote),
		mPitchBendIn( "PitchBend", this, &MIDIHandler::UpdatePitchBend ),
	    mAirSpeedIn( "AirSpeed", this, &MIDIHandler::UpdateAirSpeed ),
		mStatusOut( "Status", this ),
		mLastPitch( 0 ),
		mVelocity( 0.0 ),
		mNote( 0.0 ),
		mNoteOff( 0.0 ),
		mPitchBend( 1.0 ),
		mAirSpeed( 0.0 )
	{
		Configure( cfg );
	}

	MIDIHandler::~MIDIHandler()
	{
	}

	bool MIDIHandler::ConcreteConfigure( const ProcessingConfig& cfg)throw( std::bad_cast )
	{
		mConfig = dynamic_cast< const MIDIHandlerConfig& >( cfg );

		mTranspose = mConfig.GetTranspose();
		mPitchModRange = mConfig.GetPitchModRange();

		return true;
	}

	void MIDIHandler::NoteOn(Parameters& params)
	{
	double pitchShiftFactor=1;
	short mControl1, mControl2, mControl3, mControl4;

	if ( params.GetUseMidiKeyboard() || params.GetUseMelody() ) //   MIDI handling KEYBOARD
    {
     /*   mNote+=params.GetTranspose(); 
		if (mNote<=50) 
           mNote = 0;       //we work with two octaves 
         else if (mNote >=75)
           mNote =24;
         else              // 0 is lowest pitch
           mNote -= 50;
       */  
         mLastPitch = mNote;
         
         // MidiNote to Frequency :: F=440*2^((P-69)/12)
         params.SetStatResFreq(440.0*pow(2,(mNote+48-69)/12.0));
         
         
         if( params.GetPlay() )// this means transition !
         {
           params.SetTransitionInit(true); // step into trans init process
           params.SetTransitionMode(true);
		   params.SetDisplayedValuesChanged(true);
           //XA clutch
		   /*TIndex val = 50-mVelocity;
           val = CLIP(val,3,30);*/

		   //TIndex val = 5; // too much noise in the transition

		   TIndex val=2;
           params.SetAttackTimbreTransFrames(val);
         
           if (mNote>=params.GetAttackTimbre().GetPitch())   // check if transition up or down
            params.SetTransitionUpwards(true);
           else
            params.SetTransitionUpwards(false);
           params.SetTransitionInterval(abs(mNote-params.GetAttackTimbre().GetPitch()));
         }
         
         if (params.GetTransitionMode()) // check for note transition 
         {
			 
			 mStatusOut.SendControl( 2 ); // Sending transition state notification to CSaltoDSP
			 //std::cout << "Note: "<< mNote << std::endl;

            // set only transition specific parameters
            // we dont need a new attack
            params.SetAttackTimbrePitch(mNote);
            double ipFactor = mVelocity/60.0;
            /*
            if (ipFactor<0.1) 
              ipFactor=0.1;
            params.SetInterPolFactor(ipFactor); 
            */
            params.SetDisplayedValuesChanged(true);
         }
         else // play note from beginning, use attack and set timbre vektor
         {
			mStatusOut.SendControl( 1 );  // Sending Attack state notification to CSaltoDSP
           params.SetAttackTimbrePitch(mNote);
           
		   //XA: changed to meet breathcontroller params
           if (mVelocity>=100)          
              params.SetAttackTimbreAttack(VHARD);
           else if (mVelocity>=60)
              params.SetAttackTimbreAttack(HARD);
           else if (mVelocity>=25)
              params.SetAttackTimbreAttack(SOFT);
           else
              params.SetAttackTimbreAttack(VSOFT);       
            
           
           if (mVelocity>=50) // use forte stat templates
           {
              // select current stat template depending on pitch region
              if (mNote > 9)
                params.SetCurrentStatTemplate(7);
              else if (mNote > 6)
                params.SetCurrentStatTemplate(6);
              else if (mNote > 3)
                params.SetCurrentStatTemplate(5);
              else if (mNote >= 0)
                params.SetCurrentStatTemplate(4);
           
              // map breath volume to stationary timbre template 
              double ipFactor = (mVelocity-50)/140.0;
              params.SetInterPolFactor(ipFactor);
            }
            else // use piano stat templates
            {
              // select current stat template depending on pitch region
              if (mNote > 9)
                params.SetCurrentStatTemplate(3);
              else if (mNote > 6)
                params.SetCurrentStatTemplate(2);
              else if (mNote > 3)
                params.SetCurrentStatTemplate(1);
              else if (mNote >= 0)
                params.SetCurrentStatTemplate(0);
             
             // map breath volume to stationary timbre template 
              double ipFactor =mVelocity/50.0;
              if (ipFactor<0.1) 
                ipFactor=0.1;
              else if (ipFactor>=1)
                ipFactor=1;
              params.SetInterPolFactor(1-ipFactor);
            } 
            params.SetAttackTimbreLevel(mVelocity*0.5+63);  
            params.SetDisplayedValuesChanged(true); // display needs update
            params.SetPlay(true); // activate play
         }
      }
	else    // breathcontroller
    {
#ifdef LINUX
//pthread_mutex_lock(&DSP_MIDI_mutex);
#endif //LINUX
     /*    mNote = mNote+params.GetTranspose();
         if (mNote<=50) 
           mNote = 0;       //we work with two octaves 
         else if (mNote >=75)
           mNote =24;
         else              // 0 is lowest pitch
           mNote -= 50;
     */    
         mLastPitch = mNote;
         
         // MidiNote to Frequency :: F=440*2^((P-69)/12)
         params.SetStatResFreq(440.0*pow(2,(mNote+48-69)/12.0));
         
         if(params.GetPlay()               // this means transition !
            && params.GetAttackTimbre().GetLevel()>0
            &&(! params.GetTransitionInit())) 
         {
           params.SetTransitionInit(true); // step into trans init process
           params.SetTransitionMode(true);
           params.SetDisplayedValuesChanged(true); // display needs update 
  
           TIndex val = 30-mVelocity;
#ifdef TRUMPET
           val = CLIP(val,1,10);
#endif           
#ifdef ALTOSAX    
          val = CLIP(val,2,10);
#endif            
           
		  params.SetAttackTimbreTransFrames(val);   
				 
           if (mNote>=params.GetAttackTimbre().GetPitch())   // check if transition up or down
           	 params.SetTransitionUpwards(true);
           else
           	 params.SetTransitionUpwards(false);
         	 params.SetTransitionInterval(abs(mNote-params.GetAttackTimbre().GetPitch()));			 
         }
         
         if (params.GetTransitionMode()) // check for note transition 
         {
            // set only transition specific parameters
            // we dont need a new attack
            params.SetAttackTimbrePitch(mNote);
            mLastPitch = mNote;
            params.SetDisplayedValuesChanged(true);
         }
         else // play note from beginning, use attack and set timbre vektor
         {
            
           params.SetAttackTimbrePitch(mNote);
           mLastPitch = mNote;
           int attack_reduction = 25;
           mVelocity -= attack_reduction; //decrase attack velocity, because always is bigger than stationary part
//cout << " velocity: " << velocity << endl;
           if (mVelocity>=(125-attack_reduction)) //110
           {
              params.SetAttackTimbreAttack(VHARD);
              params.SetBreathOnlySound(false);
           }
           else if (mVelocity>=(85-attack_reduction))//(velocity>=60) //80
           {
              params.SetAttackTimbreAttack(HARD);
              params.SetBreathOnlySound(false);
           }
           else if (mVelocity>=(50-attack_reduction))
           {
              params.SetAttackTimbreAttack(SOFT);
              params.SetBreathOnlySound(false);
           }
           else if (mVelocity>=(25-attack_reduction))
           {
              params.SetAttackTimbreAttack(VSOFT);
              params.SetBreathOnlySound(false);
           }
           else
           {
              params.SetAttackTimbreAttack(VSOFT);
              params.SetBreathOnlySound(true);
//              params.SetAttackTimbreAttack(BREATHONLY);
          }

           params.SetDisplayedValuesChanged(true); // display needs update
           
           /* activate play */
           params.SetPlay(true);
         }
      }

	}	

	void MIDIHandler::NoteOff(Parameters& params)
	{

  //  mNote +=params.GetTranspose();
	/* NOTE OFF */
//    if (mNote<=50) 
//		mNote = 0;       //we work with two octaves 
	//else if (mNote >=75)
	//	mNote =24;
//	else              // 0 is lowest pitch
//		mNote -= 50;
   

    if ( params.GetUseMidiKeyboard() || params.GetUseMelody() ) //   MIDI handling KEYBOARD
    {
	  if(mNoteOff==params.GetAttackTimbre().GetPitch())
	  {
          params.SetPlay(false);
	  }
      // else ignore note off
      
      /* controller data on channel 11 */
/*     if(bytes[0]==186) 
/*      {
         /* Controller 1 */
/*        if(bytes[1]==32) 
        {
          mControl1 = (bytes[2]-MIDI_EXT_CTRL_OFFSET);
          TIndex volume = params.GetAttackTimbre().GetLevel()+mControl1;        
          if (volume>127)
            volume = 127;
          else if ( volume<0)
            volume = 0; 
          params.SetAttackTimbreLevel(volume);        
          params.SetDisplayedValuesChanged(true); // display needs update
        }        
        
        /* Controller 2 */
/*        else if(bytes[1]==33) 
        {
          mControl2 += 2*(bytes[2]-MIDI_EXT_CTRL_OFFSET);
          if (mControl2>MIDI_EXT_CTRL_RANGE)
            mControl2 = MIDI_EXT_CTRL_RANGE;
          if (mControl2<0)
            mControl2 = 0;  
          
          TFloat32 ipFactor = (mControl2 * 9.)/127 +1; //  -> [1,10]
          TFloat32 logFactor = log10(ipFactor);
          
          params.SetInterPolFactor(logFactor);
          params.SetDisplayedValuesChanged(true); // display needs update
        }
        
        /* Controller 3 */
/*        else if(bytes[1]==34) 
        {           
        }
        /* Controller 4 */
/*        else if(bytes[1]==35) 
        { 
            mControl4 += 2*(bytes[2]-MIDI_EXT_CTRL_OFFSET);
            if (mControl4>MIDI_EXT_CTRL_RANGE)
              mControl4=MIDI_EXT_CTRL_RANGE-1;
            if (mControl4<0)
              mControl4=0;  
                          
            TIndex tFrames = mControl4/8;
            if (tFrames<1)    // doesnt sound for 0 or 1 frame
              tFrames=1;
            params.SetAttackTimbreTransFrames(tFrames);
            params.SetDisplayedValuesChanged(true);
         }
       }
      /* Pitch Modulation Wheel */
/*      if(bytes[0]==224)
      {
        params.SetPitchModFactor(
          ((double)bytes[2]-70)/70.0*params.GetPitchModRange()+1.0);
       
      }*/
    } // end MIDI keyboard
// ======================================================================= //
// ======================================================================= //
// ======================================================================= //      
    else //breathcontroller
	{
      
    /*  mNote +=params.GetTranspose();
          
        if (mNote<=50) 
           mNote = 0;       //we work with two octaves 
         else if (mNote >=75)
           mNote =24;
         else              // 0 is lowest pitch
           mNote -= 50;
     */   
        
        if(params.GetAttackTimbre().GetLevel()==0)//params.GetAttackTimbre().GetPitch())
        {
          params.SetPlay(false);
          params.SetTransitionMode(false);
          params.SetTransitionInit(false);
        } 
        
        // else ignore note off
//      }
      
      
/*      if((bytes[0]==176)) // controller data on channel 1
      {
        if(bytes[1]==01)  /* keyboard  Modulation Wheel */
/*      {
            TIndex tFrames = bytes[2]/16;
            if (tFrames<2)    // doesnt sound for 0 or 1 frame
              tFrames=2;
            params.SetAttackTimbreTransFrames(tFrames);
            params.SetDisplayedValuesChanged(true);
        }
        
        /* WX5 breath control data */
/*        else if(bytes[1]==11) 
        {
          TIndex volume = bytes[2];
//cout << " volume: " << volume << endl;
          params.SetAttackTimbreLevel(volume);
          pitch = params.GetAttackTimbre().GetPitch();
          
          if (volume>=100) // use forte stat templates
          {
            // select current stat template depending on pitch region
            if (pitch > 9)
              params.SetCurrentStatTemplate(7);
            else if (pitch > 6)
              params.SetCurrentStatTemplate(6);
            else if (pitch > 3)
              params.SetCurrentStatTemplate(5);
            else if (pitch >= 0)
              params.SetCurrentStatTemplate(4);
           
           // map breath volume to stationary timbre template 
            double ipFactor = (volume-100)/200.0;
            params.SetInterPolFactor(ipFactor);
          }
          else // use piano stat templates
          {
            // select current stat template depending on pitch region
            if (pitch > 9)
              params.SetCurrentStatTemplate(3);
            else if (pitch > 6)
              params.SetCurrentStatTemplate(2);
            else if (pitch > 3)
              params.SetCurrentStatTemplate(1);
            else if (pitch >= 0)
              params.SetCurrentStatTemplate(0);
           
           // map breath volume to stationary timbre template 
            double ipFactor =volume/100.0;
            if (ipFactor<0.02) 
              ipFactor=0.02;
            else if (ipFactor>=1)
              ipFactor=1;
            params.SetInterPolFactor(1-ipFactor);
          }
          
          if(params.GetAttackTimbre().GetLevel()==0)//params.GetAttackTimbre().GetPitch())
          {
            params.SetPlay(false);
            params.SetTransitionMode(false);
            params.SetTransitionInit(false);
          } 
          
          params.SetDisplayedValuesChanged(true); // display needs update
        }
        else if(bytes[1]==16) // WX5 wheel up
        {
           TIndex tFrames = bytes[2]/4;
            if (tFrames<2)    // doesnt sound for 0 or 1 frame
              tFrames=2;
            params.SetAttackTimbreTransFrames(tFrames);
            params.SetDisplayedValuesChanged(true);
        }
        else if(bytes[1]==17) // WX5 wheel down
        { 
          double ipFactor = bytes[2]/127.0;
          if (ipFactor>=1) ipFactor=1;
          params.SetInterPolFactor(1-ipFactor);
          params.SetDisplayedValuesChanged(true); // display needs update
        }
        else if(bytes[1]==65) // WX5 portamento switch
        { 
          if (bytes[2]==0)
            params.SetAttackTimbreTransFrames(12);
          else if (bytes[2]==127) 
            params.SetAttackTimbreTransFrames(3);           
          params.SetDisplayedValuesChanged(true); // display needs update
        }
      }
      /* Pitch Modulation Wheel */
/*      if((bytes[0]==224))
      {
        params.SetPitchModFactor(
          ((double)bytes[2]-70)/70.0*params.GetPitchModRange()+1.0);
      }
      
      /*
      if ((bytes[0]!=224)&&(bytes[0]!=176))
      {
          printf("\nMIDI IN : %d",timestamp);
          printf("%02x %02x %02x   %d",bytes[0],bytes[1],bytes[2],size);
          fflush(stdout);
      }
      */
//#ifdef LINUX
//pthread_mutex_unlock(&DSP_MIDI_mutex);
//#endif //LINUX
	} // end breath controller		
	}

	bool MIDIHandler::Do( Parameters &params )
	{
		if ( ( mStatus == eNoteOn )  && ( mLastPitch != mNote ) )
		{
			params.SetPitchModFactor( mPitchBend );
			NoteOn( params );
			//std::cout << "Note On "<< mNote << std::endl;
			//std::cout << "Note: "<< mNote << std::endl;
		}
		else if ( mStatus == eNoteOff )
		{
			NoteOff( params );

			mStatusOut.SendControl( 4 ); // Sending Release state notification to CSatloDSP
			//std::cout << "Note Off "<< mNote << std::endl;
			//std::cout << "Note: "<< mNote << std::endl;
		}
		else if( mStatus == eCtrAirSpeed )
		{
			CtrAirSpeed( params );
		}

		return true;
	}

	void MIDIHandler::CtrAirSpeed( Parameters &params )
	{
	//cout << " volume: " << volume << endl;
	  params.SetAttackTimbreLevel( mAirSpeed );
	  mNote = params.GetAttackTimbre().GetPitch();
  
	  if (mAirSpeed >= 100 ) // use forte stat templates
	  {
		// select current stat template depending on pitch region
		if (mNote > 9)
		  params.SetCurrentStatTemplate(7);
		else if (mNote > 6)
		  params.SetCurrentStatTemplate(6);
		else if (mNote > 3)
		  params.SetCurrentStatTemplate(5);
		else if (mNote >= 0)
		  params.SetCurrentStatTemplate(4);
   
	   // map breath volume to stationary timbre template 
		double ipFactor = ( mAirSpeed - 100) / 200.0;
		params.SetInterPolFactor(ipFactor);
	  }
	  else // use piano stat templates
	  {
		// select current stat template depending on pitch region
		if (mNote > 9)
		  params.SetCurrentStatTemplate(3);
		else if (mNote > 6)
		  params.SetCurrentStatTemplate(2);
		else if (mNote > 3)
		  params.SetCurrentStatTemplate(1);
		else if (mNote >= 0)
		  params.SetCurrentStatTemplate(0);
   
	   // map breath volume to stationary timbre template 
		double ipFactor = mAirSpeed / 100.0;
		if (ipFactor<0.02) 
		  ipFactor=0.02;
		else if (ipFactor>=1)
		  ipFactor=1;
		params.SetInterPolFactor(1-ipFactor);
	  }
  
	  if(params.GetAttackTimbre().GetLevel()==0)//params->GetAttackTimbre().GetPitch())
	  {
		params.SetPlay(false);
		params.SetTransitionMode(false);
		params.SetTransitionInit(false);
	  } 
  
	  params.SetDisplayedValuesChanged(true); // display needs update

	}

}

