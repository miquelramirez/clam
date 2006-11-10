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

#ifndef ChordRanking_hxx
#define ChordRanking_hxx

#include <QtOpenGL/QGLWidget>
#undef GetClassName
#include "FloatArrayDataSource.hxx"
#include <CLAM/PortMonitor.hxx>

#include <vector>

class ChordRankingMonitor : public CLAM::PortMonitor<std::vector<CLAM::TData> >, public CLAM::VM::FloatArrayDataSource
{
public:
	ChordRankingMonitor()
		: _size(0)
	{
	}
private:
	const char* GetClassName() const { return "ChordRanking"; };
	const std::string & getLabel(unsigned bin) const
	{
		static std::vector<std::string> a = initBinLabelVector();
		if (bin>a.size()) throw "Bad bin";
		return a[bin];
	}
	std::vector<std::string> initBinLabelVector() const
	{
		static char * roots[] = {
			"G",
			"G#",
			"A",
			"A#",
			"B",
			"C",
			"C#",
			"D",
			"D#",
			"E",
			"F",
			"F#",
			};
		static struct Modes {
			const char * name;
			unsigned nRoots;
		} modes[] =
		{
			{"None",        1},
			{"Major",      12},
			{"Minor",      12},
			{"Major7",     12}, // Cmaj7
			{"Dominant7",  12}, // C7
			{"MinorMajor7",12}, // Cm/maj7
			{"Minor7",     12}, // Cm7
//			{"Suspended2", 12}, // Csus2
//			{"Suspended4", 12}, // Csus4
//			{"Major6",     12}, // C6
//			{"Minor6",     12}, // Cm6
//			{"6/9",        12}, // C6/9
			{"Augmented",   4}, // Caug
			{"Diminished", 12}, // Cdim
			{"Diminished7",12}, // Cdim7
//			{"Fifth",      12}, // C5
			{0, 0}
		};
		std::vector<std::string> chordNames;
		for (unsigned i = 0; modes[i].name; i++)
		{
			std::string mode=modes[i].name;
			if (modes[i].nRoots == 1)
			{
				chordNames.push_back(mode);
				continue;
			}
			for (unsigned root=0; root<modes[i].nRoots; root++)
			{
				std::string rootName(roots[root]);
				chordNames.push_back(rootName+mode);
			}
		}
		return chordNames;
	}
	const CLAM::TData * frameData()
	{
		const std::vector<CLAM::TData> & chords = FreezeAndGetData();
		_size = chords.size();
		return &chords[0];
	}
	void release()
	{
		UnfreezeData();
	}
	unsigned nBins() const
	{
		return _size;
	}
	bool isEnabled() const
	{
		return GetExecState() == CLAM::Processing::Running;
	}
private:
	unsigned _size;
};


#include <QtDesigner/QDesignerExportWidget>

namespace CLAM
{
namespace VM
{

	class QDESIGNER_WIDGET_EXPORT ChordRanking : public QWidget
	{
		Q_OBJECT

		public:
			ChordRanking(QWidget * parent);
			~ChordRanking();
		public:
			virtual void paintEvent(QPaintEvent * event);
			virtual void timerEvent(QTimerEvent *event);
			void updateIfNeeded();
			void setDataSource( FloatArrayDataSource & dataSource );
			void clearData();
		protected:
			int _updatePending;
			double _maxValue;
			unsigned _nBins;
			FloatArrayDataSource * _dataSource;
			const CLAM::TData * _data;
			QFont _font;
	};


}
}



#endif// ChordRanking_hxx
