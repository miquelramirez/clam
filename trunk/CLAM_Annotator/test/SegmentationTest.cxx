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
		CAS_DE_TEST( testBoundAsString_asConstructed );
		CAS_DE_TEST( testBoundAsString_insertingAPoint );
		CAS_DE_TEST( testBoundAsString_insertingAPointBeyondLimit );
		CAS_DE_TEST( testBoundAsString_insertingAPointBehindLimit );
		CAS_DE_TEST( testPickEndBound_withExactValue );
		CAS_DE_TEST( testPickEndBound_withNonMatchingValue );
		CAS_DE_TEST( testPickEndBound_withinTolerance );
		CAS_DE_TEST( testPickEndBound_withSeveralPointsWithinTolerance );
		CAS_DE_TEST( testPickEndBound_outsideTheRange );
		CAS_DE_TEST( testPickSegmentBody );
	}
	void testBoundAsString_asConstructed()
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
		ASSERT_IGUALS(4u, position);
	}
	void testPickEndBound_withinTolerance()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(50);
		segmentation.insert(100);
		segmentation.insert(150);

		unsigned position= segmentation.pickEndBound(100.2,0.5);
		ASSERT_IGUALS(1u, position);
	}
	void testPickEndBound_withSeveralPointsWithinTolerance()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		unsigned position= segmentation.pickEndBound(101,20);
		ASSERT_IGUALS(1u, position);
	}
	void testPickEndBound_outsideTheRange()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		unsigned position= segmentation.pickEndBound(221,20);
		ASSERT_IGUALS(4u, position);
	}
	void testPickSegmentBody()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		unsigned position= segmentation.pickSegmentBody(102);
		ASSERT_IGUALS(2u, position);
	}
};

REGISTRA_TEST(SegmentationTest)

}

}

