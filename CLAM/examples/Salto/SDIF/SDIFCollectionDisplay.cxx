#include "SDIFCollectionDisplay.hxx"
#include "SDIFFrame.hxx"
#include "SDIFMatrix.hxx"
#include "SDIFStream.hxx"
#include <stdio.h>

void SDIFCollectionDisplay::DisplayFrame(const SDIFFrame* pFrame)
{
	printf("  %4s Time:%8f StreamId:%3d N-Matrices:%3d\n",
		pFrame->mHeader.mType.String(),
 		pFrame->mHeader.mTime, pFrame->mHeader.mStreamId, 
 		pFrame->mHeader.mnMatrices);
	SDIFMatrix* pMatrix = pFrame->mpFirst;
	for (int i=0;i<pFrame->mHeader.mnMatrices;i++)
	{
		printf("    %4s DataType:%04x Rows:%4d Cols:%4d\n",
			pMatrix->mHeader.mType.String(),
			pMatrix->mHeader.mDataType,
			pMatrix->mHeader.mnRows,
			pMatrix->mHeader.mnColumns
		);
		
		for (int r=0;r<pMatrix->mHeader.mnRows;r++)
		{
			for (int c=0;c<pMatrix->mHeader.mnColumns;c++)
			{
				printf("      %f",
					pMatrix->GetValue(c,r));
			}
			printf("\n");
		}
		pMatrix=pMatrix->mpNext;
	}	
}

void SDIFCollectionDisplay::Display(void)
{
	SDIFStream* pStream = mpFirstStream;
	while (pStream)
	{
		printf("\nstream %d\n",pStream->mpFirst->mHeader.mStreamId);
		SDIFFrame* pFrame = pStream->mpFirst;
		while (pFrame) {
			DisplayFrame(pFrame);
			pFrame = pFrame->mpNextInStream;
		}
		pStream = pStream->mpNext;
	}
/*	{
		printf("\nALL STREAMS\n");
		SDIFFrame* pFrame = mpFirst;
		while (pFrame) {
			DisplayFrame(pFrame);
			pFrame = pFrame->mpNext;
		}
	}
*/
}
