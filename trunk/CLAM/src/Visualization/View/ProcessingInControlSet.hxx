#ifndef __PROCESSINGINCONTROLSET__
#define __PROCESSINGINCONTROLSET__

#include "InputControlModel.hxx"
#include <map>
#include <string>

namespace CLAMVM
{
	class ProcessingInControlSet
	{
	private:
		typedef std::map< std::string, InputControlModel* >   tChildTbl;
		typedef tChildTbl::value_type                         tChildvalue_type;
	protected:		

		tChildTbl                                             mChildren;
				
	public:
		typedef tChildTbl::iterator                           iterator;
		typedef tChildTbl::const_iterator                     const_iterator;

		ProcessingInControlSet();

		virtual ~ProcessingInControlSet();
				
		void Insert( std::string name, InputControlModel& a );
				
		InputControlModel& Retrieve( std::string name );

		iterator begin()
		{
			return mChildren.begin();
		}

		const_iterator begin() const
		{
			return mChildren.begin();
		}

		iterator end()
		{
			return mChildren.end();
		}

		const_iterator end() const
		{
			return mChildren.end();
		}

		Signalv1< std::string >         NamePublished;
	};

}

#endif // ProcessingInControlSet.hxx
