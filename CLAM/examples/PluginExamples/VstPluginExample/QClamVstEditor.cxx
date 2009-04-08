#include <QtGui/QApplication>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QCloseEvent>
#include <QtGui/QResizeEvent>
#include <QtGui/QMoveEvent>
#include <QtUiTools/QUiLoader>
#include <QtCore/QBuffer>
#include <sstream>
#include "QClamVstEditor.hxx"
#include "VstNetworkExporter.hxx"

extern HINSTANCE hInstance;

std::string windowMessage2String(UINT message)
{
	#define WM2STR_TRANSLATE(message) case message: return #message;
	switch (message)
	{
		WM2STR_TRANSLATE(WM_NCCREATE);
		WM2STR_TRANSLATE(WM_NCCALCSIZE);
		WM2STR_TRANSLATE(WM_CREATE);
		WM2STR_TRANSLATE(WM_SIZE);
		WM2STR_TRANSLATE(WM_MOVE);
		WM2STR_TRANSLATE(WM_SHOWWINDOW);
		WM2STR_TRANSLATE(WM_WINDOWPOSCHANGING);
		WM2STR_TRANSLATE(WM_WINDOWPOSCHANGED);
		WM2STR_TRANSLATE(WM_STYLECHANGING);
		WM2STR_TRANSLATE(WM_STYLECHANGED);
		WM2STR_TRANSLATE(WM_MOUSEWHEEL);
		WM2STR_TRANSLATE(WM_CTLCOLOREDIT);
		WM2STR_TRANSLATE(WM_DESTROY);
		WM2STR_TRANSLATE(WM_NCDESTROY);
		WM2STR_TRANSLATE(WM_LBUTTONDOWN);
		WM2STR_TRANSLATE(WM_RBUTTONDOWN);
		WM2STR_TRANSLATE(WM_MBUTTONDOWN);
		WM2STR_TRANSLATE(WM_DRAWITEM);
		WM2STR_TRANSLATE(WM_MEASUREITEM);
		WM2STR_TRANSLATE(WM_PAINT);
		default:
			std::ostringstream os;
			os << message;
			return os.str();
	}
	#undef WM2STR_TRANSLATE
}

LONG_PTR WINAPI WindowProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::cout << "WindowProc: " << windowMessage2String(message) << std::endl;
	return DefWindowProc (hwnd, message, wParam, lParam);
}

const std::string & PluginEditorWindowClassName()
{
	static std::string windowClassName;
	if (windowClassName != "") return windowClassName;

	std::ostringstream os;
	os << "Plugin_" << hInstance << std::ends;
	windowClassName = os.str();
	WNDCLASS windowClass;
	windowClass.style = CS_GLOBALCLASS | CS_PARENTDC;//|CS_OWNDC; // add Private-DC constant
	windowClass.lpfnWndProc = WindowProc;
	windowClass.cbClsExtra  = 0;
	windowClass.cbWndExtra  = 0;
	windowClass.hInstance   = hInstance;
	windowClass.hIcon = 0;
	windowClass.hCursor = LoadCursor (NULL, IDC_ARROW);
	windowClass.hbrBackground = GetSysColorBrush (COLOR_BTNFACE);
	windowClass.lpszMenuName  = 0;
	windowClass.lpszClassName = windowClassName.c_str();
	RegisterClass (&windowClass);
	return windowClassName;
}

QVstWindow::QVstWindow(WId handle, QClamVstEditor * editor)
	: QWidget(0)
	, _editor(editor)
//	, _oldWndProc(0)
//	, _oldWndData(0)
	, _parent(handle)
{
	std::cout << "Widget Construction" << std::endl;
//	_oldWndProc = GetWindowLongPtr(handle, GWLP_WNDPROC);
//	_oldWndData = GetWindowLongPtr(handle, GWLP_USERDATA);

	RECT rect;
	GetWindowRect((WId)_parent, &rect);

	WId child = CreateWindowEx (
		0, PluginEditorWindowClassName().c_str(), "Window",
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 
		0, 0, rect.right-rect.left, rect.bottom-rect.top, 
		_parent, 0, hInstance, NULL);

//	SetWindowLongPtr ((HWND)child, GWLP_USERDATA, (LONG_PTR)this);
	std::cout << "Widget Creating" << std::endl;
	create(child, true, true); 
	std::cout << "Widget Created" << std::endl;
}


void QVstWindow::closeEvent(QCloseEvent * event)
{
	std::cout << "Widget Closing" << std::endl;
	QWidget::closeEvent(event);
	std::cout << "QWidget Closed" << std::endl;
	if (not _editor->isOpen())
		std::cout << "Widget Was not open" << std::endl;
	else _editor->close();
	std::cout << "Widget closed" << std::endl;
}

std::string rectString(WId nativeWindow)
{
	std::ostringstream os;
	RECT rect;
	GetWindowRect(nativeWindow, &rect);
	os
		<< rect.left << ","
		<< rect.top << ":"
		<< rect.right-rect.left << ","
		<< rect.bottom-rect.top;
	return os.str();
}

std::ostream & operator << (std::ostream & os, ERect & rect)
{
	return os
		<< rect.left << ","
		<< rect.top << ":"
		<< rect.right-rect.left << ","
		<< rect.bottom-rect.top;
}

std::ostream & operator<< (std::ostream & os, const QSize & size)
{
	return os
		<< size.width() << ","
		<< size.height();
}

