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

#include <QtGui/QLabel>
#include <QtGui/QFrame>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include "vmColorScale.hxx"
#include "vmSpecgramPanel.hxx"

namespace CLAM
{
	namespace VM
	{
		SpecgramPanel::SpecgramPanel(QWidget* parent)
			: QWidget(parent)
		{
			InitSpecgramPanel();
		}

		SpecgramPanel::~SpecgramPanel()
		{
		}

		void SpecgramPanel::SetColorScale(const std::vector<QColor>& scale)
		{
			mColorScale->SetScale(scale);
		}

		void SpecgramPanel::setFixedLabels(QString specRange, QString totalSlices)
		{
			mSpectralRange->setText("Spectral Range (Hz): "+specRange);
			mTotalSlices->setText("Slices: "+totalSlices);
		}

		void SpecgramPanel::updateLabels(QString freq, QString db, QString slice, QString time)
		{
			mFrequency->setText(freq);
			mDecibels->setText(db);
			mSlice->setText(slice);
			(time=="--") ? mTime->setText("  t: "+time) : mTime->setText("  t: "+time+" s.");
		}

		void SpecgramPanel::InitSpecgramPanel()
		{
			QBoxLayout* layout = new QHBoxLayout(this);
			layout->setMargin(0);
			layout->setSpacing(2);
			layout->addWidget(GetLeftSide());
			layout->addWidget(GetRightSide());

			connect(mColorScale,SIGNAL(widthChanged(int)),this,SIGNAL(colorScaleWidthChanged(int)));
		}

		QWidget* SpecgramPanel::GetLeftSide()
		{
			QFrame* leftGroup = new QFrame(this);
			leftGroup->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			QFontMetrics fm(font());
			int width = fm.width("Frequency (Hz): 00000")+6;
			leftGroup->setFixedSize(width,66);

			QVBoxLayout* lfMainLayout = new QVBoxLayout(leftGroup);
			lfMainLayout->setMargin(3);
			lfMainLayout->setSpacing(0);
			QHBoxLayout* lfInnerTopLayout = new QHBoxLayout;
			QHBoxLayout* lfInnerMiddleLayout = new QHBoxLayout;
			QHBoxLayout* lfInnerBottomLayout = new QHBoxLayout;
			lfInnerTopLayout->setMargin(0);
			lfInnerMiddleLayout->setMargin(0);
			lfInnerBottomLayout->setMargin(0);
			lfInnerTopLayout->setSpacing(0);
			lfInnerMiddleLayout->setSpacing(0);
			lfInnerBottomLayout->setSpacing(0);

			QLabel* freqTxtLabel = new QLabel(leftGroup);
			freqTxtLabel->setText("Frequency (Hz): ");
			mFrequency = new QLabel(leftGroup);
			mFrequency->setFixedWidth(leftGroup->width()-freqTxtLabel->width()-6);
			mFrequency->setAlignment(Qt::AlignRight);
			mFrequency->setText("--");
			lfInnerTopLayout->addWidget(freqTxtLabel);
			lfInnerTopLayout->addWidget(mFrequency);

			QLabel* decibelTxtLabel = new QLabel(leftGroup);
			decibelTxtLabel->setText("Decibels (dB): ");
			mDecibels = new QLabel(leftGroup);
			mDecibels->setFixedWidth(leftGroup->width()-decibelTxtLabel->width()-6);
			mDecibels->setAlignment(Qt::AlignRight);
			mDecibels->setText("--");
			lfInnerMiddleLayout->addWidget(decibelTxtLabel);
			lfInnerMiddleLayout->addWidget(mDecibels);

			QLabel* sliceTxtLabel = new QLabel(leftGroup);
			sliceTxtLabel->setText("Slice: ");
			mSlice = new QLabel(leftGroup);
			mSlice->setFixedWidth(leftGroup->width()-sliceTxtLabel->width()-6);
			mSlice->setAlignment(Qt::AlignRight);
			mSlice->setText("--");
			lfInnerBottomLayout->addWidget(sliceTxtLabel);
			lfInnerBottomLayout->addWidget(mSlice);

			lfMainLayout->addLayout(lfInnerTopLayout);
			lfMainLayout->addLayout(lfInnerMiddleLayout);
			lfMainLayout->addLayout(lfInnerBottomLayout);

			return leftGroup;
		}

		QWidget* SpecgramPanel::GetRightSide()
		{
			QFrame* rightGroup = new QFrame(this);
			rightGroup->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			rightGroup->setFixedHeight(66);

			QVBoxLayout* rfMainLayout = new QVBoxLayout(rightGroup);
			rfMainLayout->setMargin(3);
			rfMainLayout->setSpacing(0);
			QHBoxLayout* rfInnerTopLayout = new QHBoxLayout;
			rfInnerTopLayout->setMargin(0);
			rfInnerTopLayout->setSpacing(5);
			QHBoxLayout* rfInnerMiddleLayout = new QHBoxLayout;
			rfInnerMiddleLayout->setMargin(0);
			rfInnerMiddleLayout->setSpacing(0);
			QHBoxLayout* rfInnerBottomLayout = new QHBoxLayout;
			rfInnerBottomLayout->setMargin(0);
			rfInnerBottomLayout->setSpacing(3);

			mTotalSlices = new QLabel(rightGroup);
			mTime = new QLabel(rightGroup);
			rfInnerTopLayout->addWidget(mTotalSlices);
			rfInnerTopLayout->addWidget(mTime);
			rfInnerTopLayout->addStretch();

			mSpectralRange = new QLabel(rightGroup);
			rfInnerMiddleLayout->addWidget(mSpectralRange);
			rfInnerMiddleLayout->addStretch();

			QLabel* leftTag = new QLabel(rightGroup);
			leftTag->setText("0 dB");
			QLabel* rightTag = new QLabel(rightGroup);
			rightTag->setText("-150 dB");
			mColorScale = new ColorScale(rightGroup);
			rfInnerBottomLayout->addWidget(leftTag);
			rfInnerBottomLayout->addWidget(mColorScale,1);
			rfInnerBottomLayout->addWidget(rightTag);
			rfInnerBottomLayout->addStretch();

			mTime->setText("  t: --");
			mTotalSlices->setText("Slices: --");
			mSpectralRange->setText("Spectral Range (Hz): --");

			rfMainLayout->addLayout(rfInnerTopLayout);
			rfMainLayout->addLayout(rfInnerMiddleLayout);
			rfMainLayout->addLayout(rfInnerBottomLayout);
	    
			return rightGroup;
		}
	}
}

// END

