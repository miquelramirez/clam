#ifndef __RHYTHMIR_CMDPROC__
#define __RHYTHMIR_CMDPROC__

#include "TickExtractorConfig.hxx"
#include <string>

namespace RhythmIR
{
	class CmdLineParms
	{
	public:
		
		std::string GetInputFilename() const;
		std::string GetOutputFilename() const;
		const CLAM::TickExtractorConfig& GetConfig() const;

		static CmdLineParms ParseArgs( int argc, char** argv );

	protected:
		static void PrintHelp();

	private:
		CmdLineParms( std::string in, std::string out,
			      const CLAM::TickExtractorConfig& cfg );

	protected:
		std::string                mInputFilename;
		std::string                mOutputFilename;
		CLAM::TickExtractorConfig  mConfig;
		
	};

	inline std::string CmdLineParms::GetInputFilename() const
	{
		return mInputFilename;
	}

	inline std::string CmdLineParms::GetOutputFilename() const
	{
		return mOutputFilename;
	}

	inline const CLAM::TickExtractorConfig& CmdLineParms::GetConfig() const
	{
		return mConfig;
	}
}

#endif // rhythmir_cmdporc.hxx
