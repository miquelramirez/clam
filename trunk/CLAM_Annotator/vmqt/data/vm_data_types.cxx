#include "vm_data_types.hxx"

namespace CLAM
{
	namespace VM
	{

/////// Color //////////////
		Color::Color()
			: r(0)
			, g(0)
			, b(0)
			, a(255)
		{
		}

		Color::Color(unsigned short red, unsigned short green, unsigned short blue, unsigned short alpha)
			: r(red)
			, g(green)
			, b(blue)
			, a(alpha)
		{
		}

		Color::Color(const Color& c)
			: r(c.r)
			, g(c.g)
			, b(c.b)
			, a(c.a)
		{
		}

		Color::~Color()
		{
		}

		const Color& Color::operator=(const Color& c)
		{
			r = c.r;
			g = c.g;
			b = c.b;
			a = c.a;
			return *this;
		}
	
		bool Color::operator==(const Color& c)
		{
			return (r==c.r && g==c.g && b==c.b && a==c.a);
		}

///////// GLView /////////////////////
		GLView::GLView()
			: left(-1.0)
			, right(1.0)
			, bottom(-1.0)
			, top(1.0)
			, near(-1.0)
			, far(1.0)
		{
		}

		GLView::GLView(double leftv, double rightv, double bottomv, double topv, double nearv, double farv)
			: left(leftv)
			, right(rightv)
			, bottom(bottomv)
			, top(topv)
			, near(nearv)
			, far(farv)
		{
		}

		GLView::GLView(const GLView& v)
			: left(v.left)
			, right(v.right)
			, bottom(v.bottom)
			, top(v.top)
			, near(v.near)
			, far(v.far)
		{
		}

		GLView::~GLView()
		{
		}

		bool GLView::operator==(const GLView& v)
		{
			return (left==v.left && right==v.right && bottom==v.bottom && top==v.top && near==v.near && far==v.far);
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

		double Range::span()
		{
			return (max-min);
		}

/////////// Keyboard //////////////////////
		Keyboard::Keyboard()
		{
			init();
		}

		Keyboard::~Keyboard()
		{
		}

		void Keyboard::init()
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

		SinTrackNode::SinTrackNode(double f, unsigned t, const CLAM::VM::Color& c)
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
