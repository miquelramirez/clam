#include "DescriptorComputation.hxx"
#include "Descriptor.hxx"

namespace CLAM
{
	DescriptorComputation::DescriptorComputation()
	{
		Configure( DescriptorComputationConfig() );
	}

	DescriptorComputation::DescriptorComputation( const DescriptorComputationConfig& cfg )
	{
		Configure( cfg );
	}

	DescriptorComputation::~DescriptorComputation()
	{
	}

	bool DescriptorComputation::ConcreteConfigure( const ProcessingConfig& cfg )
	{
		CopyAsConcreteConfig( mConfig, cfg );
		return true;
	}

	const char* DescriptorComputation::GetClassName() const
	{
		return "DescriptorComputation";
	}

	bool DescriptorComputation::Do( void )
	{
		/* not implemented */
		return false;
	}

	bool DescriptorComputation::Do( Descriptor& input )
	{
		input.Compute();
		return true;
	}

}
