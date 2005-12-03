#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "DiscontinuousSegmentation.hxx"
#include <sstream>

using CLAM::DiscontinuousSegmentation;

namespace CLAMTest
{
	class DiscontinuousSegmentationTest;

	CPPUNIT_TEST_SUITE_REGISTRATION( DiscontinuousSegmentationTest );

	class DiscontinuousSegmentationTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( DiscontinuousSegmentationTest );
		CPPUNIT_TEST( testConstructor );
		CPPUNIT_TEST( testInsert_atZero );
		CPPUNIT_TEST( testInsert_inTheMiddleOfAnExistingSegment );
		CPPUNIT_TEST( testInsert_beyondLimit );
		CPPUNIT_TEST( testInsert_behindLimit );
		CPPUNIT_TEST( testPickOffset_withExactValue );
		CPPUNIT_TEST( testPickOffset_withNonMatchingValue );
		CPPUNIT_TEST( testPickOffset_withinTolerance );
		CPPUNIT_TEST( testPickOffset_withSeveralPointsWithinTolerance );
		CPPUNIT_TEST( testPickOffset_outsideTheRange );
		CPPUNIT_TEST( testPickOnset_withExactValue );
		CPPUNIT_TEST( testPickOnset_withNonMatchingValue );
		CPPUNIT_TEST( testPickOnset_withinTolerance );
		CPPUNIT_TEST( testPickOnset_withSeveralPointsWithinTolerance );
		CPPUNIT_TEST( testPickOnset_outsideTheRange );
		CPPUNIT_TEST( testPickSegmentBody );
		CPPUNIT_TEST( testPickSegmentBody_behindRange );
		CPPUNIT_TEST( testPickSegmentBody_afterRange );
		CPPUNIT_TEST( testPickSegmentBody_atABound );
		CPPUNIT_TEST( testPickSegmentBody_atZeroBound );
		CPPUNIT_TEST( testSelection );
		CPPUNIT_TEST( testSelection_multiple );
		CPPUNIT_TEST( testDeselection );
		CPPUNIT_TEST( testInsert_movesSelection );
		CPPUNIT_TEST( testClearSelection );
		CPPUNIT_TEST( testDragOnset_ofFirstBoundHasEffect );
		CPPUNIT_TEST( testDragOffset_ofLastBoundHasEffect );
		CPPUNIT_TEST( testDragOnset );
		CPPUNIT_TEST( testDragOffset );
		CPPUNIT_TEST( testDragOnset_thatDoesNotExist );
		CPPUNIT_TEST( testDragOffset_thatDoesNotExist );
		CPPUNIT_TEST( testDragOnset_toLeftLimitedByPreviousOffset );
		CPPUNIT_TEST( testDragOnset_toLeftLimitedByZeroWhenFirstOne );
		CPPUNIT_TEST( testDragOnset_toRightLimitedByOwnOffset );
		CPPUNIT_TEST( testDragOffset_toRightLimitedByNextOnset );
		CPPUNIT_TEST( testDragOffset_toRightLimitedByMaxWhenLastOne );
		CPPUNIT_TEST( testDragOffset_toLeftLimitedByOwnOnset );
		CPPUNIT_TEST( testRemove );
		CPPUNIT_TEST( testRemove_movesSelection );
		CPPUNIT_TEST( testRemove_removesRemovedSelection );
		CPPUNIT_TEST( testRemove_firstSegment );
		CPPUNIT_TEST( testRemove_lastSegmentExpandsPrevious );
		CPPUNIT_TEST( testRemove_singleSegmentHasEffect );
		CPPUNIT_TEST( testConstructor_putsCurrentAtZero );
		CPPUNIT_TEST( testInsert_keepsCurrentAtZero );
		CPPUNIT_TEST( testCurrent_changesCurrent );
		CPPUNIT_TEST( testCurrent_aboveNSegmentsHasNoEffect );
		CPPUNIT_TEST( testInsert_beforeCurrentCorrectsIt );
		CPPUNIT_TEST( testRemove_beforeCurrentCorrectsIt );
		CPPUNIT_TEST( testRemove_afterCurrentKeepsIt );
		CPPUNIT_TEST( testRemove_justTheCurrentChangesIt );
		CPPUNIT_TEST( testRemove_justTheCurrentButItIsZeroKeepsAtZero );
		CPPUNIT_TEST( testInsert_onTheMiddleOfAFrame );
		CPPUNIT_TEST( testInsert_onGapAtTheBegining );
		CPPUNIT_TEST( testInsert_onGapAtTheBeginingMovesCurrentWhenAfter );
		CPPUNIT_TEST_SUITE_END();

	public:

		void testConstructor()
		{
			DiscontinuousSegmentation segmentation(200.0);

			CPPUNIT_ASSERT_EQUAL(std::string(""), segmentation.boundsAsString());
		}
		void testInsert_atZero()
		{
			DiscontinuousSegmentation segmentation(200.0);
			unsigned pos = segmentation.insert(0);
			CPPUNIT_ASSERT_EQUAL(0u, pos);
			CPPUNIT_ASSERT_EQUAL(std::string("(0,200) "), segmentation.boundsAsString());
		}
		void testInsert_inTheMiddleOfAnExistingSegment()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			unsigned pos = segmentation.insert(100);

