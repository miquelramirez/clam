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

#ifndef __VMQT_DATA_TYPES_H__
#define __VMQT_DATA_TYPES_H__
#include <QtGui/QColor>

namespace CLAM
{
	namespace VM
	{
		struct GLView
		{
			double left;
			double right;
			double bottom;
			double top;
			double mNear;
			double mFar;

			GLView();
			GLView(double leftv, double rightv, double bottomv, double topv, double nearv, double farv);
			GLView(const GLView& v);
			~GLView();

			bool operator==(const GLView& v);
		};

		struct GLViewport
		{
			int x;
			int y;
			int w;
			int h;

			GLViewport();
			GLViewport(int xv, int yv, int wv, int hv);
			GLViewport(const GLViewport& v);
			~GLViewport();

			const GLViewport& operator=(const GLViewport& v);
			bool operator==(const GLViewport& v);
			bool operator!=(const GLViewport& v);
		};

		struct Range
		{
			double min;
			double max;

			Range();
			Range(double minv, double maxv);
			Range(const Range& range);
			~Range();

			double Span();
		};

		enum ERulerScale
		{
			eLinearScale,
			eLogScale
		};

		enum ERulerPos
		{
			eLeft,
			eRight,
			eBottom,
			eTop
		};

		enum EOrientation
		{
			eHorizontal,
			eVertical
		};

		enum EBPFEditorFlags
		{
			eAllowInsertions = 0x01,
			eAllowDeletions  = 0x02,
			eAllowHorEdition = 0x04,
			eAllowVerEdition = 0x08
		};

		enum EPlayingFlags
		{
			eAudio         = 0x01,
			eUseOscillator = 0x02
		};

		struct Keyboard
		{
			bool key_ctrl;
			bool key_delete;
			bool key_down;
			bool key_h;
			bool key_insert;
			bool key_left;
			bool key_minus;
			bool key_next;
			bool key_plus;
			bool key_prior;
			bool key_r;
			bool key_right;
			bool key_shift;
			bool key_up;
			bool key_v;
		   
			Keyboard();
			~Keyboard();

			void Init();
		};

		struct Peak
		{
			double freq;
			double mag;

			Peak();
			Peak(double f, double m);
			Peak(const Peak& peak);
			~Peak();

			bool operator==(const Peak& peak) const;
			const Peak& operator=(const Peak& peak);
		};

		struct SinTrackNode
		{
			double          freq;
			unsigned        timeIndex;
			QColor color;

			SinTrackNode();
			SinTrackNode(double f, unsigned t, const QColor& c);
			SinTrackNode(const SinTrackNode& node);
			~SinTrackNode();

			SinTrackNode& operator=(const SinTrackNode& node);

		};
	}
}

#endif

