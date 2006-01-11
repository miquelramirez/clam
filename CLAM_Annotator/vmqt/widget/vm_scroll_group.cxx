#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QString>
#include <QPainter>
#include <QScrollBar>
#include <QLabel>
#include <QPushButton>
#include "vm_scroll_group.hxx"

namespace CLAM
{
	namespace VM
	{

		class VerticalLabel : public QFrame
		{
		public:
			VerticalLabel(QWidget* parent);
			~VerticalLabel();

			void setText(const QString& text);

		protected:
			void paintEvent(QPaintEvent* e);

		private:
			QFont   vl_font;
			QString vl_text;
		};

		VerticalLabel::VerticalLabel(QWidget* parent)
			: QFrame(parent)
		{
			vl_font.setFamily("Fixed");
			vl_font.setPointSize(9);
			vl_font.setBold(true);

			this->setMinimumSize(20,25);
			this->setFixedWidth(20);
			this->setMaximumHeight(70);
			this->setToolTip("Zoom Ratio");
			this->setText("1:1");
		}

		VerticalLabel::~VerticalLabel()
		{
		}

		void VerticalLabel::setText(const QString& text)
		{
			vl_text = text;
			this->setFixedHeight(vl_text.length()*vl_font.pointSize());
			update();
		}

		void VerticalLabel::paintEvent(QPaintEvent* e)
		{
			QFrame::paintEvent(e);
			QPainter p(this);
			QRect r = rect();
			p.setFont(vl_font);
			p.translate(r.x(),r.y()+r.height());
			p.rotate(-90.0);
			p.drawText(0,0,r.height(),r.width(),Qt::AlignHCenter|Qt::AlignTop,vl_text);
			p.resetMatrix();
		}

		ScrollGroup::ScrollGroup(CLAM::VM::EOrientation ori, QWidget* parent)
			: QWidget(parent)
			  , sg_hlabel(0)
			  , sg_vlabel(0)
		{
			setFocusPolicy(Qt::NoFocus);
			(ori==CLAM::VM::eHorizontal) ? build_horizontal() : build_vertical();
		}

		ScrollGroup::~ScrollGroup()
		{
		}

		void ScrollGroup::set_pixmaps(const QPixmap& zin, const QPixmap& zout)
		{
			sg_zin_button->setText("");
			sg_zout_button->setText("");

			sg_zin_button->setIcon(QIcon(zin));
			sg_zout_button->setIcon(QIcon(zout));
		}

		int ScrollGroup::get_scroll_value() const
		{
			return sg_scroll_bar->value();
		}

		int ScrollGroup::get_max_scroll_value() const
		{
			return sg_scroll_bar->maximum();
		}

		void ScrollGroup::updateScrollValue(int value)
		{
			if(sg_scroll_bar->value() == value) return;		
			sg_scroll_bar->setValue(value);
		}

		void ScrollGroup::setMaxScrollValue(int value)
		{
			if(value < 0 || sg_scroll_bar->maximum() == value) return;
			sg_scroll_bar->setMaximum(value);		
		}

		void ScrollGroup::updateZoomRatio(QString str)
		{
			(sg_hlabel) ? sg_hlabel->setText(str) : sg_vlabel->setText(str);
		}

		void ScrollGroup::build_buttons()
		{	
			sg_zin_button = new QPushButton(this);
			sg_zin_button->setAutoRepeat(true);
			sg_zin_button->setFixedSize(20,20);
			sg_zin_button->setText("+");
			sg_zin_button->setToolTip("Zoom In");
		 
			sg_zout_button = new QPushButton(this);
			sg_zout_button->setAutoRepeat(true);
			sg_zout_button->setFixedSize(20,20);
			sg_zout_button->setText("-");
			sg_zout_button->setToolTip("Zoom Out");
			
			sg_zin_button->setFocusPolicy(Qt::NoFocus);
			sg_zout_button->setFocusPolicy(Qt::NoFocus);
		}

		void ScrollGroup::build_horizontal()
		{
			build_buttons();

			QFont f("Fixed",9);
			f.setBold(true);
	
			sg_scroll_bar= new QScrollBar(Qt::Horizontal,this);
			sg_scroll_bar->setFixedHeight(sg_scroll_bar->sizeHint().height());
			sg_scroll_bar->setRange(0,0);
			sg_scroll_bar->setSingleStep(20);
			sg_scroll_bar->setPageStep(100);
			sg_scroll_bar->setFocusPolicy(Qt::NoFocus);

			sg_hlabel = new QLabel(this);
			sg_hlabel->setMinimumSize(30,20);
			sg_hlabel->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
			sg_hlabel->setFont(f);
			sg_hlabel->setAlignment(Qt::AlignCenter);
			sg_hlabel->setToolTip("Zoom Ratio");
			sg_hlabel->setText("1:1");
			
			QHBoxLayout* layout = new QHBoxLayout(this);
			layout->setMargin(0);
			layout->setSpacing(0);
			layout->addWidget(sg_scroll_bar,1);
			layout->addWidget(sg_hlabel);
			layout->addWidget(sg_zin_button);
			layout->addWidget(sg_zout_button);

			make_connections();
		}

		void ScrollGroup::build_vertical()
		{
			build_buttons();

			QFont f("Fixed",10);
			f.setBold(true);

			sg_vlabel = new CLAM::VM::VerticalLabel(this);
   
			sg_scroll_bar = new QScrollBar(Qt::Vertical,this);
			sg_scroll_bar->setFixedWidth(sg_scroll_bar->sizeHint().width());
			sg_scroll_bar->setRange(0,0);
			sg_scroll_bar->setSingleStep(20);
			sg_scroll_bar->setPageStep(100);
			sg_scroll_bar->setFocusPolicy(Qt::NoFocus);

			QVBoxLayout* layout = new QVBoxLayout(this);
			layout->setMargin(0);
			layout->setSpacing(0);
			layout->addWidget(sg_zout_button);
			layout->addWidget(sg_zin_button);
			layout->addWidget(sg_vlabel);
			layout->addWidget(sg_scroll_bar,1);

			setFixedWidth(20);

			make_connections();
		}

		void ScrollGroup::make_connections()
		{
			connect(sg_zin_button,SIGNAL(clicked()),this,SIGNAL(zoomIn()));
			connect(sg_zout_button,SIGNAL(clicked()),this,SIGNAL(zoomOut()));
			connect(sg_scroll_bar,SIGNAL(valueChanged(int)),this,SIGNAL(scrollValueChanged(int)));
		}
	}
}

// END
