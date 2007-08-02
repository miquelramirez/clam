#ifndef FormantExtractor_hxx
#define FormantExtractor_hxx


#include <CLAM/Audio.hxx>
#include <CLAM/SpectralPeakArray.hxx>
#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Frame.hxx>
#include <CLAM/NullProcessingConfig.hxx>

namespace CLAM
{
	class FormantExtractor : public Processing
	{
		InPort<Audio> AudioIn;
		OutPort<SpectralPeakArray> FormantsOut;
	public:
		const char* GetClassName() const { return "FormantExtractor"; }
		FormantExtractor();

		bool Do();

		bool Do(Audio& in, SpectralPeakArray& out);

	};

}
#endif // FormantExtractor_hxx
