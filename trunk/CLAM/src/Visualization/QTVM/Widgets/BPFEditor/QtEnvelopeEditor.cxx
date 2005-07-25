#include "QtEnvelopeEditor.hxx"

namespace CLAM
{
	QtEnvelopeEditor::QtEnvelopeEditor( QWidget* parent )
	{
		mEmbededEditor = new Envelope_Point_Editor( parent );
		mEmbededEditor->setMinimumSize( QSize( 350, 350) );
		mMaxXBounded = false;
		mMaxYBounded = false;
		mMinYBounded = false;
		mMinXBounded = false;
	}

	QtEnvelopeEditor::~QtEnvelopeEditor()
	{
	}
	
	void QtEnvelopeEditor::SetValue( const BPF& bpf )
	{
		if (mMaxYBounded) mEnvelope.set_maxY_value( mMaxY );
		if (mMinYBounded) mEnvelope.set_minY_value( mMinY );
		if (mMaxXBounded) mEnvelope.set_maxX_value( mMaxX );
		if (mMinXBounded) mEnvelope.set_minX_value( mMinX );

		for ( int i=0; i<bpf.Size(); i++ )
		{
			// TODO change this to floats for CLAM
			mEnvelope.add_node_at_offset( bpf.GetXValue( i ), bpf.GetValueFromIndex( i ) );
		}

		mEmbededEditor->set_envelope( &mEnvelope );
	}

	BPF QtEnvelopeEditor::GetValue()
	{
		BPF value;
		Envelope* env = mEmbededEditor->get_envelope();
		value.SetSize(0);
		for ( int i=0; i< env->get_node_count(); i++ )
		{
			// TODO change this to floats for CLAM
			value.Insert( env->get_node_offset( i ), env->get_node_height( i ) );
		}
		return value;
	}

	
}
