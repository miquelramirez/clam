#ifndef __QtEnvelopeEditor_hxx__
#define __QtEnvelopeEditor_hxx__

#include <qwidget.h>
#include <vector>
#include "BPF.hxx"
#include "QtEnvelopePointEditor.hxx"
#include "QtEnvelope.hxx"

// Provisional
#include "AudioManager.hxx"

namespace CLAM 
{

class QtEnvelopeEditor : public QWidget {

Q_OBJECT

public:

	QtEnvelopeEditor(QWidget * parent);

	void SetValue(const BPF & bpf);

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

	BPF GetValue();

	
	~QtEnvelopeEditor();

private:
	QtEnvelopePointEditor * mEmbededEditor;
	QtEnvelope mEnvelope;
	bool mMaxXBounded;
	bool mMaxYBounded;
	bool mMinYBounded;
	bool mMinXBounded;
	TData mMinX, mMaxX;
	TData mMinY, mMaxY;

};

} // namespace CLAM

#endif // __QtEnvelopeEditor_hxx__

