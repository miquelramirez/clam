/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QFrame>
#include <QtCore/QString>
#include <QtGui/QPainter>
#include <QtGui/QScrollBar>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include "vmScrollGroup.hxx"
#include "vmIconData.hxx"

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
			QFont   mFont;
			QString mText;
		};

		VerticalLabel::VerticalLabel(QWidget* parent)
			: QFrame(parent)
		{
			mFont.setFamily("Fixed");
			mFont.setPointSize(9);
			mFont.setBold(true);

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
			mText = text;
			this->setFixedHeight(mText.length()*mFont.pointSize());
			update();
		}

		void VerticalLabel::paintEvent(QPaintEvent* e)
		{
			QFrame::paintEvent(e);
			QPainter p(this);
			QRect r = rect();
			p.setFont(mFont);
			p.translate(r.x(),r.y()+r.height());
			p.rotate(-90.0);
			p.drawText(0,0,r.height(),r.width(),Qt::AlignHCenter|Qt::AlignTop,mText);
			p.resetMatrix();
		}

		ScrollGroup::ScrollGroup(EOrientation ori, QWidget* parent)
			: QWidget(parent)
			  , mHlabel(0)
			  , mVlabel(0)
		{
			setFocusPolicy(Qt::NoFocus);
			(ori==CLAM::VM::eHorizontal) ? BuildHorizontal() : BuildVertical();
		}

		ScrollGroup::~ScrollGroup()
		{
		}

		void ScrollGroup::SetPixmaps(const QPixmap& zin, const QPixmap& zout)
		{
			mZinButton->setText("");
			mZoutButton->setText("");

			mZinButton->setIcon(QIcon(zin));
			mZoutButton->setIcon(QIcon(zout));
		}

		int ScrollGroup::GetScrollValue() const
		{
			return mScrollBar->value();
		}

		int ScrollGroup::GetMaxScrollValue() const
		{
			return mScrollBar->maximum();
		}

		void ScrollGroup::updateScrollValue(int value)
		{
			if(mScrollBar->value() == value) return;		
			mScrollBar->setValue(value);
		}

		void ScrollGroup::setMaxScrollValue(int value)
		{
			if(value < 0 || mScrollBar->maximum() == value) return;
			mScrollBar->setMaximum(value);		
		}

		void ScrollGroup::updateZoomRatio(QString str)
		{
			(mHlabel) ? mHlabel->setText(str) : mVlabel->setText(str);
		}

		void ScrollGroup::BuildButtons()
		{	
			mZinButton = new QPushButton(this);
			mZinButton->setAutoRepeat(true);
			mZinButton->setFixedSize(20,20);
//			mZinButton->setText("+");
			mZinButton->setToolTip("Zoom In");
			mZinButton->setIcon(QPixmap(icon_zoomin));
		 
			mZoutButton = new QPushButton(this);
			mZoutButton->setAutoRepeat(true);
			mZoutButton->setFixedSize(20,20);
//			mZoutButton->setText("-");
			mZoutButton->setToolTip("Zoom Out");
			mZoutButton->setIcon(QPixmap(icon_zoomout));
			
			mZinButton->setFocusPolicy(Qt::NoFocus);
			mZoutButton->setFocusPolicy(Qt::NoFocus);
		}

		void ScrollGroup::BuildHorizontal()
		{
			BuildButtons();

			QFont f("Fixed",9);
			f.setBold(true);
	
			mScrollBar= new QScrollBar(Qt::Horizontal,this);
			mScrollBar->setFixedHeight(mScrollBar->sizeHint().height());
			mScrollBar->setRange(0,0);
			mScrollBar->setSingleStep(20);
			mScrollBar->setPageStep(100);
			mScrollBar->setFocusPolicy(Qt::NoFocus);

			mHlabel = new QLabel(this);
			mHlabel->setMinimumSize(30,20);
			mHlabel->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
			mHlabel->setFont(f);
			mHlabel->setAlignment(Qt::AlignCenter);
			mHlabel->setToolTip("Zoom Ratio");
			mHlabel->setText("1:1");
			
			QHBoxLayout* layout = new QHBoxLayout(this);
			layout->setMargin(0);
			layout->setSpacing(0);
			layout->addWidget(mScrollBar,1);
			layout->addWidget(mHlabel);
			layout->addWidget(mZinButton);
			layout->addWidget(mZoutButton);

			setFixedHeight(20);
			
			MakeConnections();
		}

		void ScrollGroup::BuildVertical()
		{
			BuildButtons();

			QFont f("Fixed",10);
			f.setBold(true);

			mVlabel = new VerticalLabel(this);
   
			mScrollBar = new QScrollBar(Qt::Vertical,this);
			mScrollBar->setFixedWidth(mScrollBar->sizeHint().width());
			mScrollBar->setRange(0,0);
			mScrollBar->setSingleStep(20);
			mScrollBar->setPageStep(100);
			mScrollBar->setFocusPolicy(Qt::NoFocus);

			QVBoxLayout* layout = new QVBoxLayout(this);
			layout->setMargin(0);
			layout->setSpacing(0);
			layout->addWidget(mZoutButton);
			layout->addWidget(mZinButton);
			layout->addWidget(mVlabel);
			layout->addWidget(mScrollBar,1);

			setFixedWidth(20);

			MakeConnections();
		}

		void ScrollGroup::MakeConnections()
		{
			connect(mZinButton,SIGNAL(clicked()),this,SIGNAL(zoomIn()));
			connect(mZoutButton,SIGNAL(clicked()),this,SIGNAL(zoomOut()));
			connect(mScrollBar,SIGNAL(valueChanged(int)),this,SIGNAL(scrollValueChanged(int)));
		}
	}
}

// END

