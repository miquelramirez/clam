#ifndef __RENDERER__
#define __RENDERER__

#include "DataTypes.hxx"
#include "VMColor.hxx"

namespace CLAM
{
	namespace VM
	{
		typedef struct{
			TData left;
			TData right;
		} HBounds;

		class Renderer
		{
			public:
				Renderer();
				virtual ~Renderer();

				void SetHBounds(const TData& left, const TData& right);
				void SetVBounds(const TData& top,const TData& bottom);
				void SetColor(Color c);
				Color GetColor() const;

				void SetEnabled(bool e);

				virtual void Render() = 0;

			protected:
				HBounds	_bounds;
				TData _top;
				TData _bottom;
				Color	_color;

				bool IsEnabled();

			private:
				bool _enabled;

				void Init();
		};
	}
}

#endif

