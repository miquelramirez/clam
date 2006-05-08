/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef PcpTorus_hxx
#define PcpTorus_hxx

#include <QtOpenGL/QGLWidget>
#undef GetClassName
#include "Project.hxx"
#include <vector>

namespace CLAM_Annotator { class FrameDivision; }

namespace CLAM
{
namespace VM
{

	class InstantView : public QGLWidget
	{
		Q_OBJECT
		public:
			InstantView(QWidget *parent) : QGLWidget(parent) { }
			virtual void updateData(const CLAM::DescriptionDataPool & data, CLAM::TData samplingRate)=0;
			virtual void clearData()=0;
		public slots:
			virtual void setCurrentTime(double timeMiliseconds)=0;
	};

	class PcpTorus : public InstantView
	{
		Q_OBJECT

		public:
			PcpTorus(QWidget * parent);
			~PcpTorus();
			virtual void initializeGL();
			virtual void resizeGL(int width, int height);
			virtual void paintGL();
		private:
			void Draw();
			void DrawTile(int x, int y);

		public slots:
			virtual void setCurrentTime(double timeMiliseconds);
		private:
			const std::string & getLabel(unsigned pitch);
		public:
			void setSource(const CLAM_Annotator::Project & project, const std::string & scope, const std::string & name);
			void updateData(const CLAM::DescriptionDataPool & data, CLAM::TData samplingRate);
			void clearData();
		protected:
			double *_frameData;
			int _updatePending;
			std::vector<std::string> _binLabels;
			double _maxValue;
			unsigned _nBins;
		private:
			unsigned _nFrames;
			unsigned _currentFrame;
			std::vector<double> _data;
			QFont _font;
			const CLAM_Annotator::FrameDivision * _frameDivision;
			CLAM::TData _samplingRate;
			const CLAM_Annotator::Project * _project;
			std::string _scope;
			std::string _name;
	};


}
}



#endif// PcpTorus_hxx