bool resizeFrame(WId parent, int width, int height)
{
	long frameSize = (2 * GetSystemMetrics (SM_CYFIXEDFRAME));

	long diffWidth  = 0;
	long diffHeight = 0;

	HWND window = (HWND)parent;

	while ((diffWidth != frameSize) && (window)) // look for FrameWindow
	{
		std::string mdiClientClass = "MDIClient";
		HWND hTempParentWnd = GetParent (window);
		char buffer[1024];
		GetClassName (hTempParentWnd, buffer, 1024);
		if (!hTempParentWnd) break;
		if (buffer != mdiClientClass) break;
		RECT currentRect;
		GetWindowRect (window, &currentRect);
		RECT parentRect;
		GetWindowRect (hTempParentWnd, &parentRect);

		SetWindowPos (window, HWND_TOP, 0, 0, width + diffWidth, height + diffHeight, SWP_NOMOVE);

		diffWidth  += (parentRect.right - parentRect.left) - (currentRect.right - currentRect.left);
		diffHeight += (parentRect.bottom - parentRect.top) - (currentRect.bottom - currentRect.top);

		if ((diffWidth > 80) || (diffHeight > 80)) // parent belongs to host
		{
			std::cout << "Parent belongs to host" << std::endl;
			return true;
		}

		if (diffWidth < 0)
			diffWidth = 0;
		if (diffHeight < 0)
			diffHeight = 0;

		window = hTempParentWnd;
	}

	if (window)
		SetWindowPos (window, HWND_TOP, 0, 0, width + diffWidth, height + diffHeight, SWP_NOMOVE);


}

void QVstWindow::resizeEvent(QResizeEvent * event)
{
	std::cout << "Widget Resizing "  << event->size() << std::endl;
	std::cout << " Parent size " << rectString(_parent) << std::endl; 
	QWidget::resizeEvent(event);
	ERect * rect;
	_editor->getRect(&rect);
	resizeFrame(winId(), event->size().width(), event->size().height());
	std::cout << " Rect before " << *rect << std::endl; 
	rect->right =rect->left+width();
	rect->bottom = rect->top +height();
	
	std::cout << " Rect after " << *rect << " adapted" << std::endl; 
	std::cout << "Widget Resized " << size() << std::endl;
}

void QVstWindow::moveEvent(QMoveEvent * event)
{
	std::cout << "Widget Moving " 
		<< event->pos().x() << "," 
		<< event->pos().y() << std::endl;
	QWidget::moveEvent(event);
}

QVstWindow::~QVstWindow()
{
	std::cout << "Widget Destroying" << std::endl;
	_editor->detachWidget();
	std::cout << "Widget Detaching" << std::endl;
	create(0, false, true); // Detach the window without destroying it
	// Restoring previous data
//	SetWindowLongPtr(winId(), GWLP_WNDPROC, _oldWndProc);
//	SetWindowLongPtr(winId(), GWLP_USERDATA, _oldWndData);
	std::cout << "Widget Destroyed" << std::endl;
}

void requireQApp()
{
	std::cout << "Assuring Application" << std::endl;
	if (qApp) return;
	std::cout << "Application init" << std::endl;
	int argc=0;
	char * argv[] = {0};
	new QApplication(argc, argv);
}

ERect defaultRect = {0,0,100,400};

QClamVstEditor::QClamVstEditor(CLAM::VstNetworkExporter * effect, const std::string & uifile)
	: AEffEditor(effect)
	, _uifile(uifile)
	, _widget(0)
	, _app(0)
	, _rect(defaultRect)
{
	std::cout << "Creating editor" << std::endl;
	requireQApp();
	effect->setEditor (this);
}

void QClamVstEditor::idle ()
{
//	std::cout << (systemWindow?".":"0") << std::flush;
//	requireQApp();
	if (qApp ) QApplication::processEvents();
	else std::cout << "T" << std::flush;
}

bool QClamVstEditor::open(void * ptr)
{
	std::cout << "Opening Editor " << ptr << std::endl;
	AEffEditor::open(ptr);
	requireQApp();
	_widget = new QVstWindow((WId)ptr, this);
	std::cout << "Widget Showing" << std::endl;
	std::cout << "Widget Shown" << std::endl;
	std::cout << "Setting the layout" << std::endl;
	_widget->show();
	QHBoxLayout * layout = new QHBoxLayout(_widget);
	std::cout << "Adding the widget" << std::endl;
	QUiLoader loader;
	QBuffer file;
	file.setData(_uifile.c_str(), _uifile.size());
	QWidget * ui = loader.load(&file, _widget);
	if (not ui) std::cerr << "Error loading interface" << std::endl;
	else layout->addWidget(ui);
//	_widget->setStyleSheet("background-color: blue; border: red solid 2pt;");
//	ui->setStyleSheet("background-color: white; padding: 3pt; border: yellow solid 2pt;");
	ui->adjustSize();
	return true;
}

void QClamVstEditor::close ()
{
	std::cout << "Closing Editor" << std::endl;
	AEffEditor::close();
	std::cout << "AEffEditor::close called" << std::endl;
	if (_widget) delete _widget;
	std::cout << "Closed Editor" << std::endl;
}

QClamVstEditor::~QClamVstEditor()
{
	std::cout << "Destroying Editor" << std::endl;
	if (_widget) delete _widget;
}


void QClamVstEditor::setParameter (VstInt32 index, float value)
{
	std::cout << "Edit: Setting Parameter " << index << " to " << value << std::endl;
	// TODO: Propagate the parameter to the widgets
}

bool QClamVstEditor::getRect (ERect **ppErect)
{
	if (not systemWindow)
	{
		_rect = defaultRect;
		return true;
	}
	WId sizeReference = _widget? _widget->winId() : (WId) systemWindow;
	RECT rect;
	GetWindowRect(sizeReference, &rect);
	_rect.right = rect.right;
	_rect.left = rect.left;
	_rect.top = rect.top;
	_rect.bottom = rect.bottom;
	*ppErect = &_rect;
	return true;
}

