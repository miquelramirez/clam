#include <algorithm>
#include "CLAMGL.hxx"
#include "NetAudioPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		NetAudioPlotController::NetAudioPlotController() 
		{
			_mustProcessData = false;
			SetvRange(TData(-1.0),TData(1.0));
		}

		NetAudioPlotController::~NetAudioPlotController()
		{
		}

		void NetAudioPlotController::SetData(const Audio& audio)
		{
			_audio = audio;
			FullView();
			_mustProcessData = true;
			emit requestRefresh();
		}

		void NetAudioPlotController::SetDataColor(Color c)
		{
			_dRenderer.SetColor(c);
		}

		void NetAudioPlotController::Draw()
		{
			if(MustProcessData()) ProcessData();
			_dRenderer.Render();
			DrawAxis();
		}

		void NetAudioPlotController::FullView()
		{
			SetnSamples(_audio.GetBuffer().Size());

			_view.left = TData(0.0);
			_view.right = TData(GetnSamples());
			_view.top = GetvMax();
			_view.bottom = GetvMin();
	
			emit sendView(_view);
		}

		void NetAudioPlotController::DrawAxis()
		{
			Color c = _dRenderer.GetColor();
			glColor3ub(c.r,c.g,c.b);
			glBegin(GL_LINES);
				glVertex3f(0.0f,1.0,-1.0f);
				glVertex3f(float(GetnSamples()),1.0f,-1.0f);
			glEnd();
		}

		void NetAudioPlotController::ProcessData()
		{
			TSize offset = 0;
			TSize len = GetnSamples();

			if(_processedData.Size() <= len)
				_processedData.Resize(len+1);
			_processedData.SetSize(len+1);

			std::copy(_audio.GetBuffer().GetPtr()+offset,_audio.GetBuffer().GetPtr()+offset+len+1,_processedData.GetPtr());
			
			_dRenderer.SetDataPtr(_processedData.GetPtr(),_processedData.Size(),NormalMode);	
			_mustProcessData = false;
		}

		bool NetAudioPlotController::MustProcessData() const
		{
			return _mustProcessData;
		}
	}
}

// END

