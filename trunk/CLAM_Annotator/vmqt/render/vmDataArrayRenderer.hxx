#ifndef __VMQT_DATA_ARRAY_RENDERER_H__
#define __VMQT_DATA_ARRAY_RENDERER_H__

#include "Array.hxx"
#include "vmRenderer2D.hxx"
#include <vector>

namespace CLAM
{
	namespace VM
	{

		class DataArrayRenderer : public Renderer2D
		{
		public:
			DataArrayRenderer();
			~DataArrayRenderer();
	
			void SetData(const DataArray& data);
			void SetDataColor(const Color& c);

			void SetHugeModeEnabled(bool en);

		protected:
			void Render();
			void SetHBounds(double left, double right);
			void SetVBounds(double bottom, double top);
			void SetViewport(const GLViewport& v);
			
		private:
			std::vector<CLAM::TData> mCachedData;
			std::vector<CLAM::TData> mProcessedData;
			std::vector<CLAM::TData> mMinArray;
			std::vector<CLAM::TData> mMaxArray;

			bool mHugeMode;
			bool mHugeModeEnabled;
			bool mMustProcessData;

			Color  mDataColor;
			GLView mLocalView;
			
			void ProcessData();
			void DrawHugeMode();
			void DrawNormalMode();
		};
	}
}

#endif


