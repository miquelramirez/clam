#include "Renderer.hxx"

namespace CLAM
{
	namespace VM
	{
		Renderer::Renderer()
		{
			SetHBounds(TData(0.0),TData(1.0));
			SetVBounds(TData(1.0),TData(-1.0));
			SetEnabled(true);
			SetColor(VMColor::Green());
		}

		Renderer::~Renderer()
		{
		}

		void Renderer::SetHBounds(const TData& left, const TData& right)
		{
			_bounds.left = left;
			_bounds.right = right;
		}

		void Renderer::SetVBounds(const TData& top,const TData& bottom)
		{
			_top = top;
			_bottom = bottom;
		}

		void Renderer::SetColor(Color c)
		{
			_color.r = c.r;
			_color.g = c.g;
			_color.b = c.b;
		}

		Color Renderer::GetColor() const
		{
			return _color;
		}

		void Renderer::SetEnabled(bool e)
		{
			_enabled = e;
		}

		bool Renderer::IsEnabled()
		{
			return _enabled;
		}
	}
}

// END
