#include "CLAM_Math.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"
#include "DPMultiPlot.hxx"
using CLAM::TData;
using CLAM::DataArray;
using CLAM::VM::DPMultiPlot;
int main()
{
	DataArray sig0;
	DataArray sig1;
	sig0.Resize(11025);
	sig1.Resize(11025);
	sig0.SetSize(11025);
	sig1.SetSize(11025);
	double f=20.0;
	double fs=11025.0;
	double t=0.0;
	double x0,x1;
	int j=0;
	while(t <= 1)
	{
		x0=cos(2*PI*f*t);
		x1=sin(2*PI*f*t);
		sig0[j]=TData(x0);
		sig1[j]=TData(x1);
		t+=1.0/fs;
		j++;
	}
	DPMultiPlot plot;
	plot.Label("MultiPlot");
	plot.Geometry(100,100,500,225);
	plot.AddData("data0",sig0);
	plot.AddData("data1",sig1);
	plot.SetColor("data0",CLAM::VM::VMColor::Cyan());
	plot.SetColor("data1",CLAM::VM::VMColor::Magenta());
	plot.SetXRange(0.0,1.0);
	plot.SetYRange(-1.5,1.5);
	plot.SetUnits("s","");
	plot.SetToolTips("Instant Time","Amplitude");

	plot.Run();
}
// END

