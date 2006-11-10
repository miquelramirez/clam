#include "Fl_SMS_Morph_Control.hxx"
#include <FL/Fl.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Check_Button.H>
#include "Fl_SMS_BPF_Editor.hxx"
#include "Fl_SMS_BPF_Sync_Editor.hxx"
#include <iostream>

namespace CLAMVM
{

	Fl_SMS_Morph_Control::Fl_SMS_Morph_Control( int X, int Y, int W, int H, const char* label ) 
		: Fl_Group( X,Y,W,H,label ),
		  mGlobalControlKey( "Global morph factor vs. time"),
		  mSinAmpControlKey(  "Sinusoidal component amplitude hybridization" ),
		  mPitchHybControlKey( "Pitch hybridization" ),
		  mSinFreqControlKey( "Sinusoidal component frequency hybridization" ),
		  mResAmpControlKey( "Residual component amplitude hybridization" ),
		  mTimeSyncControlKey( "Time synchronization" ),
		  mSinShapeW1ControlKey( "Sinusoidal spectral shape weight A" ),
		  mSinShapeW2ControlKey( "Sinusoidal spectral shape weight B" ),
		  mResShapeW1ControlKey( "Residual spectral shape weight A" ),
		  mResShapeW2ControlKey( "Residual spectral shape weight B" ),
		  mSinShapeControlKey( "Sinusoidal spectral shape interpolation" ),
		  mResShapeControlKey( "Residual spectral shape interpolation" ),
		  mFirstTimeShown( true ),
		  mSynchSinAmpEditorWithGlobal( true ), mSynchSinFreqEditorWithGlobal( true ),
		  mSynchPitchHybEditorWithGlobal( true ), mSynchResAmpEditorWithGlobal( true )
	{
		mpFrameInterpSelector = new Fl_Check_Button( X+5, Y+5, 80, 20 );
		mpFrameInterpSelector->label( "Interpolate intermediate frames" );
		mpFrameInterpSelector->labelsize( 12 );
		mpFrameInterpSelector->tooltip( "Enable for allow interpolation of frames "
						"while morphing sounds of different lenghts" );
		mpFrameInterpSelector->when( FL_WHEN_CHANGED );
		mpFrameInterpSelector->callback( (Fl_Callback*)sFrameInterpolationCb, this );
		mpFrameInterpSelector->down_box(FL_DOWN_BOX);
		mpFrameInterpSelector->value(0);
  		
		mpEnvelopeSelector = new Fl_Choice( X+5, Y+45, 150, 20, "Sound hybridization controls"  );
		mpEnvelopeSelector->labelsize( 12 );
		mpEnvelopeSelector->textsize( 12 );
		mpEnvelopeSelector->down_box(FL_BORDER_BOX);
		mpEnvelopeSelector->align(FL_ALIGN_TOP_LEFT);
		
		mpEnvelopeContainer = new Fl_Group( X+10, Y+80,  W-20, H-85, "No label for now" );
		mpEnvelopeContainer->labelsize( 12 );
		mpEnvelopeContainer->box( FL_ENGRAVED_BOX );
		mpEnvelopeContainer->end();
		
		end();

		GlobalEnvelopeEdited.Wrap( this, &Fl_SMS_Morph_Control::OnGlobalEnvelopeEdition );
		SinAmpEnvelopeEdited.Wrap( this, &Fl_SMS_Morph_Control::OnSinAmpEnvelopeEdition );
		PitchHybEnvelopeEdited.Wrap( this, &Fl_SMS_Morph_Control::OnPitchHybEnvelopeEdition );
		SinFreqEnvelopeEdited.Wrap( this, &Fl_SMS_Morph_Control::OnSinFreqEnvelopeEdition );
		ResAmpEnvelopeEdited.Wrap( this, &Fl_SMS_Morph_Control::OnResAmpEnvelopeEdition );
		TimeSyncEnvelopeEdited.Wrap( this, &Fl_SMS_Morph_Control::OnTimeSyncEnvelopeEdition );
		SinShapeW1EnvelopeEdited.Wrap( this, &Fl_SMS_Morph_Control::OnSinShapeW1EnvelopeEdition );
		SinShapeW2EnvelopeEdited.Wrap( this, &Fl_SMS_Morph_Control::OnSinShapeW2EnvelopeEdition );
		ResShapeW1EnvelopeEdited.Wrap( this, &Fl_SMS_Morph_Control::OnResShapeW1EnvelopeEdition );
		ResShapeW2EnvelopeEdited.Wrap( this, &Fl_SMS_Morph_Control::OnResShapeW2EnvelopeEdition );
		SinShapeEnvelopeEdited.Wrap( this, &Fl_SMS_Morph_Control::OnSinShapeEnvelopeEdition );
		ResShapeEnvelopeEdited.Wrap( this, &Fl_SMS_Morph_Control::OnResShapeEnvelopeEdition );

		ChangeSinAmpSynchState.Wrap( this, &Fl_SMS_Morph_Control::OnSynchStateForSinAmpEditorChanged );
		ChangeSinFreqSynchState.Wrap( this, &Fl_SMS_Morph_Control::OnSynchStateForSinFreqEditorChanged );
		ChangePitchHybSynchState.Wrap( this, &Fl_SMS_Morph_Control::OnSynchStateForPitchHybEditorChanged );
		ChangeResAmpSynchState.Wrap( this, &Fl_SMS_Morph_Control::OnSynchStateForResAmpEditorChanged );

		InitEnvelopeSelectorContents();
		CreateEnvelopeEditors();

	}

