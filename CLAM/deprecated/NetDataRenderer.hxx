#ifndef __NETDATARENDERER__
#define __NETDATARENDERER__

#include "DataTypes.hxx"
#include "Renderer.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetDataRenderer : public Renderer
		{
		public:
			NetDataRenderer();
			~NetDataRenderer();

			void SetDataPtr(const TData* data,unsigned nElems);
			void Render();

		private:
			const TData* mData;
			unsigned     mElems;
		};
	}
}

#endif


