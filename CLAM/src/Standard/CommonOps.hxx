#ifndef __COMMONOPS__
#define __COMMONOPS__

namespace CLAM
{

	template < typename DataType >
	class Multiply
	{
	public:
	
		static const DataType IdentityElement;
		
		template <typename DataType2>
		DataType operator()( DataType op1, DataType2 op2 )
		{
			return op1 * op2;
		}
	};

	template <typename DataType>
		const DataType Multiply<DataType>::IdentityElement = 1;
	
	template< typename DataType >
	class Add
	{
	public:

		static const DataType IdentityElement;

		template <typename DataType2>
		DataType operator() ( DataType op1, DataType2 op2 )
		{
			return op1 + op2;
		}
	};

	template< typename DataType >
		const DataType Add<DataType>::IdentityElement = 0;


}

#endif // CommonOps.hxx
