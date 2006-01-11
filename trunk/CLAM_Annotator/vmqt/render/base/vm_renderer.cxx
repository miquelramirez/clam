#include "vm_renderer.hxx"

namespace CLAM
{
	namespace VM
	{
		Renderer::Renderer()
			: rd_enabled(true)
		{
		}

		Renderer::~Renderer()
		{
		}

		void Renderer::set_enabled(bool enabled)
		{
			rd_enabled = enabled;
		}
	}
}

// END
