#ifndef __TOKENDELAYCONTROLLER__
#define __TOKENDELAYCONTROLLER__

#include "ProcessingController.hxx"
namespace CLAM
{
	class Spectrum;
	template <class T> class TokenDelay;
};

namespace CLAMVM
{
	using CLAM::TokenDelay;
	using CLAM::Spectrum;
		

	class SpectralTokenDelayController 
		: public ProcessingController
	{
	protected:
		const TokenDelay<Spectrum>*  mObserved;

	public:
		SpectralTokenDelayController();

		~SpectralTokenDelayController();

		virtual const char* GetClassName() const
		{
			return "SpectralTokenDelayController";
		}

		virtual bool BindTo( Processing& proc );

		virtual bool Publish();
	};
}

#endif // TokenDelayController.hxx
