#ifndef QClamVstEditor_hxx
#define QClamVstEditor_hxx

#include <QtGui/QWidget>
#include <aeffeditor.h>
#include <iostream>
#include <windows.h>
namespace CLAM { class VstNetworkExporter; }
class QClamVstEditor;
class QCloseEvent;
class QResizeEvent;
class QMoveEvent;

class QVstWindow : public QWidget
{
	Q_OBJECT
public:
	QVstWindow(WId handle, QClamVstEditor * editor);
	~QVstWindow();
	void closeEvent(QCloseEvent * event);
	void resizeEvent(QResizeEvent * event);
	void moveEvent(QMoveEvent * event);
protected:
	QClamVstEditor * _editor;
	LONG_PTR _oldWndProc;
	LONG_PTR _oldWndData;
	WId _parent;
};

class QClamVstEditor : public AEffEditor
{
public:
	QClamVstEditor(CLAM::VstNetworkExporter * effect, const std::string & uifile);
	~QClamVstEditor();
	/// Open editor, pointer to parent windows is platform-dependent (HWND on Windows, WindowRef on Mac).
	virtual bool open(void * ptr);
	/// Idle call supplied by Host application
	virtual void idle ();
	/// Close editor (detach from parent window)
	virtual void close ();

	virtual bool onKeyDown (VstKeyCode& keyCode)	{
		std::cout << "onKeyDown " 
			<< keyCode.character << " " 
			<< keyCode.virt << " "
			<< keyCode.modifier << std::endl;
		return false; }		///< Receive key down event. Return true only if key was really used!
	virtual bool onKeyUp (VstKeyCode& keyCode)		{
		std::cout << "onKeyDown " 
			<< keyCode.character << " " 
			<< keyCode.virt << " "
			<< keyCode.modifier << std::endl;
		return false; }		///< Receive key up event. Return true only if key was really used!
	virtual bool onWheel (float distance)			{
		std::cout << "onWheel " << distance << std::endl;
		return false; }		///< Handle mouse wheel event, distance is positive or negative to indicate wheel direction.
	virtual bool setKnobMode (VstInt32 val)			{
		std::cout << "setKnobMode " << val << std::endl;
		return false; }		///< Set knob mode (if supported by Host). See CKnobMode in VSTGUI.

	void setParameter (VstInt32 index, float value);
	bool getRect (ERect **ppErect);
	void detachWidget() { _widget = 0; } // To be called from widget destructor
protected:
	std::string _uifile;
	QWidget * _widget;
	QApplication * _app;
	ERect _rect;
};


#endif//QClamVstEditor_hxx
