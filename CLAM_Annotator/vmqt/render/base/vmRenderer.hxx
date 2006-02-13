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

			virtual void Render()=0;

			virtual void SetEnabled(bool enabled);

		protected:
			bool mEnabled;
		};
	}
}

#endif

