#ifndef FrameDescriptorsPane_hxx
#define FrameDescriptorsPane_hxx

#include <QtGui/QSplitter>
#include <QtGui/QListWidget>
#include "vmBPFPlot.hxx"

class FrameDescriptorsPane : public QSplitter
{
	Q_OBJECT

	CLAM::VM::BPFPlot *mBPFEditor;
	QListWidget *mFrameLevelAttributeList;
 
public:
	FrameDescriptorsPane(QWidget * parent)
		: QSplitter(parent)
	{
		setOrientation(Qt::Horizontal);
		mBPFEditor = new CLAM::VM::BPFPlot(this);
		QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
		sizePolicy3.setHorizontalStretch(2);
		sizePolicy3.setVerticalStretch(0);
		sizePolicy3.setHeightForWidth(mBPFEditor->sizePolicy().hasHeightForWidth());
		mBPFEditor->setSizePolicy(sizePolicy3);
		mBPFEditor->setMinimumSize(QSize(700, 150));
		mBPFEditor->setFocusPolicy(Qt::WheelFocus);
		mBPFEditor->setFrameShape(QFrame::StyledPanel);
		mBPFEditor->setFrameShadow(QFrame::Raised);
		addWidget(mBPFEditor);
		mFrameLevelAttributeList = new QListWidget(this);
		mFrameLevelAttributeList->setMinimumSize(QSize(200, 0));
		mFrameLevelAttributeList->setAlternatingRowColors(true);
		mFrameLevelAttributeList->setResizeMode(QListView::Adjust);
		addWidget(mFrameLevelAttributeList);
// TODO: move elsewhere!
//		mVSplit->addWidget(this);

		mBPFEditor->setWhatsThis(
			tr("Annotator", "<p>The <b>frame level descriptors editor</b> allows editing\n"
			"			 floating point descriptors that has an occurrence for each frame.</p>\n"
			"			 <ul>\n"
			"			 <li>Dragging Points: Moving them.</li>\n"
			"			 <li>Page up/down: Set focus prev/next point</li>\n"
			"			 <li>Hover point: Set focus to point</li>\n"
			"			 <li>Dragging play positon: Moves it.</li>\n"
			"			 <li>'r'+Click: Set the current play position.</li>\n"
			"			 <li>Arrows: Change focused point values</li>\n"
			"			 </ul>")
		);

		mBPFEditor->SetFlags(CLAM::VM::eAllowVerEdition);//|CLAM::VM::eHasVerticalScroll); // QTPORT: What about this flag
		mBPFEditor->SetZoomSteps(5,5);
		mBPFEditor->SetXRange(0.0,2.0);

		mBPFEditor->setAutoFillBackground(true);
	}
	void adaptEnvelopesToCurrentSchema(CLAM::Annotator::Project & project)
	{
		mFrameLevelAttributeList->clear();

		const std::list<std::string>& names = project.GetNamesByScopeAndType("Frame", "Float");
		const unsigned nTabs = names.size();
		std::list<std::string>::const_iterator name = names.begin();
		for (unsigned i = 0; i<nTabs; name++, i++)
		{
			mFrameLevelAttributeList->addItem(name->c_str());
		}
	}
};

#endif
