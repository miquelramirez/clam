#ifndef __QtEnvelopeEditor_hxx__
#define __QtEnvelopeEditor_hxx__

#include <qwidget.h>
#include <vector>
#include "BPF.hxx"
#include "envelope_point_editor.hxx"
#include "envelope.hxx"

// Provisional
#include "AudioManager.hxx"

namespace CLAM 
{

class QtEnvelopeEditor : public QWidget {

Q_OBJECT

public:

	QtEnvelopeEditor(QWidget * parent)
	{
		mEmbededEditor = new Envelope_Point_Editor( parent );
		mEmbededEditor->setMinimumSize( QSize( 350, 350) );
		mMaxXBounded = false;
		mMaxYBounded = false;
		mMinYBounded = false;
		mMinXBounded = false;
	}

	void SetValue(const BPF & bpf)
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

	void BoundMaxX(TData value)
	{
		mMaxXBounded = true;
		mMaxX = value;
	}

	void BoundMinX(TData value)
	{
		mMinXBounded = true;
		mMinX = value;
	}

	void BoundMaxY(TData value)
	{
		mMaxYBounded = true;
		mMaxY = value;
	}

	void BoundMinY(TData value)
	{
		mMinYBounded = true;
		mMinY = value;
	}

	void UnboundMaxX()
	{
		mMaxXBounded = false;
	}

	void UnboundMaxY()
	{
		mMaxYBounded = false;
	}

	void UnboundMinY()
	{
		mMinYBounded = false;
	}

	void UnboundMinX()
	{
		mMinXBounded = false;
	}

	BPF GetValue()
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

	
	~QtEnvelopeEditor() 
	{
	}
private:
	Envelope_Point_Editor * mEmbededEditor;
	Envelope mEnvelope;
	bool mMaxXBounded;
	bool mMaxYBounded;
	bool mMinYBounded;
	bool mMinXBounded;
	TData mMinX, mMaxX;
	TData mMinY, mMaxY;

};

} // namespace CLAM

#endif // __QtEnvelopeEditor_hxx__
