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
		CAS_DE_TEST( testConstructor );
		CAS_DE_TEST( testInsert_aPoint );
		CAS_DE_TEST( testInsert_inTheMiddle );
		CAS_DE_TEST( testInsert_beyondLimit );
		CAS_DE_TEST( testInsert_behindLimit );
		CAS_DE_TEST( testPickOffset_withExactValue );
		CAS_DE_TEST( testPickOffset_withNonMatchingValue );
		CAS_DE_TEST( testPickOffset_withinTolerance );
		CAS_DE_TEST( testPickOffset_withSeveralPointsWithinTolerance );
		CAS_DE_TEST( testPickOffset_outsideTheRange );
		CAS_DE_TEST( testPickOnset_withExactValue );
		CAS_DE_TEST( testPickOnset_withNonMatchingValue );
		CAS_DE_TEST( testPickOnset_withinTolerance );
		CAS_DE_TEST( testPickOnset_withSeveralPointsWithinTolerance );
		CAS_DE_TEST( testPickOnset_outsideTheRange );
		CAS_DE_TEST( testPickSegmentBody );
		CAS_DE_TEST( testPickSegmentBody_behindRange );
		CAS_DE_TEST( testPickSegmentBody_afterRange );
		CAS_DE_TEST( testPickSegmentBody_atABound );
		CAS_DE_TEST( testPickSegmentBody_atZeroBound );
		CAS_DE_TEST( testSelection );
		CAS_DE_TEST( testSelection_multiple );
		CAS_DE_TEST( testDeselection );
		CAS_DE_TEST( testInsert_movesSelection );
		CAS_DE_TEST( testClearSelection );
		CAS_DE_TEST( testDragOffset );
		CAS_DE_TEST( testDragOffset_beyondNextBound );
		CAS_DE_TEST( testDragOffset_beyondPreviousBound );
		CAS_DE_TEST( testDragOffset_ofLastBoundHasNoEffect );
		CAS_DE_TEST( testDragOffset_ofPreLastSegment );
		CAS_DE_TEST( testDragOffset_ofFirstBoundLimitIsZero );
		CAS_DE_TEST( testDragOnset );
		CAS_DE_TEST( testDragOnset_beyondNextBound );
		CAS_DE_TEST( testDragOnset_beyondPreviousBound );
		CAS_DE_TEST( testDragOnset_ofFirstBoundHasNoEffect );
		CAS_DE_TEST( testDragOnset_ofLastSegment );
		CAS_DE_TEST( testDragOnset_ofLastBoundLimitIsLength );
		CAS_DE_TEST( testRemove );
		CAS_DE_TEST( testRemove_movesSelection );
		CAS_DE_TEST( testRemove_removesRemovedSelection );
		CAS_DE_TEST( testRemove_firstSegment );
		CAS_DE_TEST( testRemove_lastSegmentExpandsPrevious );
		CAS_DE_TEST( testRemove_singleSegmentHasNoEffect );
	}
	void testConstructor()
	{
		Segmentation segmentation(200.0);

		ASSERT_IGUALS("(0,200) ", segmentation.boundsAsString());
	}
	void testInsert_aPoint()
	{
		Segmentation segmentation(200.0);
		unsigned pos = segmentation.insert(100);

		ASSERT_IGUALS(1u, pos);
		ASSERT_IGUALS("(0,100) (100,200) ", segmentation.boundsAsString());
	}
	void testInsert_inTheMiddle()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(50);
		segmentation.insert(150);
		unsigned pos = segmentation.insert(100);

		ASSERT_IGUALS(2u, pos);
		ASSERT_IGUALS("(0,50) (50,100) (100,150) (150,200) ", segmentation.boundsAsString());
	}
	void testInsert_beyondLimit()
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
	void testInsert_behindLimit()
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
	void testPickOffset_withExactValue()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(50);
		segmentation.insert(100);
		segmentation.insert(150);

		unsigned position= segmentation.pickOffset(100,0.5);
		ASSERT_IGUALS(1u, position);
	}
	void testPickOffset_withNonMatchingValue()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(50);
		segmentation.insert(100);
		segmentation.insert(150);

		unsigned position= segmentation.pickOffset(125,0.5);
		ASSERT_IGUALS(4u, position);
	}
	void testPickOffset_withinTolerance()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(50);
		segmentation.insert(100);
		segmentation.insert(150);

		unsigned position= segmentation.pickOffset(100.2,0.5);
		ASSERT_IGUALS(1u, position);
	}
	void testPickOffset_withSeveralPointsWithinTolerance()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		unsigned position= segmentation.pickOffset(101,20);
		ASSERT_IGUALS(1u, position);
	}
	void testPickOffset_outsideTheRange()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		unsigned position= segmentation.pickOffset(221,20);
		ASSERT_IGUALS(4u, position);
	}
	void testPickOnset_withExactValue()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(50);
		segmentation.insert(100);
		segmentation.insert(150);

		unsigned position= segmentation.pickOnset(100,0.5);
		ASSERT_IGUALS(2u, position);
	}
	void testPickOnset_withNonMatchingValue()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(50);
		segmentation.insert(100);
		segmentation.insert(150);

		unsigned position= segmentation.pickOnset(125,0.5);
		ASSERT_IGUALS(4u, position);
	}
	void testPickOnset_withinTolerance()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(50);
		segmentation.insert(100);
		segmentation.insert(150);

		unsigned position= segmentation.pickOnset(100.2,0.5);
		ASSERT_IGUALS(2u, position);
	}
	void testPickOnset_withSeveralPointsWithinTolerance()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		unsigned position= segmentation.pickOnset(101,20);
		ASSERT_IGUALS(2u, position);
	}
	void testPickOnset_outsideTheRange()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		unsigned position= segmentation.pickOnset(221,20);
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
	void testPickSegmentBody_behindRange()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		unsigned position= segmentation.pickSegmentBody(-1);
		ASSERT_IGUALS(4u, position);
	}
	void testPickSegmentBody_afterRange()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		unsigned position= segmentation.pickSegmentBody(201);
		ASSERT_IGUALS(4u, position);
	}
	void testPickSegmentBody_atABound()
	{
		// This behaviour is not specified just to know what happens
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		unsigned position= segmentation.pickSegmentBody(100);
		ASSERT_IGUALS(1u, position);
	}
	void testPickSegmentBody_atZeroBound()
	{
		// This behaviour is not specified just to know what happens
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		unsigned position= segmentation.pickSegmentBody(0);
		ASSERT_IGUALS(0u, position);
	}
	void testSelection()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.select(2);

		ASSERT_IGUALS("(0,90) (90,100) +(100,110) (110,200) ", segmentation.boundsAsString());
	}
	void testSelection_multiple()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.select(2);
		segmentation.select(1);

		ASSERT_IGUALS("(0,90) +(90,100) +(100,110) (110,200) ", segmentation.boundsAsString());
	}
	void testDeselection()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.select(2);
		segmentation.select(1);
		segmentation.deselect(2);

		ASSERT_IGUALS("(0,90) +(90,100) (100,110) (110,200) ", segmentation.boundsAsString());
	}
	void testInsert_movesSelection()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.select(1);
		segmentation.select(2);

		segmentation.insert(95);

		ASSERT_IGUALS("(0,90) +(90,95) (95,100) +(100,110) (110,200) ", segmentation.boundsAsString());
	}
	void testClearSelection()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.select(1);
		segmentation.select(2);

		segmentation.clearSelection();

		ASSERT_IGUALS("(0,90) (90,100) (100,110) (110,200) ", segmentation.boundsAsString());
	}
	void testDragOffset()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.dragOffset(1,105);

		ASSERT_IGUALS("(0,90) (90,105) (105,110) (110,200) ", segmentation.boundsAsString());
	}
	void testDragOffset_beyondNextBound()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.dragOffset(1,115);

		ASSERT_IGUALS("(0,90) (90,110) (110,110) (110,200) ", segmentation.boundsAsString());
	}
	void testDragOffset_beyondPreviousBound()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.dragOffset(1,85);

		ASSERT_IGUALS("(0,90) (90,90) (90,110) (110,200) ", segmentation.boundsAsString());
	}
	void testDragOffset_ofLastBoundHasNoEffect()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.dragOffset(4,190);

		ASSERT_IGUALS("(0,90) (90,100) (100,110) (110,200) ", segmentation.boundsAsString());
	}
	void testDragOffset_ofPreLastSegment()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.dragOffset(3,114);

		ASSERT_IGUALS("(0,90) (90,100) (100,110) (110,200) ", segmentation.boundsAsString());
	}
	void testDragOffset_ofFirstBoundLimitIsZero()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.dragOffset(0,-12);

		ASSERT_IGUALS("(0,0) (0,100) (100,110) (110,200) ", segmentation.boundsAsString());
	}
	void testDragOnset()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.dragOnset(2,105);

		ASSERT_IGUALS("(0,90) (90,105) (105,110) (110,200) ", segmentation.boundsAsString());
	}
	void testDragOnset_beyondNextBound()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.dragOnset(2,115);

		ASSERT_IGUALS("(0,90) (90,110) (110,110) (110,200) ", segmentation.boundsAsString());
	}
	void testDragOnset_beyondPreviousBound()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.dragOnset(2,85);

		ASSERT_IGUALS("(0,90) (90,90) (90,110) (110,200) ", segmentation.boundsAsString());
	}
	void testDragOnset_ofFirstBoundHasNoEffect()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.dragOnset(0,10);

		ASSERT_IGUALS("(0,90) (90,100) (100,110) (110,200) ", segmentation.boundsAsString());
	}
	void testDragOnset_ofLastSegment()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.dragOnset(3,114);

		ASSERT_IGUALS("(0,90) (90,100) (100,114) (114,200) ", segmentation.boundsAsString());
	}
	void testDragOnset_ofLastBoundLimitIsLength()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.dragOnset(3,210);

		ASSERT_IGUALS("(0,90) (90,100) (100,200) (200,200) ", segmentation.boundsAsString());
	}
	void testRemove()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		ASSERT_IGUALS("(0,90) (90,100) (100,110) (110,200) ", segmentation.boundsAsString());

		segmentation.remove(2);

		ASSERT_IGUALS("(0,90) (90,110) (110,200) ", segmentation.boundsAsString());
	}
	void testRemove_movesSelection()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.select(1);
		segmentation.select(3);

		segmentation.remove(2);

		ASSERT_IGUALS("(0,90) +(90,110) +(110,200) ", segmentation.boundsAsString());
	}
	void testRemove_removesRemovedSelection()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.select(2);

		segmentation.remove(2);

		ASSERT_IGUALS("(0,90) (90,110) (110,200) ", segmentation.boundsAsString());
	}
	void testRemove_firstSegment()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.select(1);
		segmentation.select(3);

		segmentation.remove(0);

		ASSERT_IGUALS("+(0,100) (100,110) +(110,200) ", segmentation.boundsAsString());
	}
	void testRemove_lastSegmentExpandsPrevious()
	{
		Segmentation segmentation(200.0);
		segmentation.insert(90);
		segmentation.insert(100);
		segmentation.insert(110);

		segmentation.select(1);
		segmentation.select(3);

		segmentation.remove(3);

		ASSERT_IGUALS("(0,90) +(90,100) (100,200) ", segmentation.boundsAsString());
	}
	void testRemove_singleSegmentHasNoEffect()
	{
		Segmentation segmentation(200.0);

		segmentation.remove(0);

		ASSERT_IGUALS("(0,200) ", segmentation.boundsAsString());
	}
};

REGISTRA_TEST(SegmentationTest)

}

}

