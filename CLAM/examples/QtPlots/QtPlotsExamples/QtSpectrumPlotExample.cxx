#include <qmessagebox.h>
#include "Spectrum.hxx"
#include "XMLStorage.hxx"
#include "QtAppWrapper.hxx"
#include "QtSpectrumPlot.hxx"
using CLAM::Spectrum;
using CLAM::XMLStorage;
using CLAM::VM::QtSpectrumPlot;
int main()
{
	FILE *fp = NULL;

	CLAM::VM::QtAppWrapper::Init();
	if((fp=fopen("../../data/spectrum_data.xml","r"))==NULL)
	{
		QMessageBox message("Required file not found",
		                "spectrum_data.xml not found!",
						QMessageBox::Critical,
						QMessageBox::Ok,
						QMessageBox::NoButton,
						QMessageBox::NoButton); 
		message.exec();
		CLAM::VM::QtAppWrapper::Quit();
		return 0;
	}
	else
	{
		fclose(fp);
	}
	Spectrum spec;
	XMLStorage::Restore(spec,"../../data/spectrum_data.xml");

	QtSpectrumPlot specPlot;
	specPlot.Label("Spectrum");
	specPlot.Geometry(100,100,500,225);
	specPlot.SetData(spec);
	specPlot.Show();
	return CLAM::VM::QtAppWrapper::Run();
}
// END

