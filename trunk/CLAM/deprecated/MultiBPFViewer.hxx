#ifndef __MULTIBPFVIEWER__
#define __MULTIBPFVIEWER__

#include <string>
#include "BPF.hxx"
#include "VMColor.hxx"

namespace CLAM
{
	namespace VM
	{
		class BPFEditor;

		/**
		 * Viewing several BPFs at time.
		 *
		 * @ingroup QTVM
		 */

		class MultiBPFViewer
		{
		public:
			MultiBPFViewer();
			~MultiBPFViewer();
				
			void Label(const std::string& label);
			void Geometry(int x, int y, int w, int h);

			void SetXRange(const double& min, const double& max, const EScale& scale=EScale::eLinear);
			void SetYRange(const double& min, const double& max, const EScale& scale=EScale::eLinear);

			void SetXScale(const EScale& scale);
			void SetYScale(const EScale& scale);

			/**
			 * Sets default BPF. 
			 * Use this method if you wish only a single BPF.
			 */
			void SetData(const BPF& bpf); 

			/**
			 * Adds additional BPF.
			 * Use this method to add additional BPFs after the default one.
			 */
			void AddData(const std::string& key, const BPF& bpf);
		   
			void SetDataColor(const std::string& key, const Color& lines_color, const Color& handlers_color);

			void Run();

		private:
			BPFEditor* mPlot;
				
		};
	}
}

#endif

