#include "CSaltoMIDITarget.hxx"
#include <stdio.h>

#ifdef LINUX
  #include <pthread.h>
  extern pthread_mutex_t DSP_MIDI_mutex;
#endif //LINUX


//----------------------------------------------------------------------------//
//CSaltoMIDITarget::CSaltoMIDITarget(CSaltoParameter* pParams)
CSaltoMIDITarget::CSaltoMIDITarget(MTG::Parameters* pParams)
{
	mpParams = pParams;
	mLastPitch=0;
}

CSaltoMIDITarget::~CSaltoMIDITarget()
{
}


void CSaltoMIDITarget::Handle(TMIDITime timetag, const TMIDIByte bytes[], TInt32 size)
{
  TMIDITime timestamp;
//  TMIDIByte bytes[256];
  TIndex velocity=0;
  TIndex pitch= 48;
  double pitchShiftFactor=1;
  short mControl1, mControl2, mControl3, mControl4;

    if (mpParams->GetUseMidiKeyboard()) //   MIDI handling KEYBOARD
    {
       if(bytes[0]==144 && bytes[2]!=0 ) // NOTE on channel 1
       { 
         pitch = bytes[1]+mpParams->GetTranspose();
         velocity = bytes[2];

         if (pitch<=50) 
           pitch = 0;       //we work with two octaves 
         else if (pitch >=75)
           pitch =24;
         else              // 0 is lowest pitch
           pitch -= 50;
         
         mLastPitch = pitch;
         
         // MidiNote to Frequency :: F=440*2^((P-69)/12)
         mpParams->SetStatResFreq(440.0*pow(2,(pitch+48-69)/12.0));
         
         
         if(mpParams->GetPlay()) // this means transition !
         {
           mpParams->SetTransitionInit(true); // step into trans init process
           mpParams->SetTransitionMode(true);
           TIndex val = 50-velocity;
           val = CLIP(val,3,30);
           mpParams->SetAttackTimbreTransFrames(val);
           mpParams->SetDisplayedValuesChanged(true); // display needs update 
         
           if (pitch>=mpParams->GetAttackTimbre().GetPitch())   // check if transition up or down
            mpParams->SetTransitionUpwards(true);
           else
            mpParams->SetTransitionUpwards(false);
           mpParams->SetTransitionInterval(abs(pitch-mpParams->GetAttackTimbre().GetPitch()));
         }
         
         if (mpParams->GetTransitionMode()) // check for note transition 
         {
            // set only transition specific parameters
            // we dont need a new attack
            mpParams->SetAttackTimbrePitch(pitch);
            double ipFactor = velocity/60.0;
            /*
            if (ipFactor<0.1) 
              ipFactor=0.1;
            mpParams->SetInterPolFactor(ipFactor); 
            */
            mpParams->SetDisplayedValuesChanged(true);
         }
         else // play note from beginning, use attack and set timbre vektor
         {
            
           mpParams->SetAttackTimbrePitch(pitch);
            
           if (velocity>=70)          
              mpParams->SetAttackTimbreAttack(VHARD);
           else if (velocity>=50)
              mpParams->SetAttackTimbreAttack(HARD);
           else if (velocity>=20)
              mpParams->SetAttackTimbreAttack(SOFT);
           else
              mpParams->SetAttackTimbreAttack(VSOFT);       
            
           
           if (velocity>=50) // use forte stat templates
           {
              // select current stat template depending on pitch region
              if (pitch > 9)
                mpParams->SetCurrentStatTemplate(7);
              else if (pitch > 6)
                mpParams->SetCurrentStatTemplate(6);
              else if (pitch > 3)
                mpParams->SetCurrentStatTemplate(5);
              else if (pitch >= 0)
                mpParams->SetCurrentStatTemplate(4);
           
              // map breath volume to stationary timbre template 
              double ipFactor = (velocity-50)/140.0;
              mpParams->SetInterPolFactor(ipFactor);
            }
            else // use piano stat templates
            {
              // select current stat template depending on pitch region
              if (pitch > 9)
                mpParams->SetCurrentStatTemplate(3);
              else if (pitch > 6)
                mpParams->SetCurrentStatTemplate(2);
              else if (pitch > 3)
                mpParams->SetCurrentStatTemplate(1);
              else if (pitch >= 0)
                mpParams->SetCurrentStatTemplate(0);
             
             // map breath volume to stationary timbre template 
              double ipFactor =velocity/50.0;
              if (ipFactor<0.1) 
                ipFactor=0.1;
              else if (ipFactor>=1)
                ipFactor=1;
              mpParams->SetInterPolFactor(1-ipFactor);
            } 
            mpParams->SetAttackTimbreLevel(velocity*0.5+63);  
            mpParams->SetDisplayedValuesChanged(true); // display needs update
            mpParams->SetPlay(true); // activate play
         }
      }
      
      /* NOTE OFF */
      if((bytes[0]==128) || (bytes[0]==144 && bytes[2]==0))
      {
        pitch = bytes[1]+mpParams->GetTranspose();
                  
        if (pitch<=50) 
           pitch = 0;       //we work with two octaves 
         else if (pitch >=75)
           pitch =24;
         else              // 0 is lowest pitch
           pitch -= 50;
        
        if(pitch==mpParams->GetAttackTimbre().GetPitch())
          mpParams->SetPlay(false);
        // elso ignore note off
      }
      
      /* controller data on channel 11 */
      if(bytes[0]==186) 
      {
         /* Controller 1 */
        if(bytes[1]==32) 
        {
          mControl1 = (bytes[2]-MIDI_EXT_CTRL_OFFSET);
          TIndex volume = mpParams->GetAttackTimbre().GetLevel()+mControl1;        
          if (volume>127)
            volume = 127;
          else if ( volume<0)
            volume = 0; 
          mpParams->SetAttackTimbreLevel(volume);        
          mpParams->SetDisplayedValuesChanged(true); // display needs update
        }        
        
        /* Controller 2 */
        else if(bytes[1]==33) 
        {
          mControl2 += 2*(bytes[2]-MIDI_EXT_CTRL_OFFSET);
          if (mControl2>MIDI_EXT_CTRL_RANGE)
            mControl2 = MIDI_EXT_CTRL_RANGE;
          if (mControl2<0)
            mControl2 = 0;  
          
          TFloat32 ipFactor = (mControl2 * 9.)/127 +1; //  -> [1,10]
          TFloat32 logFactor = log10(ipFactor);
          
          mpParams->SetInterPolFactor(logFactor);
          mpParams->SetDisplayedValuesChanged(true); // display needs update
        }
        
        /* Controller 3 */
        else if(bytes[1]==34) 
        {           
        }
        /* Controller 4 */
        else if(bytes[1]==35) 
        { 
            mControl4 += 2*(bytes[2]-MIDI_EXT_CTRL_OFFSET);
            if (mControl4>MIDI_EXT_CTRL_RANGE)
              mControl4=MIDI_EXT_CTRL_RANGE-1;
            if (mControl4<0)
              mControl4=0;  
                          
            TIndex tFrames = mControl4/8;
            if (tFrames<1)    // doesnt sound for 0 or 1 frame
              tFrames=1;
            mpParams->SetAttackTimbreTransFrames(tFrames);
            mpParams->SetDisplayedValuesChanged(true);
         }
       }
      /* Pitch Modulation Wheel */
      if(bytes[0]==224)
      {
        mpParams->SetPitchModFactor(
          ((double)bytes[2]-70)/70.0*mpParams->GetPitchModRange()+1.0);
       
      }
    } // end MIDI keyboard
// ======================================================================= //
// ======================================================================= //
// ======================================================================= //      
    else    // breathcontroller
    {
#ifdef LINUX
//pthread_mutex_lock(&DSP_MIDI_mutex);
#endif //LINUX
      if(bytes[0]==144 && bytes[2]!=0 ) // NOTE on channel 1
      { 
          /*// TEST
          printf("\n NoteOn : ");
          printf("%02x %02x %02x   %d",bytes[0],bytes[1],bytes[2],size);
          fflush(stdout);
          // TEST
          */

         pitch = bytes[1]+mpParams->GetTranspose();
         velocity = bytes[2];

         if (pitch<=50) 
           pitch = 0;       //we work with two octaves 
         else if (pitch >=75)
           pitch =24;
         else              // 0 is lowest pitch
           pitch -= 50;
         
         mLastPitch = pitch;
         
         // MidiNote to Frequency :: F=440*2^((P-69)/12)
         mpParams->SetStatResFreq(440.0*pow(2,(pitch+48-69)/12.0));
         
         if(mpParams->GetPlay()               // this means transition !
            && mpParams->GetAttackTimbre().GetLevel()>0
            &! mpParams->GetTransitionInit()) 
         {
           mpParams->SetTransitionInit(true); // step into trans init process
           mpParams->SetTransitionMode(true);
           mpParams->SetDisplayedValuesChanged(true); // display needs update 
  
           TIndex val = 30-velocity;
#ifdef TRUMPET
           val = CLIP(val,1,10);
#endif           
#ifdef ALTOSAX    
          val = CLIP(val,2,10);
#endif            
           //mpParams->SetAttackTimbreTransFrames(val);    
				 
           if (pitch>=mpParams->GetAttackTimbre().GetPitch())   // check if transition up or down
           	 mpParams->SetTransitionUpwards(true);
           else
           	 mpParams->SetTransitionUpwards(false);
         	 mpParams->SetTransitionInterval(abs(pitch-mpParams->GetAttackTimbre().GetPitch()));			 
         }
         
         if (mpParams->GetTransitionMode()) // check for note transition 
         {
            // set only transition specific parameters
            // we dont need a new attack
            mpParams->SetAttackTimbrePitch(pitch);
            mLastPitch = pitch;
            mpParams->SetDisplayedValuesChanged(true);
         }
         else // play note from beginning, use attack and set timbre vektor
         {
            
           mpParams->SetAttackTimbrePitch(pitch);
           mLastPitch = pitch;
           int attack_reduction = 25;
           velocity -= attack_reduction; //decrase attack velocity, because always is bigger than stationary part
//cout << " velocity: " << velocity << endl;
           if (velocity>=(125-attack_reduction)) //110
           {
              mpParams->SetAttackTimbreAttack(VHARD);
              mpParams->SetBreathOnlySound(false);
           }
           else if (velocity>=(85-attack_reduction))//(velocity>=60) //80
           {
              mpParams->SetAttackTimbreAttack(HARD);
              mpParams->SetBreathOnlySound(false);
           }
           else if (velocity>=(50-attack_reduction))
           {
              mpParams->SetAttackTimbreAttack(SOFT);
              mpParams->SetBreathOnlySound(false);
           }
           else if (velocity>=(25-attack_reduction))
           {
              mpParams->SetAttackTimbreAttack(VSOFT);
              mpParams->SetBreathOnlySound(false);
           }
           else
           {
              mpParams->SetAttackTimbreAttack(VSOFT);
              mpParams->SetBreathOnlySound(true);
//              mpParams->SetAttackTimbreAttack(BREATHONLY);
          }

           mpParams->SetDisplayedValuesChanged(true); // display needs update
           
           /* activate play */
           mpParams->SetPlay(true);
         }
      }
      
      /* NOTE OFF */
      if((bytes[0]==128) || (bytes[0]==144 && bytes[2]==0))
      {
        pitch = bytes[1]+mpParams->GetTranspose();
          
/*        // TEST
        printf("\n NoteOff : ");
        printf("%02x %02x %02x   %d",bytes[0],bytes[1],bytes[2],size);
        fflush(stdout);
*/      // TEST
        
        if (pitch<=50) 
           pitch = 0;       //we work with two octaves 
         else if (pitch >=75)
           pitch =24;
         else              // 0 is lowest pitch
           pitch -= 50;
        
        
        if(mpParams->GetAttackTimbre().GetLevel()==0)//mpParams->GetAttackTimbre().GetPitch())
        {
          mpParams->SetPlay(false);
          mpParams->SetTransitionMode(false);
          mpParams->SetTransitionInit(false);
        } 
        
        // else ignore note off
      }
      
      
      if((bytes[0]==176)) // controller data on channel 1
      {
        if(bytes[1]==01)  /* keyboard  Modulation Wheel */
        {
            TIndex tFrames = bytes[2]/16;
            if (tFrames<2)    // doesnt sound for 0 or 1 frame
              tFrames=2;
            mpParams->SetAttackTimbreTransFrames(tFrames);
            mpParams->SetDisplayedValuesChanged(true);
        }
        
        /* WX5 breath control data */
        else if(bytes[1]==11) 
        {
          TIndex volume = bytes[2];
//cout << " volume: " << volume << endl;
          mpParams->SetAttackTimbreLevel(volume);
          pitch = mpParams->GetAttackTimbre().GetPitch();
          
          if (volume>=100) // use forte stat templates
          {
            // select current stat template depending on pitch region
            if (pitch > 9)
              mpParams->SetCurrentStatTemplate(7);
            else if (pitch > 6)
              mpParams->SetCurrentStatTemplate(6);
            else if (pitch > 3)
              mpParams->SetCurrentStatTemplate(5);
            else if (pitch >= 0)
              mpParams->SetCurrentStatTemplate(4);
           
           // map breath volume to stationary timbre template 
            double ipFactor = (volume-100)/200.0;
            mpParams->SetInterPolFactor(ipFactor);
          }
          else // use piano stat templates
          {
            // select current stat template depending on pitch region
            if (pitch > 9)
              mpParams->SetCurrentStatTemplate(3);
            else if (pitch > 6)
              mpParams->SetCurrentStatTemplate(2);
            else if (pitch > 3)
              mpParams->SetCurrentStatTemplate(1);
            else if (pitch >= 0)
              mpParams->SetCurrentStatTemplate(0);
           
           // map breath volume to stationary timbre template 
            double ipFactor =volume/100.0;
            if (ipFactor<0.02) 
              ipFactor=0.02;
            else if (ipFactor>=1)
              ipFactor=1;
            mpParams->SetInterPolFactor(1-ipFactor);
          }
          
          if(mpParams->GetAttackTimbre().GetLevel()==0)//mpParams->GetAttackTimbre().GetPitch())
          {
            mpParams->SetPlay(false);
            mpParams->SetTransitionMode(false);
            mpParams->SetTransitionInit(false);
          } 
          
          mpParams->SetDisplayedValuesChanged(true); // display needs update
        }
        else if(bytes[1]==16) // WX5 wheel up
        {
           TIndex tFrames = bytes[2]/4;
            if (tFrames<2)    // doesnt sound for 0 or 1 frame
              tFrames=2;
            mpParams->SetAttackTimbreTransFrames(tFrames);
            mpParams->SetDisplayedValuesChanged(true);
        }
        else if(bytes[1]==17) // WX5 wheel down
        { 
          double ipFactor = bytes[2]/127.0;
          if (ipFactor>=1) ipFactor=1;
          mpParams->SetInterPolFactor(1-ipFactor);
          mpParams->SetDisplayedValuesChanged(true); // display needs update
        }
        else if(bytes[1]==65) // WX5 portamento switch
        { 
          if (bytes[2]==0)
            mpParams->SetAttackTimbreTransFrames(12);
          else if (bytes[2]==127) 
            mpParams->SetAttackTimbreTransFrames(3);           
          mpParams->SetDisplayedValuesChanged(true); // display needs update
        }
      }
      /* Pitch Modulation Wheel */
      if((bytes[0]==224))
      {
        mpParams->SetPitchModFactor(
          ((double)bytes[2]-70)/70.0*mpParams->GetPitchModRange()+1.0);
      }
      
      /*
      if ((bytes[0]!=224)&&(bytes[0]!=176))
      {
          printf("\nMIDI IN : %d",timestamp);
          printf("%02x %02x %02x   %d",bytes[0],bytes[1],bytes[2],size);
          fflush(stdout);
      }
      */
#ifdef LINUX
//pthread_mutex_unlock(&DSP_MIDI_mutex);
#endif //LINUX
    } // end breath controller
}
