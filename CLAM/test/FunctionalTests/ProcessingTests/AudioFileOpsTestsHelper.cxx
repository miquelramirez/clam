#include "AudioFileOpsTestsHelper.hxx"
#include "similarityHelper.hxx"
#include "PCMAudioStream.hxx"

namespace CLAMTest
{
	double fileSimilarity( const CLAM::AudioFileSource& file, int channel,
			       const CLAM::Audio& rhs )
	{
		CLAM::AudioFileSource source = file;
		CLAM::AudioCodecs::Stream* stream = source.GetStream();
		CLAM::DataArray samplesExtracted;
		samplesExtracted.Resize( rhs.GetSize() );
		samplesExtracted.SetSize( rhs.GetSize() );
		

		stream->PrepareReading();

		stream->ReadData( channel, 
				 samplesExtracted.GetPtr(), 
				 samplesExtracted.Size() );


		stream->Dispose();

		delete stream;

		return evaluateSimilarity( samplesExtracted,
					   rhs.GetBuffer() );
	}

	void fileSimilarity( const CLAM::AudioFileSource& file,
			     const CLAM::Audio& left, const CLAM::Audio& right,
			     double& similarityLeft, double& similarityRight )
	{
		CLAM::AudioFileSource source = file;
		CLAM::AudioCodecs::Stream* stream = source.GetStream();

		CLAM::DataArray samplesExtractedLeft;
		samplesExtractedLeft.Resize( left.GetSize() );
		samplesExtractedLeft.SetSize( left.GetSize() );

		CLAM::DataArray samplesExtractedRight;
		samplesExtractedRight.Resize( right.GetSize() );
		samplesExtractedRight.SetSize( right.GetSize() );

		stream->PrepareReading();

		int* channelBuff = new int[2];
		channelBuff[0] = 0;
		channelBuff[1] = 1;
		
		CLAM::TData** samplesMatrix = new CLAM::TData*[2];
		samplesMatrix[0] = samplesExtractedLeft.GetPtr();
		samplesMatrix[1] = samplesExtractedRight.GetPtr();

		stream->ReadData( channelBuff, 2,
				 samplesMatrix, left.GetSize() );

		stream->Dispose();

		similarityLeft = evaluateSimilarity( samplesExtractedLeft, left.GetBuffer() );
		similarityRight = evaluateSimilarity( samplesExtractedRight, right.GetBuffer() );

		delete channelBuff;
		delete samplesMatrix;
		delete stream;
		
	}

}