	void Fl_SMS_Morph_Control::CreateEnvelopeEditors()
	{
		mEnvelopeEditors[ mGlobalControlKey ] = BuildGlobalEditor();
		mEnvelopeEditors[ mSinAmpControlKey ] = BuildSinAmpEditor();
		mEnvelopeEditors[ mPitchHybControlKey ] = BuildPitchHybEditor();
		mEnvelopeEditors[ mSinFreqControlKey ] = BuildSinFreqEditor();
		mEnvelopeEditors[ mResAmpControlKey ] = BuildResAmpEditor();
		mEnvelopeEditors[ mTimeSyncControlKey ] = BuildTimeSyncEditor();
		mEnvelopeEditors[ mSinShapeW1ControlKey ] = BuildSinShapeW1Editor();
		mEnvelopeEditors[ mSinShapeW2ControlKey ] = BuildSinShapeW2Editor();
		mEnvelopeEditors[ mResShapeW1ControlKey ] = BuildResShapeW1Editor();
		mEnvelopeEditors[ mResShapeW2ControlKey ] = BuildResShapeW2Editor();
		mEnvelopeEditors[ mSinShapeControlKey ] = BuildSinShapeEditor();
		mEnvelopeEditors[ mResShapeControlKey ] = BuildResShapeEditor();
	}
	
	void Fl_SMS_Morph_Control::DestroyEnvelopeEditors()
	{
		EnvelopeWidgetRepository::iterator i;

		for ( i = mEnvelopeEditors.begin(); i != mEnvelopeEditors.end(); i++ )
		{
			if ( i->second->parent() == NULL )
				delete i->second;
		}
	}

	void Fl_SMS_Morph_Control::ActivateFrameInterpolation()
	{
		mpFrameInterpSelector->value(1);
		mpFrameInterpSelector->redraw();
	}

	void Fl_SMS_Morph_Control::DeactivateFrameInterpolation()
	{
		mpFrameInterpSelector->value(0);
	}

