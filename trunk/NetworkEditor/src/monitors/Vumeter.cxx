#include <CLAM/ProcessingFactory.hxx>
#include "Vumeter.hxx"

namespace Hidden
{
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, VumeterMonitor> regVumeterMonitor("Vumeter");

	static class VumeterMetadata
	{
	public:
		VumeterMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("Vumeter", "port_monitor_type", typeid(CLAM::TData).name());
			factory.AddAttribute("Vumeter", "icon", "vumeter.svg");
		}
	} dummy;
}

void Vumeter::paintEvent(QPaintEvent * event)
{
	unsigned nLeds=height()/_ledHeight;
	double ledStep = 60./nLeds;
	unsigned actualLedHeight = height()/nLeds;
	double actualBound=1.0;
	if (_dataSource->hasUpperBound())
	{
		double upperBound = _dataSource->upperBound();
		actualBound = std::max(actualBound, std::abs(upperBound));
	}
	if (_dataSource->hasLowerBound())
	{
		double lowerBound = _dataSource->lowerBound();
		actualBound = std::max(actualBound, std::abs(lowerBound));
	}
	double logEnergy = 60 + 20*log(energy()/actualBound);
	if (_memorizedPeak<=logEnergy)
	{
		_remainingPeakMemory=_peakMemory;
		_memorizedPeak=logEnergy;
	}
	unsigned peak = _memorizedPeak*nLeds/60;
	QPainter painter(this);
	painter.setPen(Qt::black);
	for (unsigned i=0; i<nLeds; i++)
	{
		QColor color = _color0;
		if (i>4*nLeds/10) color=_color1;
		if (i>7*nLeds/10) color=_color2;
		if (i>=9*nLeds/10) color=_color3;
		if (logEnergy<=ledStep*i && i!=peak)
			painter.setBrush(color.dark());
		else
			painter.setBrush(color);
		painter.drawRect(
			margin,
			height()-actualLedHeight*i-margin,
			width()-2*margin,
			-actualLedHeight+margin);
	}
	if (_remainingPeakMemory>0)
		_remainingPeakMemory--;
	else
		_memorizedPeak-=_peakDecay;
}

double Vumeter::energy()
{
	double result = 1e-10;
	if ( !_dataSource) return result;
	const CLAM::TData * data = _dataSource->frameData();
	unsigned size = _dataSource->nBins();
	if ( !size)
	{
		_dataSource->release();
		return result;
	}
	for (unsigned i=0; i<size; i++)
	{
		const CLAM::TData & bin = data[i];
		if (bin>result) result=bin;
		if (bin<-result) result=-bin;
	}
	_dataSource->release();
	return result;
}

