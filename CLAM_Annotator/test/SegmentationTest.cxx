#include "MiniCppUnit.hxx"

#include "Segmentation.hxx"
#include <sstream>

namespace CLAM_Annotator
{
namespace Tests
{


class SegmentationTest : public GrupDeTests<SegmentationTest>
{
public:
	GRUP_DE_TESTS( SegmentationTest )
	{
		CAS_DE_TEST( testBoundAsString_withDefaultConstructed );
		CAS_DE_TEST( testBoundAsString_insertingAPoint );
		CAS_DE_TEST( testBoundAsString_insertingAPointBeyondLimit );
		CAS_DE_TEST( testBoundAsString_insertingAPointBehindLimit );
		CAS_DE_TEST( testPickEndBound_withExactValue );
		CAS_DE_TEST( testPickEndBound_withNonMatchingValue );
//		CAS_DE_TEST( testPickEndBound_withinTolerance );
	}
	void testBoundAsString_withDefaultConstructed()
	{
		Segmentation segmentation(200.0);

		ASSERT_IGUALS("(0,200) ", segmentation.boundsAsString());
	}
	void testBoundAsString_insertingAPoint()
	{
		Segmentation segmentation(200.0);
		unsigned pos = segmentation.insert(100);

		ASSERT_IGUALS(1u, pos);
		ASSERT_IGUALS("(0,100) (100,200) ", segmentation.boundsAsString());
	}
	void testBoundAsString_insertingAPointBeyondLimit()
	{
		Segmentation segmentation(200.0);
		try
		{
			segmentation.insert(400);
			FALLA("Should have thrown an exception");
		}
		catch (Segmentation::InsertedOutOfBounds & e)
		{
			ASSERT_IGUALS("Segmentation point inserted out of limits", e.what());
		}
		ASSERT_IGUALS("(0,200) ", segmentation.boundsAsString());
	}
	void testBoundAsString_insertingAPointBehindLimit()
	{
		Segmentation segmentation(200.0);
		try
		{
			segmentation.insert(-0.1);
			FALLA("Should have thrown an exception");
		}
		catch (Segmentation::InsertedOutOfBounds & e)
		{
			ASSERT_IGUALS("Segmentation point inserted out of limits", e.what());
		}

		ASSERT_IGUALS("(0,200) ", segmentation.boundsAsString());
	}
	void testPickEndBound_withExactValue()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(50);
		segmentation.insert(100);
		segmentation.insert(150);

		unsigned position= segmentation.pickEndBound(100,0.5);
		ASSERT_IGUALS(1u, position);
	}
	void testPickEndBound_withNonMatchingValue()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(50);
		segmentation.insert(100);
		segmentation.insert(150);

		unsigned position= segmentation.pickEndBound(125,0.5);
		ASSERT_IGUALS(1u, position);
	}
};

REGISTRA_TEST(SegmentationTest)

}

}