	void Fl_SMS_Morph_Control::RetrieveGlobalEnvelope( CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mGlobalControlKey ] );

		editor->InsertPointsIntoBPF( bpf );
	}

	void Fl_SMS_Morph_Control::SetGlobalEnvelope( const CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mGlobalControlKey ] );

		editor->Clear();
		editor->InitPoints( bpf );
		editor->damage( FL_DAMAGE_ALL );
	}

	void Fl_SMS_Morph_Control::RetrieveSinAmpEnvelope( CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mSinAmpControlKey ] );
		
		editor->InsertPointsIntoBPF( bpf );
	}

	void Fl_SMS_Morph_Control::SetSinAmpEnvelope( const CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mSinAmpControlKey ] );
		
		editor->Clear();
		editor->InitPoints( bpf );
		editor->damage( FL_DAMAGE_ALL );		
	}

	void Fl_SMS_Morph_Control::RetrievePitchHybEnvelope( CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mPitchHybControlKey ] );

		editor->InsertPointsIntoBPF( bpf );
	}

	void Fl_SMS_Morph_Control::SetPitchHybEnvelope( const CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mPitchHybControlKey ] );
		
		editor->Clear();
		editor->InitPoints( bpf );
		editor->damage( FL_DAMAGE_ALL );
		
	}

	void Fl_SMS_Morph_Control::RetrieveSinFreqEnvelope( CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mSinFreqControlKey ] );

		editor->InsertPointsIntoBPF( bpf );
	}

	void Fl_SMS_Morph_Control::SetSinFreqEnvelope( const CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mSinFreqControlKey ] );
		
		editor->Clear();
		editor->InitPoints( bpf );
		editor->damage( FL_DAMAGE_ALL );
	}

	void Fl_SMS_Morph_Control::RetrieveResAmpEnvelope( CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mResAmpControlKey ] );

		editor->InsertPointsIntoBPF( bpf );
	}

	void Fl_SMS_Morph_Control::SetResAmpEnvelope( const CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mResAmpControlKey ] );
		
		editor->Clear();
		editor->InitPoints( bpf );
		editor->damage( FL_DAMAGE_ALL );
	}

	void Fl_SMS_Morph_Control::RetrieveTimeSyncEnvelope( CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mTimeSyncControlKey ] );

		editor->InsertPointsIntoBPF( bpf );
	}

	void Fl_SMS_Morph_Control::SetTimeSyncEnvelope( const CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mTimeSyncControlKey ] );
		
		editor->Clear();
		editor->InitPoints( bpf );
		editor->damage( FL_DAMAGE_ALL );
	}

	void Fl_SMS_Morph_Control::RetrieveSinShapeW1Envelope( CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mSinShapeW1ControlKey ] );

		editor->InsertPointsIntoBPF( bpf );
	}

	void Fl_SMS_Morph_Control::SetSinShapeW1Envelope( const CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mSinShapeW1ControlKey ] );
		
		editor->Clear();
		editor->InitPoints( bpf );
		editor->damage( FL_DAMAGE_ALL );
	}

	void Fl_SMS_Morph_Control::RetrieveSinShapeW2Envelope( CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mSinShapeW2ControlKey ] );

		editor->InsertPointsIntoBPF( bpf );
	}

	void Fl_SMS_Morph_Control::SetSinShapeW2Envelope( const CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mSinShapeW2ControlKey ] );
		
		editor->Clear();
		editor->InitPoints( bpf );
		editor->damage( FL_DAMAGE_ALL );
	}

	void Fl_SMS_Morph_Control::RetrieveResShapeW1Envelope( CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mResShapeW1ControlKey ] );

		editor->InsertPointsIntoBPF( bpf );
	}

	void Fl_SMS_Morph_Control::SetResShapeW1Envelope( const CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mResShapeW1ControlKey ] );
		
		editor->Clear();
		editor->InitPoints( bpf );
		editor->damage( FL_DAMAGE_ALL );
	}

	void Fl_SMS_Morph_Control::RetrieveResShapeW2Envelope( CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mResShapeW2ControlKey ] );

		editor->InsertPointsIntoBPF( bpf );
	}

	void Fl_SMS_Morph_Control::SetResShapeW2Envelope( const CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mResShapeW2ControlKey ] );
		
		editor->Clear();
		editor->InitPoints( bpf );
		editor->damage( FL_DAMAGE_ALL );
	}

	void Fl_SMS_Morph_Control::RetrieveSinShapeEnvelope( CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mSinShapeControlKey ] );

		editor->InsertPointsIntoBPF( bpf );
	}

	void Fl_SMS_Morph_Control::SetSinShapeEnvelope( const CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mSinShapeControlKey ] );
		
		editor->Clear();
		editor->InitPoints( bpf );
		editor->damage( FL_DAMAGE_ALL );
	}

	void Fl_SMS_Morph_Control::RetrieveResShapeEnvelope( CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mResShapeControlKey ] );

		editor->InsertPointsIntoBPF( bpf );
	}

	void Fl_SMS_Morph_Control::SetResShapeEnvelope( const CLAM::BPF& bpf )
	{
		Fl_SMS_BPF_Editor* editor = static_cast<Fl_SMS_BPF_Editor*>( mEnvelopeEditors[ mResShapeControlKey ] );
		
		editor->Clear();
		editor->InitPoints( bpf );
		editor->damage( FL_DAMAGE_ALL );
	}


	void Fl_SMS_Morph_Control::OnSynchStateForSinAmpEditorChanged( bool state )
	{
		mSynchSinAmpEditorWithGlobal = state;
	}

	void Fl_SMS_Morph_Control::OnSynchStateForSinFreqEditorChanged( bool state )
	{
		mSynchSinFreqEditorWithGlobal = state;
	}

	void Fl_SMS_Morph_Control::OnSynchStateForPitchHybEditorChanged( bool state )
	{
		mSynchPitchHybEditorWithGlobal = state;
	}

	void Fl_SMS_Morph_Control::OnSynchStateForResAmpEditorChanged( bool state )
	{
		mSynchResAmpEditorWithGlobal = state;
	}

	void Fl_SMS_Morph_Control::OnGlobalEnvelopeEdition()
	{
		GlobalEnvelopeChanged.Emit();
		CLAM::BPF globalBPF;

		RetrieveGlobalEnvelope( globalBPF );

		if ( MustBeSinAmpInSynch() )
			SetSinAmpEnvelope( globalBPF );
		if ( MustBeSinFreqInSynch() )
			SetSinFreqEnvelope( globalBPF );
		if ( MustBePitchHybInSynch() )
			SetPitchHybEnvelope( globalBPF );
		if ( MustBeResAmpInSynch() )
			SetResAmpEnvelope( globalBPF );
	}

	void Fl_SMS_Morph_Control::OnSinAmpEnvelopeEdition()
	{
		SinAmpEnvelopeChanged.Emit();
	}

	void Fl_SMS_Morph_Control::OnPitchHybEnvelopeEdition()
	{
		PitchHybEnvelopeChanged.Emit();
	}

	void Fl_SMS_Morph_Control::OnSinFreqEnvelopeEdition()
	{
		SinFreqEnvelopeChanged.Emit();
	}

	void Fl_SMS_Morph_Control::OnResAmpEnvelopeEdition()
	{
		ResAmpEnvelopeChanged.Emit();
	}

	void Fl_SMS_Morph_Control::OnTimeSyncEnvelopeEdition()
	{
		TimeSyncEnvelopeChanged.Emit();
	}

	void Fl_SMS_Morph_Control::OnSinShapeW1EnvelopeEdition()
	{
		SinShapeW1EnvelopeChanged.Emit();
	}

	void Fl_SMS_Morph_Control::OnSinShapeW2EnvelopeEdition()
	{
		SinShapeW2EnvelopeChanged.Emit();
	}

	void Fl_SMS_Morph_Control::OnResShapeW1EnvelopeEdition()
	{
		ResShapeW1EnvelopeChanged.Emit();
	}

	void Fl_SMS_Morph_Control::OnResShapeW2EnvelopeEdition()
	{
		ResShapeW2EnvelopeChanged.Emit();
	}

	void Fl_SMS_Morph_Control::OnSinShapeEnvelopeEdition()
	{
		SinShapeEnvelopeChanged.Emit();
	}

	void Fl_SMS_Morph_Control::OnResShapeEnvelopeEdition()
	{
		ResShapeEnvelopeChanged.Emit();
	}

	Fl_Widget* Fl_SMS_Morph_Control::BuildGlobalEditor()
	{
		Fl_SMS_BPF_Editor* widget = new Fl_SMS_BPF_Editor( 0, 0, 100, 100 );
		widget->label( mGlobalControlKey.c_str() );
		widget->SetHorizontalRange( 0.0, 1.0 );
		widget->SetVerticalRange( 0.0, 1.0 );
		widget->SetGridWidth( 0.1, 0.1 );
		widget->tooltip( "This widget allows to control the morph between the two sounds "
				 "being the X axis the time, and the Y axis the interpolation factor "
				 "so a factor of 0.0 means that the resulting sound is exactly the "
				 "source and a factor of 1.0 means that the resulting sound matches "
				 "exactly the target. By default this control is synchronized to Sinusoidal"
				 "Amplitude and Frequency and Residual Amplitude envelopes.");
		widget->hide();
		widget->InitPoints( 0.5 );
		add( widget );

		widget->PointsChanged.Connect( GlobalEnvelopeEdited );
		
		return widget;
	}

	Fl_Widget* Fl_SMS_Morph_Control::BuildSinAmpEditor()
	{
		Fl_SMS_BPF_Sync_Editor* widget = new Fl_SMS_BPF_Sync_Editor( 0, 0, 100, 100 );
		widget->label( mSinAmpControlKey.c_str() );
		widget->SetHorizontalRange( 0.0, 1.0 );
		widget->SetVerticalRange( 0.0, 1.0 );
		widget->SetGridWidth( 0.1, 0.1 );
		widget->tooltip( "Allows to control an aspect of the timbre blending performed," 
			"concretely the amplitude of the sinusoidal component, that is the amplitude" 
			"of the spectral peaks of the resulting sound expressed as a interpolation"
			" factor between the peaks of the two original sounds. The X axis is time, "
			"expressed in a parametric form,  and the Y axis is the interpolation factor "
			"between the two sinusoidal amplitude envelopes. An interpolation factor of "
			"0.0 means that the resulting sound sinusoidal envelope will match the "
			"source's one, and a factor of 1.0 means that the resulting sound sinusoidal" 
			"envelope matches exactly the target's one." );

		widget->hide();
		widget->InitPoints( 0.5 );
		add( widget );
		widget->PointsChanged.Connect( SinAmpEnvelopeEdited );
		widget->SynchronizationChanged.Connect( ChangeSinAmpSynchState );		

		return widget;
	}

	Fl_Widget* Fl_SMS_Morph_Control::BuildSinFreqEditor()
	{
		Fl_SMS_BPF_Sync_Editor* widget = new Fl_SMS_BPF_Sync_Editor( 0, 0, 100, 100 );
		widget->label( mSinFreqControlKey.c_str() );
		widget->SetHorizontalRange( 0.0, 1.0 );
		widget->SetVerticalRange( 0.0, 1.0 );
		widget->SetGridWidth( 0.1, 0.1 );
		widget->tooltip( " This envelope allows to control an aspect of the timbre "
			"blending performed, concretely the frequency of the sinusoidal component,"
			"that is the frequency of the spectral peaks of the resulting sound "
			"expressed as a interpolation factor between the peaks of the two original "
			"sounds. The X axis is time, expressed in a parametric form,  and the Y "
			"axis is the interpolation factor between the two frequencies of peaks "
			" in the sinusoidal component. An interpolation factor of 0.0 means that "
			" the resulting sound sinusoidal frequencies will match the source's one, "
			"and a factor of 1.0 means that the resulting sound sinusoidal envelope "
			"matches exactly the target's one. Note that this control has rather weak "
			"effect on harmonic sounds analyzed as such. In that case the effect will "
			"be driven by the value in the pitch hybridation factor except in frames "
			"where no pitch has been found, it is then recommended that you synchronize "
			"this factor to pitch." );

		widget->hide();
		widget->InitPoints( 0.5 );
		add( widget );

		widget->PointsChanged.Connect( SinFreqEnvelopeEdited );
		widget->SynchronizationChanged.Connect( ChangeSinFreqSynchState );		

		return widget;
	}

	Fl_Widget* Fl_SMS_Morph_Control::BuildPitchHybEditor()
	{
		Fl_SMS_BPF_Sync_Editor* widget = new Fl_SMS_BPF_Sync_Editor( 0, 0, 100, 100 );
		widget->label( mPitchHybControlKey.c_str() );
		widget->SetHorizontalRange( 0.0, 1.0 );
		widget->SetVerticalRange( 0.0, 1.0 );
		widget->SetGridWidth( 0.1, 0.1 );
		widget->tooltip( 
				"Allows to control sounds' pitches blending. Note that this involves "
				"not only to blend the fundamental frequency, but also any accompanying "
				"harmonics detected as tready pointed out in the previous control. The "
				"X axis is time, expressed in a parametric form, and the Y axis is the "
				"interpolation factor between the two sounds pitches. An interpolation "
				"factor of 0.0 means that the resulting sound harmonics will match the "
				"source's one, and a factor of 1.0 means that the resulting sound "
				"harmonics will match exactly the target's one in their frequency "
				"distributions.");

		widget->hide();
		widget->InitPoints( 0.5 );
		add( widget );
		
		widget->PointsChanged.Connect( PitchHybEnvelopeEdited );
		widget->SynchronizationChanged.Connect( ChangePitchHybSynchState );		

		return widget;
	}

	Fl_Widget* Fl_SMS_Morph_Control::BuildResAmpEditor()
	{
		Fl_SMS_BPF_Sync_Editor* widget = new Fl_SMS_BPF_Sync_Editor( 0, 0, 100, 100 );
		widget->label( mResAmpControlKey.c_str() );
		widget->SetHorizontalRange( 0.0, 1.0 );
		widget->SetVerticalRange( 0.0, 1.0 );
		widget->SetGridWidth( 0.1, 0.1 );
		widget->tooltip( 
				"This widget allows you to control the blending of the two sounds "
				"residual component amplitude envelope. The X axis represents "
				"transformation time, in a parametric form, and the Y axis represents "
				"the interpolation factor between the two sounds hoarseness. So a "
				"interpolation factor of 0.0 means that the resulting sound noisy "
				"component will match source's one, and an interpolation factor of 1.0 "
				"means that the resulting sound residual will  match target's one.");

		widget->hide();
		widget->InitPoints( 0.5 );

		widget->PointsChanged.Connect( ResAmpEnvelopeEdited );
		widget->SynchronizationChanged.Connect( ChangeResAmpSynchState );		

		add( widget );

		return widget;
	}

	Fl_Widget* Fl_SMS_Morph_Control::BuildTimeSyncEditor()
	{
		Fl_SMS_BPF_Editor* widget = new Fl_SMS_BPF_Editor( 0, 0, 100, 100 );
		widget->label( mTimeSyncControlKey.c_str() );
		widget->SetHorizontalRange( 0.0, 1.0 );
		widget->SetVerticalRange( 0.0, 1.0 );
		widget->SetGridWidth( 0.1, 0.1 );
		widget->tooltip( 
				"It defines a temporal mapping between the source and the target sound." 
				"X axis is the normalized time for the source sound and Y is the "
				"normalized time for the target. Using this control you can achieve"
				"morphs on only some parts of the sound, define how you want to stretch "
				"the shortest sound or even reverse one of the sounds in respect to the "
				"other.");

		widget->hide();
		
		CLAM::BPF tmpBPF;
		tmpBPF.Insert( 0.0, 0.0 );
		tmpBPF.Insert( 1.0, 1.0 );

		widget->InitPoints( tmpBPF );

		widget->PointsChanged.Connect( TimeSyncEnvelopeEdited );

		add( widget );

		return widget;
	}

	Fl_Widget* Fl_SMS_Morph_Control::BuildSinShapeW1Editor()
	{
		Fl_SMS_BPF_Editor* widget = new Fl_SMS_BPF_Editor( 0, 0, 100, 100 );
		widget->label( mSinShapeW1ControlKey.c_str() );
		widget->SetHorizontalRange( 0.0, 1.0 );
		widget->SetVerticalRange( 0.0, 1.0 );
		widget->SetGridWidth( 0.1, 0.1 );
		widget->tooltip( 
				"SinusoidalShapeW1, SinusoidalShapeW2 and SinusoidalShapeInterpolationFactor "
				"form a non separable set. The first two define spectral envelopes where, "
				"and for every frequency, a Y value of 0 means that the amplitude of "
				"that frequency will be taken from the source spectrum and a value of 1 "
				"means that the amplitude should be that of the target spectrum. The "
				"third control is a temporal envelope that defines how much of each of "
				"the previous spectral shapes should be taken in every instant. An "
				"instantaneous value of 0 in this temporal envelope means that "
				"SinusoidalShapeW1 should be used and a value of 1 means that the "
				"SinusoidalShapeW2 should be used.");

		widget->hide();
		
		CLAM::BPF tmpBPF;
		tmpBPF.Insert( 0.0, 0.5 );
		tmpBPF.Insert( 1.0, 0.5 );

		widget->InitPoints( tmpBPF );

		widget->PointsChanged.Connect( SinShapeW1EnvelopeEdited );

		add( widget );

		return widget;
	}

	Fl_Widget* Fl_SMS_Morph_Control::BuildSinShapeW2Editor()
	{
		Fl_SMS_BPF_Editor* widget = new Fl_SMS_BPF_Editor( 0, 0, 100, 100 );
		widget->label( mSinShapeW1ControlKey.c_str() );
		widget->SetHorizontalRange( 0.0, 1.0 );
		widget->SetVerticalRange( 0.0, 1.0 );
		widget->SetGridWidth( 0.1, 0.1 );
		widget->tooltip( 
				"SinusoidalShapeW1, SinusoidalShapeW2 and SinusoidalShapeInterpolationFactor "
				"form a non separable set. The first two define spectral envelopes where, "
				"and for every frequency, a Y value of 0 means that the amplitude of "
				"that frequency will be taken from the source spectrum and a value of 1 "
				"means that the amplitude should be that of the target spectrum. The "
				"third control is a temporal envelope that defines how much of each of "
				"the previous spectral shapes should be taken in every instant. An "
				"instantaneous value of 0 in this temporal envelope means that "
				"SinusoidalShapeW1 should be used and a value of 1 means that the "
				"SinusoidalShapeW2 should be used.");

		widget->hide();
		
		CLAM::BPF tmpBPF;
		tmpBPF.Insert( 0.0, 0.5 );
		tmpBPF.Insert( 1.0, 0.5 );

		widget->InitPoints( tmpBPF );

		widget->PointsChanged.Connect( SinShapeW2EnvelopeEdited );

		add( widget );

		return widget;
	}

	Fl_Widget* Fl_SMS_Morph_Control::BuildResShapeW1Editor()
	{
		Fl_SMS_BPF_Editor* widget = new Fl_SMS_BPF_Editor( 0, 0, 100, 100 );
		widget->label( mResShapeW1ControlKey.c_str() );
		widget->SetHorizontalRange( 0.0, 1.0 );
		widget->SetVerticalRange( 0.0, 1.0 );
		widget->SetGridWidth( 0.1, 0.1 );
		widget->tooltip( 
				 "This control is curently disabled");

		widget->hide();
		
		CLAM::BPF tmpBPF;
		tmpBPF.Insert( 0.0, 0.5 );
		tmpBPF.Insert( 1.0, 0.5 );

		widget->InitPoints( tmpBPF );

		widget->PointsChanged.Connect( ResShapeW1EnvelopeEdited );

		add( widget );

		return widget;
	}

	Fl_Widget* Fl_SMS_Morph_Control::BuildResShapeW2Editor()
	{
		Fl_SMS_BPF_Editor* widget = new Fl_SMS_BPF_Editor( 0, 0, 100, 100 );
		widget->label( mResShapeW2ControlKey.c_str() );
		widget->SetHorizontalRange( 0.0, 1.0 );
		widget->SetVerticalRange( 0.0, 1.0 );
		widget->SetGridWidth( 0.1, 0.1 );
		widget->tooltip( 
				 "This control is curently disabled");

		widget->hide();
		
		CLAM::BPF tmpBPF;
		tmpBPF.Insert( 0.0, 0.5 );
		tmpBPF.Insert( 1.0, 0.5 );

		widget->InitPoints( tmpBPF );

		widget->PointsChanged.Connect( ResShapeW2EnvelopeEdited );

		add( widget );

		return widget;
	}

	Fl_Widget* Fl_SMS_Morph_Control::BuildSinShapeEditor()
	{
		Fl_SMS_BPF_Editor* widget = new Fl_SMS_BPF_Editor( 0, 0, 100, 100 );
		widget->label( mSinShapeControlKey.c_str() );
		widget->SetHorizontalRange( 0.0, 1.0 );
		widget->SetVerticalRange( 0.0, 1.0 );
		widget->SetGridWidth( 0.1, 0.1 );
		widget->tooltip( 
				"SinusoidalShapeW1, SinusoidalShapeW2 and SinusoidalShapeInterpolationFactor "
				"form a non separable set. The first two define spectral envelopes where, "
				"and for every frequency, a Y value of 0 means that the amplitude of "
				"that frequency will be taken from the source spectrum and a value of 1 "
				"means that the amplitude should be that of the target spectrum. The "
				"third control is a temporal envelope that defines how much of each of "
				"the previous spectral shapes should be taken in every instant. An "
				"instantaneous value of 0 in this temporal envelope means that "
				"SinusoidalShapeW1 should be used and a value of 1 means that the "
				"SinusoidalShapeW2 should be used.");

		widget->hide();
		
		CLAM::BPF tmpBPF;
		tmpBPF.Insert( 0.0, 1.0 );
		tmpBPF.Insert( 1.0, 1.0 );

		widget->InitPoints( tmpBPF );

		widget->PointsChanged.Connect( SinShapeEnvelopeEdited );

		add( widget );

		return widget;
	}

	Fl_Widget* Fl_SMS_Morph_Control::BuildResShapeEditor()
	{
		Fl_SMS_BPF_Editor* widget = new Fl_SMS_BPF_Editor( 0, 0, 100, 100 );
		widget->label( mResShapeControlKey.c_str() );
		widget->SetHorizontalRange( 0.0, 1.0 );
		widget->SetVerticalRange( 0.0, 1.0 );
		widget->SetGridWidth( 0.1, 0.1 );
		widget->tooltip( 
				 "This control is curently disabled");

		widget->hide();
		
		CLAM::BPF tmpBPF;
		tmpBPF.Insert( 0.0, 1.0 );
		tmpBPF.Insert( 1.0, 1.0 );

		widget->InitPoints( tmpBPF );

		widget->PointsChanged.Connect( ResShapeEnvelopeEdited );

		add( widget );

		return widget;
	}



	void Fl_SMS_Morph_Control::ShowEnvelopeEditorFor( const char* name )
	{
		std::string requested = name;

		SetVisibleEnvelopeWidget( mEnvelopeEditors[ requested ] );
	}

	void Fl_SMS_Morph_Control::ShowEnvelopeEditorFor( const std::string& name )
	{
		SetVisibleEnvelopeWidget( mEnvelopeEditors[ name ] );
	}

	void Fl_SMS_Morph_Control::SetVisibleEnvelopeWidget( Fl_Widget* w )
	{
		CLAM_ASSERT( w!=NULL, "Envelope widget given was NULL!" );

		if ( mpEnvelopeContainer->children() )
		{
			CLAM_ASSERT( mpEnvelopeContainer->children() <= 1, 
				     "Too many children for the envelope widget container!" );

			Fl_Widget* childEnvelope = mpEnvelopeContainer->child(0);
			childEnvelope->hide();
			childEnvelope->label( mpEnvelopeContainer->label() );
			mpEnvelopeContainer->remove( childEnvelope );
			add( childEnvelope );
		}

		mpEnvelopeContainer->label( w->label() );
		w->label( NULL );
		w->resize( mpEnvelopeContainer->x() + 4, mpEnvelopeContainer->y() + 2 , 
			   mpEnvelopeContainer->w() - 8, mpEnvelopeContainer->h() - 4 );
		mpEnvelopeContainer->add( w );
		w->show();
		mpEnvelopeContainer->redraw();

	}

	int Fl_SMS_Morph_Control::handle( int event )
	{
		if ( FirstTimeShown() && event == FL_SHOW )
		{
			ShowEnvelopeEditorFor( mGlobalControlKey );
			mpEnvelopeSelector->value(0);
			ShownOnce();
		}

		return Fl_Group::handle( event );
	}

	void Fl_SMS_Morph_Control::sMenuItemSelectedCb( Fl_Choice* i, Fl_SMS_Morph_Control* obj )
	{
		obj->ShowEnvelopeEditorFor( i->text( i->value() ) );
	}

	void Fl_SMS_Morph_Control::sFrameInterpolationCb( Fl_Check_Button* b, Fl_SMS_Morph_Control* obj )
	{
		if ( b->value() == 0 )
			obj->FrameInterpolationDeactivated();
		else
			obj->FrameInterpolationActivated();
	}

	void Fl_SMS_Morph_Control::FrameInterpolationActivated()
	{
		FrameInterpolationChanged.Emit( true );
	}

	void Fl_SMS_Morph_Control::FrameInterpolationDeactivated()
	{
		FrameInterpolationChanged.Emit( false );
	}

	void Fl_SMS_Morph_Control::InitEnvelopeSelectorContents()
	{
		mpEnvelopeSelector->add( mGlobalControlKey.c_str(), NULL,
					 (Fl_Callback*)sMenuItemSelectedCb, this );
		mpEnvelopeSelector->add( mSinAmpControlKey.c_str(), NULL,
					 (Fl_Callback*)sMenuItemSelectedCb, this );
		mpEnvelopeSelector->add( mSinFreqControlKey.c_str(), NULL,
					 (Fl_Callback*)sMenuItemSelectedCb, this );
		mpEnvelopeSelector->add( mResAmpControlKey.c_str(), NULL, 
					 (Fl_Callback*)sMenuItemSelectedCb, this );
		mpEnvelopeSelector->add( mPitchHybControlKey.c_str(), NULL,
					 (Fl_Callback*)sMenuItemSelectedCb, this );
		mpEnvelopeSelector->add( mTimeSyncControlKey.c_str(), NULL,
					 (Fl_Callback*)sMenuItemSelectedCb, this );
		mpEnvelopeSelector->add( mSinShapeControlKey.c_str(), NULL,
					 (Fl_Callback*)sMenuItemSelectedCb, this );				 
		mpEnvelopeSelector->add( mSinShapeW1ControlKey.c_str(), NULL,
					 (Fl_Callback*)sMenuItemSelectedCb, this );
		mpEnvelopeSelector->add( mSinShapeW2ControlKey.c_str(), NULL,
					 (Fl_Callback*)sMenuItemSelectedCb, this );
		mpEnvelopeSelector->add( mResShapeControlKey.c_str(), NULL, 
					 (Fl_Callback*)sMenuItemSelectedCb, this );
		mpEnvelopeSelector->add( mResShapeW1ControlKey.c_str(), NULL,
					 (Fl_Callback*)sMenuItemSelectedCb, this );
		mpEnvelopeSelector->add( mResShapeW2ControlKey.c_str(), NULL,
					 (Fl_Callback*)sMenuItemSelectedCb, this );

	}
	
	void Fl_SMS_Morph_Control::resize( int X, int Y, int W, int H )
	{
		Fl_Group::resize( X, Y, W, H );
		
		mpFrameInterpSelector->resize( x()+5, y()+5, 80, 20 );
		mpEnvelopeSelector->resize( x()+5, y()+45, W - 10, 20 );
		mpEnvelopeContainer->resize( x()+10, y()+80, w()-20, h()-85 );
		
	}

	Fl_SMS_Morph_Control::~Fl_SMS_Morph_Control()
	{
		DestroyEnvelopeEditors();
	}
}