			CPPUNIT_ASSERT_EQUAL(1u, pos);
			CPPUNIT_ASSERT_EQUAL(std::string("(0,100) (100,200) "), segmentation.boundsAsString());
		}
		void testInsert_beyondLimit()
		{
			DiscontinuousSegmentation segmentation(200.0);
			try
			{
				segmentation.insert(400);
				CPPUNIT_FAIL("Should have thrown an exception");
			}
			catch (DiscontinuousSegmentation::InsertedOutOfBounds & e)
			{
				CPPUNIT_ASSERT_EQUAL(std::string("Segmentation point inserted out of limits"), std::string(e.what()));
			}
			CPPUNIT_ASSERT_EQUAL(std::string(""), segmentation.boundsAsString());
		}
		void testInsert_behindLimit()
		{
			DiscontinuousSegmentation segmentation(200.0);
			try
			{
				segmentation.insert(-0.1);
				CPPUNIT_FAIL("Should have thrown an exception");
			}
			catch (DiscontinuousSegmentation::InsertedOutOfBounds & e)
			{
				CPPUNIT_ASSERT_EQUAL(std::string("Segmentation point inserted out of limits"), std::string(e.what()));
			}

			CPPUNIT_ASSERT_EQUAL(std::string(""), segmentation.boundsAsString());
		}
		void testPickOffset_withExactValue()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(50);
			segmentation.insert(100);
			segmentation.insert(150);

			unsigned position= segmentation.pickOffset(100,0.5);
			CPPUNIT_ASSERT_EQUAL(1u, position);
		}
		void testPickOffset_withNonMatchingValue()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(50);
			segmentation.insert(100);
			segmentation.insert(150);

			unsigned position= segmentation.pickOffset(125,0.5);
			CPPUNIT_ASSERT_EQUAL(4u, position);
		}
		void testPickOffset_withinTolerance()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(50);
			segmentation.insert(100);
			segmentation.insert(150);

			unsigned position= segmentation.pickOffset(100.2,0.5);
			CPPUNIT_ASSERT_EQUAL(1u, position);
		}
		void testPickOffset_withSeveralPointsWithinTolerance()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			unsigned position= segmentation.pickOffset(101,20);
			CPPUNIT_ASSERT_EQUAL(1u, position);
		}
		void testPickOffset_outsideTheRange()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			unsigned position= segmentation.pickOffset(221,20);
			CPPUNIT_ASSERT_EQUAL(4u, position);
		}
		void testPickOnset_withExactValue()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(50);
			segmentation.insert(100);
			segmentation.insert(150);

			unsigned position= segmentation.pickOnset(100,0.5);
			CPPUNIT_ASSERT_EQUAL(2u, position);
		}
		void testPickOnset_withNonMatchingValue()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(50);
			segmentation.insert(100);
			segmentation.insert(150);

			unsigned position= segmentation.pickOnset(125,0.5);
			CPPUNIT_ASSERT_EQUAL(4u, position);
		}
		void testPickOnset_withinTolerance()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(50);
			segmentation.insert(100);
			segmentation.insert(150);

			unsigned position= segmentation.pickOnset(100.2,0.5);
			CPPUNIT_ASSERT_EQUAL(2u, position);
		}
		void testPickOnset_withSeveralPointsWithinTolerance()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			unsigned position= segmentation.pickOnset(101,20);
			CPPUNIT_ASSERT_EQUAL(2u, position);
		}
		void testPickOnset_outsideTheRange()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			unsigned position= segmentation.pickOnset(221,20);
			CPPUNIT_ASSERT_EQUAL(4u, position);
		}
		void testPickSegmentBody()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			unsigned position= segmentation.pickSegmentBody(102);
			CPPUNIT_ASSERT_EQUAL(2u, position);
		}
		void testPickSegmentBody_behindRange()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			unsigned position= segmentation.pickSegmentBody(-1);
			CPPUNIT_ASSERT_EQUAL(4u, position);
		}
		void testPickSegmentBody_afterRange()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			unsigned position= segmentation.pickSegmentBody(201);
			CPPUNIT_ASSERT_EQUAL(4u, position);
		}
		void testPickSegmentBody_atABound()
		{
			// This behaviour is not specified just to know what happens
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			unsigned position= segmentation.pickSegmentBody(100);
			CPPUNIT_ASSERT_EQUAL(1u, position);
		}
		void testPickSegmentBody_atZeroBound()
		{
			// This behaviour is not specified just to know what happens
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			unsigned position= segmentation.pickSegmentBody(0);
			CPPUNIT_ASSERT_EQUAL(0u, position);
		}
		void testSelection()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.select(2);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) +(100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testSelection_multiple()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.select(2);
			segmentation.select(1);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) +(90,100) +(100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDeselection()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.select(2);
			segmentation.select(1);
			segmentation.deselect(2);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) +(90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testInsert_movesSelection()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.select(1);
			segmentation.select(2);

			segmentation.insert(95);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) +(90,95) (95,100) +(100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testClearSelection()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.select(1);
			segmentation.select(2);

			segmentation.clearSelection();

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOffset_ofLastBoundHasEffect()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOffset(3,190);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,190) "), segmentation.boundsAsString());
		}
		void testDragOnset_ofFirstBoundHasEffect()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOnset(0,10);

			CPPUNIT_ASSERT_EQUAL(std::string("(10,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOffset()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOffset(1,95);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,95) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOnset()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOnset(2,105);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (105,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOnset_thatDoesNotExist()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOnset(4,150);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOffset_thatDoesNotExist()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOffset(4,150);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOnset_toLeftLimitedByPreviousOffset()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOnset(2,95);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOnset_toLeftLimitedByZeroWhenFirstOne()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOnset(0,-10);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOnset_toRightLimitedByOwnOffset()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOnset(2,115);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (110,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOffset_toRightLimitedByNextOnset()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOffset(2,115);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOffset_toRightLimitedByMaxWhenLastOne()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOffset(3,115);
			segmentation.dragOffset(3,205);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOffset_toLeftLimitedByOwnOnset()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOffset(2,95);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,100) (110,200) "), segmentation.boundsAsString());
		}
		void testRemove()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());

			segmentation.remove(2);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (110,200) "), segmentation.boundsAsString());
		}
		void testRemove_movesSelection()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.select(1);
			segmentation.select(3);

			segmentation.remove(2);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) +(90,100) +(110,200) "), segmentation.boundsAsString());
		}
		void testRemove_removesRemovedSelection()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.select(2);

			segmentation.remove(2);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (110,200) "), segmentation.boundsAsString());
		}
		void testRemove_firstSegment()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.select(1);
			segmentation.select(3);

			segmentation.remove(0);

			CPPUNIT_ASSERT_EQUAL(std::string("+(90,100) (100,110) +(110,200) "), segmentation.boundsAsString());
		}
		void testRemove_lastSegmentExpandsPrevious()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.select(1);
			segmentation.select(3);

			segmentation.remove(3);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) +(90,100) (100,110) "), segmentation.boundsAsString());
		}
		void testRemove_singleSegmentHasEffect()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);

			segmentation.remove(0);

			CPPUNIT_ASSERT_EQUAL(std::string(""), segmentation.boundsAsString());
		}
		void testConstructor_putsCurrentAtZero()
		{
			DiscontinuousSegmentation segmentation(200.0);
			CPPUNIT_ASSERT_EQUAL(0u, segmentation.current());
		}
		void testInsert_keepsCurrentAtZero()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			CPPUNIT_ASSERT_EQUAL(0u, segmentation.current());
		}
		void testCurrent_changesCurrent()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);
			segmentation.current(2);
			CPPUNIT_ASSERT_EQUAL(2u, segmentation.current());
		}
		void testCurrent_aboveNSegmentsHasNoEffect()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);
			segmentation.current(2);
			segmentation.current(4);
			CPPUNIT_ASSERT_EQUAL(2u, segmentation.current());
		}
		void testInsert_beforeCurrentCorrectsIt()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);
			segmentation.current(2);
			segmentation.insert(95);
			CPPUNIT_ASSERT_EQUAL(3u, segmentation.current());
		}
		void testRemove_beforeCurrentCorrectsIt()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);
			segmentation.current(2);
			segmentation.remove(1);
			CPPUNIT_ASSERT_EQUAL(1u, segmentation.current());
		}
		void testRemove_afterCurrentKeepsIt()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);
			segmentation.current(2);
			segmentation.remove(3);
			CPPUNIT_ASSERT_EQUAL(2u, segmentation.current());
		}
		void testRemove_justTheCurrentChangesIt()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);
			segmentation.current(2);
			segmentation.remove(2);
			CPPUNIT_ASSERT_EQUAL(1u, segmentation.current());
		}
		void testRemove_justTheCurrentButItIsZeroKeepsAtZero()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);
			segmentation.current(0);
			segmentation.remove(0);
			CPPUNIT_ASSERT_EQUAL(0u, segmentation.current());
		}
		void testInsert_onTheMiddleOfAFrame()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(100);
			segmentation.insert(150);
			segmentation.remove(1);
			unsigned pos = segmentation.insert(130);
			CPPUNIT_ASSERT_EQUAL(1u, pos);
			CPPUNIT_ASSERT_EQUAL(std::string("(100,130) (130,150) "), segmentation.boundsAsString());
		}
		void testInsert_onGapAtTheBegining()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(100);
			segmentation.insert(150);
			segmentation.remove(1);
			unsigned pos = segmentation.insert(0);
			CPPUNIT_ASSERT_EQUAL(0u, pos);
			CPPUNIT_ASSERT_EQUAL(std::string("(0,100) (100,150) "), segmentation.boundsAsString());
		}
		void testInsert_onGapAtTheBeginingMovesCurrentWhenAfter()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(100);
			segmentation.insert(150);
			segmentation.remove(1);
			segmentation.current(0);
			unsigned pos = segmentation.insert(0);
			CPPUNIT_ASSERT_EQUAL(1u, segmentation.current());
		}
	};


}

