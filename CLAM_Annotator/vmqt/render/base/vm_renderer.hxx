#ifndef __VMQT_RENDERER_H__
#define __VMQT_RENDERER_H__

#include <QObject>

namespace CLAM
{
	namespace VM
	{

		class Renderer : public QObject
		{
		public:
			Renderer();
			~Renderer();

			virtual void render()=0;

			virtual void set_enabled(bool enabled);

		protected:
			bool rd_enabled;
		};
	}
}

#endif

