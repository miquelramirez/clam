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

#include "vmDataTypes.hxx"
#include <QtGui/QColor>

namespace CLAM
{
	namespace VM
	{

///////// GLView /////////////////////
		GLView::GLView()
			: left(-1.0)
			, right(1.0)
			, bottom(-1.0)
			, top(1.0)
			, mNear(-1.0)
			, mFar(1.0)
		{
		}

		GLView::GLView(double leftv, double rightv, double bottomv, double topv, double nearv, double farv)
			: left(leftv)
			, right(rightv)
			, bottom(bottomv)
			, top(topv)
			, mNear(nearv)
			, mFar(farv)
		{
		}

		GLView::GLView(const GLView& v)
			: left(v.left)
			, right(v.right)
			, bottom(v.bottom)
			, top(v.top)
			, mNear(v.mNear)
			, mFar(v.mFar)
		{
		}

		GLView::~GLView()
		{
		}

		bool GLView::operator==(const GLView& v)
		{
			return (left==v.left && right==v.right && bottom==v.bottom && top==v.top && mNear==v.mNear && mFar==v.mFar);
		}

////////// GLViewport ///////////////
		GLViewport::GLViewport()
			: x(0)
			, y(0)
			, w(10)
			, h(10)
		{
		}

		GLViewport::GLViewport(int xv, int yv, int wv, int hv)
			: x(xv)
			, y(yv)
			, w(wv)
			, h(hv)
		{
		}

		GLViewport::GLViewport(const GLViewport& v)
			: x(v.x)
			, y(v.y)
			, w(v.w)
			, h(v.h)
		{
		}

		GLViewport::~GLViewport()
		{
		}

		const GLViewport& GLViewport::operator=(const GLViewport& v)
		{
			x = v.x;
			y = v.y;
			w = v.w;
			h = v.h;
			return *this;
		}

		bool GLViewport::operator==(const GLViewport& v)
		{
			return (x==v.x && y==v.y && w==v.w && h==v.h);
		}

		bool GLViewport::operator!=(const GLViewport& v)
		{
			return (x!=v.x || y!=v.y || w!=v.w || h!=v.h);
		}

////////////// Range /////////////////////
		Range::Range()
			: min(0.0)
			, max(1.0)
		{
		}

		Range::Range(double minv, double maxv)
			: min(minv)
			, max(maxv)
		{
		}

		Range::Range(const Range& range)
			: min(range.min)
			, max(range.max)
		{
		}

		Range::~Range()
		{
		}

		double Range::Span()
		{
			return (max-min);
		}

/////////// Keyboard //////////////////////
		Keyboard::Keyboard()
		{
			Init();
		}

		Keyboard::~Keyboard()
		{
		}

		void Keyboard::Init()
		{
			key_ctrl=key_delete=key_down=key_h=key_insert=key_left=key_minus=key_next=key_plus=key_prior=key_r=key_right=key_shift=key_up=key_v=false;
		}

/////////// Peak ////////////////////////////
		Peak::Peak()
			: freq(0.0)
			, mag(0.0)
		{
		}

		Peak::Peak(double f, double m)
			: freq(f)
			, mag(m)
		{
		}

		Peak::Peak(const Peak& peak)
			: freq(peak.freq)
			, mag(peak.mag)
		{
		}

		Peak::~Peak()
		{
		}

		bool Peak::operator==(const Peak& peak) const
		{
			return (freq==peak.freq && mag==peak.mag);
		}

		const Peak& Peak::operator=(const Peak& peak)
		{
			freq = peak.freq;
			mag = peak.mag;
			return *this;
		}

///////// SinTrackNode /////////////////////////////
		SinTrackNode::SinTrackNode()
			: freq(0.0)
			, timeIndex(0)
		{
		}

		SinTrackNode::SinTrackNode(double f, unsigned t, const QColor& c)
			: freq(f)
			, timeIndex(t)
			, color(c)
		{
		}

		SinTrackNode::SinTrackNode(const SinTrackNode& node)
			: freq(node.freq)
			, timeIndex(node.timeIndex)
			, color(node.color)
		{
		}

		SinTrackNode::~SinTrackNode()
		{
		}

		SinTrackNode& SinTrackNode::operator=(const SinTrackNode& node)
		{
			freq = node.freq;
			timeIndex = node.timeIndex;
			color = node.color;
			return *this;
		}
	}
}

// END

