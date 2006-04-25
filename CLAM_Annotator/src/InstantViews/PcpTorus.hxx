#ifndef PcpTorus_hxx
#define PcpTorus_hxx

#include <QtOpenGL/QGLWidget>
#undef GetClassName
#include <CLAM/Array.hxx>
#include "Project.hxx"

namespace CLAM_Annotator { class FrameDivision; }

namespace CLAM
{
namespace VM
{

	class PcpTorus : public QGLWidget
	{
		Q_OBJECT

		public:
			PcpTorus(QWidget * parent);
			~PcpTorus();
			void initializeGL();
			void resizeGL(int width, int height);
			void paintGL();
		private:
			void Draw();
			void DrawTile(int x, int y);

		public slots:
			void setCurrentTime(double timeMiliseconds);
		private:
			const std::string & getLabel(unsigned pitch);
		public:
			void setSource(const CLAM_Annotator::Project & project, const std::string & scope, const std::string & name);
			void updateData(const CLAM::DescriptionDataPool & data, CLAM::TData samplingRate);
			void initData(const CLAM_Annotator::FrameDivision & frameDivision, const CLAM::DataArray * arrays, unsigned nFrames, const std::list<std::string> & binLabels, CLAM::TData samplingRate);
			void initData(unsigned nFrames);
		private:
			unsigned _nPcps;
			unsigned _currentFrame;
			double *_pcp;
			double *_pcps;
			QFont _font;
			GLuint _gradient;
			int _updatePending;
			const CLAM_Annotator::FrameDivision * _frameDivision;
			std::vector<std::string> _binLabels;
			unsigned _nBins;
			CLAM::TData _samplingRate;
			double _maxValue;
			const CLAM_Annotator::Project * _project;
			std::string _scope;
			std::string _name;
	};


}
}



#endif// PcpTorus_hxx
