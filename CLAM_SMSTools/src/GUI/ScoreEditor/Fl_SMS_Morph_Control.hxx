#ifndef __FL_SMS_MORPH_CONTROL__
#define __FL_SMS_MORPH_CONTROL__

#include <FL/Fl_Group.H>
#include <string>
#include <map>
#include "BPF.hxx"
#include "Signalv0.hxx"
#include "Signalv1.hxx"
#include "Slotv0.hxx"

class  Fl_Choice;
class  Fl_Check_Button;
class  Fl_Group;

namespace CLAMVM
{

	class Fl_SMS_Morph_Control 
		: public Fl_Group 
	{
	public:
		Fl_SMS_Morph_Control( int X, int Y, int W, int H, const char* label = 0 );
		
		virtual ~Fl_SMS_Morph_Control();

		virtual void resize( int X, int Y, int W, int H );

		virtual int handle( int event );

		SigSlot::Signalv0        GlobalEnvelopeChanged;
		SigSlot::Signalv0        SinAmpEnvelopeChanged;
		SigSlot::Signalv0        PitchHybEnvelopeChanged;
		SigSlot::Signalv0        SinFreqEnvelopeChanged;
		SigSlot::Signalv0        ResAmpEnvelopeChanged;
		SigSlot::Signalv0        TimeSyncEnvelopeChanged;
		SigSlot::Signalv0        SinShapeW1EnvelopeChanged;
		SigSlot::Signalv0        SinShapeW2EnvelopeChanged;
		SigSlot::Signalv0        ResShapeW1EnvelopeChanged;
		SigSlot::Signalv0        ResShapeW2EnvelopeChanged;
		SigSlot::Signalv0        SinShapeEnvelopeChanged;
		SigSlot::Signalv0        ResShapeEnvelopeChanged;
		SigSlot::Signalv1<bool>  FrameInterpolationChanged;   

		void RetrieveGlobalEnvelope( CLAM::BPF& );
		void SetGlobalEnvelope( const CLAM::BPF& );

		void RetrieveSinAmpEnvelope( CLAM::BPF& );
		void SetSinAmpEnvelope( const CLAM::BPF& );

		void RetrievePitchHybEnvelope( CLAM::BPF& );
		void SetPitchHybEnvelope( const CLAM::BPF& );

		void RetrieveSinFreqEnvelope( CLAM::BPF & );
		void SetSinFreqEnvelope( const CLAM::BPF& );

		void RetrieveResAmpEnvelope( CLAM::BPF& );
		void SetResAmpEnvelope( const CLAM::BPF& );

		void RetrieveTimeSyncEnvelope( CLAM::BPF& );
		void SetTimeSyncEnvelope( const CLAM::BPF& );

		void RetrieveSinShapeW1Envelope( CLAM::BPF& );
		void SetSinShapeW1Envelope( const CLAM::BPF& );

		void RetrieveSinShapeW2Envelope( CLAM::BPF& );
		void SetSinShapeW2Envelope( const CLAM::BPF& );

		void RetrieveResShapeW1Envelope( CLAM::BPF& );
		void SetResShapeW1Envelope( const CLAM::BPF& );

		void RetrieveResShapeW2Envelope( CLAM::BPF& );
		void SetResShapeW2Envelope( const CLAM::BPF& );

		void RetrieveSinShapeEnvelope( CLAM::BPF& );
		void SetSinShapeEnvelope( const CLAM::BPF& );

		void RetrieveResShapeEnvelope( CLAM::BPF& );
		void SetResShapeEnvelope( const CLAM::BPF& );

		void ActivateFrameInterpolation();
		void DeactivateFrameInterpolation();

	protected:

		
		SigSlot::Slotv0       GlobalEnvelopeEdited;
		SigSlot::Slotv0       SinAmpEnvelopeEdited;
		SigSlot::Slotv0       PitchHybEnvelopeEdited;
		SigSlot::Slotv0       SinFreqEnvelopeEdited;
		SigSlot::Slotv0       ResAmpEnvelopeEdited;
		SigSlot::Slotv0       TimeSyncEnvelopeEdited;
		SigSlot::Slotv0       SinShapeW1EnvelopeEdited;
		SigSlot::Slotv0       SinShapeW2EnvelopeEdited;
		SigSlot::Slotv0       ResShapeW1EnvelopeEdited;
		SigSlot::Slotv0       ResShapeW2EnvelopeEdited;
		SigSlot::Slotv0       SinShapeEnvelopeEdited;
		SigSlot::Slotv0       ResShapeEnvelopeEdited;

		void OnGlobalEnvelopeEdition();
		void OnSinAmpEnvelopeEdition();
		void OnPitchHybEnvelopeEdition();
		void OnSinFreqEnvelopeEdition();
		void OnResAmpEnvelopeEdition();
		void OnTimeSyncEnvelopeEdition();
		void OnSinShapeW1EnvelopeEdition();
		void OnSinShapeW2EnvelopeEdition();
		void OnResShapeW1EnvelopeEdition();
		void OnResShapeW2EnvelopeEdition();
		void OnSinShapeEnvelopeEdition();
		void OnResShapeEnvelopeEdition();

		void InitEnvelopeSelectorContents();

		static void sMenuItemSelectedCb( Fl_Choice*, Fl_SMS_Morph_Control* );
		static void sFrameInterpolationCb( Fl_Check_Button*, Fl_SMS_Morph_Control* );

		void CreateEnvelopeEditors();
		
		void ShowEnvelopeEditorFor( const std::string& name );
		
		void ShowEnvelopeEditorFor( const char* name );
		
		void FrameInterpolationActivated( );
		
		void FrameInterpolationDeactivated();

		void DestroyEnvelopeEditors();

		void SetVisibleEnvelopeWidget( Fl_Widget* w );

		bool FirstTimeShown( ) const {
			return mFirstTimeShown;
		}

		void ShownOnce( ) {
			mFirstTimeShown = false;
		}

	protected: // envelope editors builder methods
		
		Fl_Widget* BuildGlobalEditor();
		Fl_Widget* BuildSinAmpEditor();
		Fl_Widget* BuildPitchHybEditor();
		Fl_Widget* BuildSinFreqEditor();
		Fl_Widget* BuildResAmpEditor();
		Fl_Widget* BuildTimeSyncEditor();
		Fl_Widget* BuildSinShapeW1Editor();
		Fl_Widget* BuildSinShapeW2Editor();
		Fl_Widget* BuildResShapeW1Editor();
		Fl_Widget* BuildResShapeW2Editor();
		Fl_Widget* BuildSinShapeEditor();
		Fl_Widget* BuildResShapeEditor();

	
	protected: // "dynamic widgets"

		typedef std::map< std::string, Fl_Widget* >   EnvelopeWidgetRepository;
		
		EnvelopeWidgetRepository                      mEnvelopeEditors;


		bool  MustBeSinAmpInSynch() const { 
			return mSynchSinAmpEditorWithGlobal;
		}

		bool  MustBeSinFreqInSynch() const {
			return mSynchSinFreqEditorWithGlobal;
		}

		bool  MustBePitchHybInSynch() const {
			return mSynchPitchHybEditorWithGlobal;
		}

		bool  MustBeResAmpInSynch() const {
			return mSynchResAmpEditorWithGlobal;
		}

		void  OnSynchStateForSinAmpEditorChanged( bool state );
		void  OnSynchStateForSinFreqEditorChanged( bool state );
		void  OnSynchStateForPitchHybEditorChanged( bool state );
		void  OnSynchStateForResAmpEditorChanged( bool state );

		SigSlot::Slotv1<bool>   ChangeSinAmpSynchState;
		SigSlot::Slotv1<bool>   ChangeSinFreqSynchState;
		SigSlot::Slotv1<bool>   ChangePitchHybSynchState;
		SigSlot::Slotv1<bool>   ChangeResAmpSynchState;

	protected: // "static" widgets
		
		Fl_Choice*        mpEnvelopeSelector;
		Fl_Check_Button*  mpFrameInterpSelector;
		Fl_Group*         mpEnvelopeContainer;
		
		const std::string mGlobalControlKey;
		const std::string mSinAmpControlKey;
		const std::string mPitchHybControlKey;
		const std::string mSinFreqControlKey;
		const std::string mResAmpControlKey;
		const std::string mTimeSyncControlKey;

		const std::string mSinShapeW1ControlKey;
		const std::string mSinShapeW2ControlKey;
		const std::string mResShapeW1ControlKey;
		const std::string mResShapeW2ControlKey;

		const std::string mSinShapeControlKey;
		const std::string mResShapeControlKey;
		
		bool  mFirstTimeShown;

		bool  mSynchSinAmpEditorWithGlobal;
		bool  mSynchPitchHybEditorWithGlobal;
		bool  mSynchSinFreqEditorWithGlobal;
		bool  mSynchResAmpEditorWithGlobal;

	};

}

#endif //Fl_SMS_Morph_Control.hxx
