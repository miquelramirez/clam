#include "EmbededWidgets.hxx"


namespace CLAM
{

EmbededWidgetCreatorBase::Creators & EmbededWidgetCreatorBase::creators()
{
		static Creators creators;
		return creators;
}


}




