#ifndef __DATARENDERER__
#define __DATARENDERER__

#include "Array.hxx"
#include "Renderer.hxx"

namespace CLAM
{
	namespace VM
	{
		enum {NormalMode=0,DetailMode,HugeMode};

		class DataRenderer : public Renderer
		{
			public:
				DataRenderer();
				virtual ~DataRenderer();

				void SetDataPtr(const TData* data,unsigned nElems,int mode);
				void SetArrays(const TData* maxsArray,const TData* minsArray,TSize nElems);
				void SetStep(const TData& step);
				TData GetStep() const;
				void Render();

			private:
				const TData* _data;
				const TData* _maxsPtr;
				const TData* _minsPtr;
				unsigned _nElems;
				int _mode;
				TData _step;

				void RenderingNormalMode();
				void RenderingDetailMode();
				void RenderingHugeMode();
		};
	}
}

#endif


