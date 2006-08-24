#include "peaksatt.h"

peaksatt::peaksatt() 
{
	tini=0;
	tend=0;
	specpeaks.clear();
}

peaksatt::peaksatt(int t_ini, int t_end, mapa2 spectrum_peaks) 
{
	tini=t_ini;
	tend=t_end;

	specpeaks.clear();
	for (itermapa2 curr=spectrum_peaks.begin(); curr!=spectrum_peaks.end(); curr++)
		specpeaks.insert(*curr);
}

peaksatt::peaksatt(const peaksatt &patt)
{
	tini=patt.tini;
	tend=patt.tend;
//	specpeaks=patt.specpeaks;

	specpeaks.clear();
	for (mapa2::const_iterator curr=(patt.specpeaks).begin(); curr!=(patt.specpeaks).end(); curr++)
		specpeaks.insert(*curr);
}

ostream &operator<<(ostream &output, peaksatt patt)
{
	output << "t_ini=" << patt.tini << " ";
	output << "t_end=" << patt.tend;
	output << "\nSpectrum peaks=" << patt.specpeaks.size() << endl;
	for (itermapa2 curr=(patt.specpeaks).begin(); curr!=(patt.specpeaks).end(); curr++)
		output << curr->first << "= " << curr->second << endl;
	return output;
}
