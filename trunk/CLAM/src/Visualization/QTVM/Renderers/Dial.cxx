#include "CLAMGL.hxx"
#include "Dial.hxx"

namespace CLAM
{
	namespace VM
	{
		Dial::Dial()
		{
			_data = 0;
			SetColor(VMColor::Black());
		}

		Dial::~Dial()
		{
		}

		void Dial::Render()
		{
			if(IsEnabled())
			{
				if(_data < _bounds.left || _data > _bounds.right) return;
				Draw();
			}
		}

		void Dial::Draw()
		{
			glColor3ub(GLubyte(_color.r),GLubyte(_color.g),GLubyte(_color.b));
			glBegin(GL_LINES);
				glVertex2f(float(_data-_bounds.left),float(_top));
				glVertex2f(float(_data-_bounds.left),float(_bottom));
			glEnd();
		}

		void Dial::Update(const TData& value)
		{
			_data = value;
		}

		TData Dial::GetPos() const
		{
			return _data;
		}
	}
}

// END
