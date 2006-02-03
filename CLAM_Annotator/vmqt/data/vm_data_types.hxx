#ifndef __VMQT_DATA_TYPES_H__
#define __VMQT_DATA_TYPES_H__

namespace CLAM
{
	namespace VM
	{
		struct Color
		{
			unsigned short r;
			unsigned short g;
			unsigned short b;
			unsigned short a;

			Color();
			Color(unsigned short red, unsigned short green, unsigned short blue, unsigned short alpha=255);
			Color(const Color& c);
			~Color();

			const Color& operator=(const Color& c);
			bool operator==(const Color& c);
		};

		struct GLView
		{
			double left;
			double right;
			double bottom;
			double top;
			double near;
			double far;

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

			double span();
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

			void init();
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
			CLAM::VM::Color color;

			SinTrackNode();
			SinTrackNode(double f, unsigned t, const CLAM::VM::Color& c);
			SinTrackNode(const SinTrackNode& node);
			~SinTrackNode();

			SinTrackNode& operator=(const SinTrackNode& node);

		};
	}
}

#endif
