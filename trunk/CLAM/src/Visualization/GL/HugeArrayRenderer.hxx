#ifndef __HUGEARRAYRENDERER__
#define __HUGEARRAYRENDERER__

#include "GLRenderer.hxx"
#include "Array.hxx"

namespace CLAMVM
{

		using CLAM::TData;
		using CLAM::TIndex;
		using CLAM::TSize;
		using CLAM::DataArray;

		class HugeArrayRenderer : public GLRenderer
		{
		public:

				void CacheData( const DataArray& hugeArray );

				void ExecuteGLCommands();

		protected:
				
				void BuildMaxMinArrays( const DataArray& hugeArray );
				void BuildMaxMin10Arrays( );

		private:
				
				DataArray mMaxs;
				DataArray mMins;
				DataArray mMaxs10;
				DataArray mMins10;
	
		};

}

#endif // HugeArrayRenderer.hxx
