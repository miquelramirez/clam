#include "vmRenderer.hxx"

namespace CLAM
{
	namespace VM
	{
		Renderer::Renderer()
			: mEnabled(true)
		{
		}

		Renderer::~Renderer()
		{
		}

		void Renderer::SetEnabled(bool enabled)
		{
			mEnabled = enabled;
		}
	}
}

// END
