#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Segmentation.hxx"
#include <sstream>

using CLAM::Segmentation;

namespace CLAMTest
{
	class SegmentationTest;

	CPPUNIT_TEST_SUITE_REGISTRATION( SegmentationTest );

	class SegmentationTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( SegmentationTest );
		CPPUNIT_TEST( testConstructor );
		CPPUNIT_TEST( testInsert_aPoint );
		CPPUNIT_TEST( testInsert_inTheMiddle );
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
		CPPUNIT_TEST( testDragOffset );
		CPPUNIT_TEST( testDragOffset_beyondNextBound );
		CPPUNIT_TEST( testDragOffset_beyondPreviousBound );
		CPPUNIT_TEST( testDragOffset_ofLastBoundHasNoEffect );
		CPPUNIT_TEST( testDragOffset_ofPreLastSegment );
		CPPUNIT_TEST( testDragOffset_ofFirstBoundLimitIsZero );
		CPPUNIT_TEST( testDragOnset );
		CPPUNIT_TEST( testDragOnset_beyondNextBound );
		CPPUNIT_TEST( testDragOnset_beyondPreviousBound );
		CPPUNIT_TEST( testDragOnset_ofFirstBoundHasNoEffect );
		CPPUNIT_TEST( testDragOnset_ofLastSegment );
		CPPUNIT_TEST( testDragOnset_ofLastBoundLimitIsLength );
		CPPUNIT_TEST( testRemove );
		CPPUNIT_TEST( testRemove_movesSelection );
		CPPUNIT_TEST( testRemove_removesRemovedSelection );
		CPPUNIT_TEST( testRemove_firstSegment );
		CPPUNIT_TEST( testRemove_lastSegmentExpandsPrevious );
		CPPUNIT_TEST( testRemove_singleSegmentHasNoEffect );
		CPPUNIT_TEST( testConstructor_putsCurrentAtZero );
		CPPUNIT_TEST( testInsert_keepsCurrentAtZero );
		CPPUNIT_TEST( testCurrent_changesCurrent );
		CPPUNIT_TEST( testCurrent_aboveNSegmentsHasNoEffect );
		CPPUNIT_TEST( testInsert_beforeCurrentCorrectsIt );
		CPPUNIT_TEST( testRemove_beforeCurrentCorrectsIt );
		CPPUNIT_TEST( testRemove_afterCurrentKeepsIt );
		CPPUNIT_TEST( testRemove_justTheCurrentChangesIt );
		CPPUNIT_TEST( testRemove_justTheCurrentButItIsZeroKeepsAtZero );
		CPPUNIT_TEST_SUITE_END();

	public:

		void testConstructor()
		{
			Segmentation segmentation(200.0);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,200) "), segmentation.boundsAsString());
		}
		void testInsert_aPoint()
		{
			Segmentation segmentation(200.0);
			unsigned pos = segmentation.insert(100);

			CPPUNIT_ASSERT_EQUAL(1u, pos);
			CPPUNIT_ASSERT_EQUAL(std::string("(0,100) (100,200) "), segmentation.boundsAsString());
		}
		void testInsert_inTheMiddle()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(50);
			segmentation.insert(150);
			unsigned pos = segmentation.insert(100);

			CPPUNIT_ASSERT_EQUAL(2u, pos);
			CPPUNIT_ASSERT_EQUAL(std::string("(0,50) (50,100) (100,150) (150,200) "), segmentation.boundsAsString());
		}
		void testInsert_beyondLimit()
		{
			Segmentation segmentation(200.0);
			try
			{
				segmentation.insert(400);
				CPPUNIT_FAIL("Should have thrown an exception");
			}
			catch (Segmentation::InsertedOutOfBounds & e)
			{
				CPPUNIT_ASSERT_EQUAL(std::string("Segmentation point inserted out of limits"), std::string(e.what()));
			}
			CPPUNIT_ASSERT_EQUAL(std::string("(0,200) "), segmentation.boundsAsString());
		}
		void testInsert_behindLimit()
		{
			Segmentation segmentation(200.0);
			try
			{
				segmentation.insert(-0.1);
				CPPUNIT_FAIL("Should have thrown an exception");
			}
			catch (Segmentation::InsertedOutOfBounds & e)
			{
				CPPUNIT_ASSERT_EQUAL(std::string("Segmentation point inserted out of limits"), std::string(e.what()));
			}

			CPPUNIT_ASSERT_EQUAL(std::string("(0,200) "), segmentation.boundsAsString());
		}
		void testPickOffset_withExactValue()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(50);
			segmentation.insert(100);
			segmentation.insert(150);

			unsigned position= segmentation.pickOffset(100,0.5);
			CPPUNIT_ASSERT_EQUAL(1u, position);
		}
		void testPickOffset_withNonMatchingValue()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(50);
			segmentation.insert(100);
			segmentation.insert(150);

			unsigned position= segmentation.pickOffset(125,0.5);
			CPPUNIT_ASSERT_EQUAL(4u, position);
		}
		void testPickOffset_withinTolerance()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(50);
			segmentation.insert(100);
			segmentation.insert(150);

			unsigned position= segmentation.pickOffset(100.2,0.5);
			CPPUNIT_ASSERT_EQUAL(1u, position);
		}
		void testPickOffset_withSeveralPointsWithinTolerance()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			unsigned position= segmentation.pickOffset(101,20);
			CPPUNIT_ASSERT_EQUAL(1u, position);
		}
		void testPickOffset_outsideTheRange()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			unsigned position= segmentation.pickOffset(221,20);
			CPPUNIT_ASSERT_EQUAL(4u, position);
		}
		void testPickOnset_withExactValue()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(50);
			segmentation.insert(100);
			segmentation.insert(150);

			unsigned position= segmentation.pickOnset(100,0.5);
			CPPUNIT_ASSERT_EQUAL(2u, position);
		}
		void testPickOnset_withNonMatchingValue()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(50);
			segmentation.insert(100);
			segmentation.insert(150);

			unsigned position= segmentation.pickOnset(125,0.5);
			CPPUNIT_ASSERT_EQUAL(4u, position);
		}
		void testPickOnset_withinTolerance()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(50);
			segmentation.insert(100);
			segmentation.insert(150);

			unsigned position= segmentation.pickOnset(100.2,0.5);
			CPPUNIT_ASSERT_EQUAL(2u, position);
		}
		void testPickOnset_withSeveralPointsWithinTolerance()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			unsigned position= segmentation.pickOnset(101,20);
			CPPUNIT_ASSERT_EQUAL(2u, position);
		}
		void testPickOnset_outsideTheRange()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			unsigned position= segmentation.pickOnset(221,20);
			CPPUNIT_ASSERT_EQUAL(4u, position);
		}
		void testPickSegmentBody()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			unsigned position= segmentation.pickSegmentBody(102);
			CPPUNIT_ASSERT_EQUAL(2u, position);
		}
		void testPickSegmentBody_behindRange()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			unsigned position= segmentation.pickSegmentBody(-1);
			CPPUNIT_ASSERT_EQUAL(4u, position);
		}
		void testPickSegmentBody_afterRange()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			unsigned position= segmentation.pickSegmentBody(201);
			CPPUNIT_ASSERT_EQUAL(4u, position);
		}
		void testPickSegmentBody_atABound()
		{
			// This behaviour is not specified just to know what happens
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			unsigned position= segmentation.pickSegmentBody(100);
			CPPUNIT_ASSERT_EQUAL(1u, position);
		}
		void testPickSegmentBody_atZeroBound()
		{
			// This behaviour is not specified just to know what happens
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			unsigned position= segmentation.pickSegmentBody(0);
			CPPUNIT_ASSERT_EQUAL(0u, position);
		}
		void testSelection()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.select(2);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) +(100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testSelection_multiple()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.select(2);
			segmentation.select(1);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) +(90,100) +(100,110) (110,200) "), segmentation.boundsAsString());
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

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) +(90,100) (100,110) (110,200) "), segmentation.boundsAsString());
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

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) +(90,95) (95,100) +(100,110) (110,200) "), segmentation.boundsAsString());
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

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOffset()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOffset(1,105);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,105) (105,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOffset_beyondNextBound()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOffset(1,115);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,110) (110,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOffset_beyondPreviousBound()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOffset(1,85);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,90) (90,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOffset_ofLastBoundHasNoEffect()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOffset(4,190);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOffset_ofPreLastSegment()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOffset(3,114);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOffset_ofFirstBoundLimitIsZero()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOffset(0,-12);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,0) (0,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOnset()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOnset(2,105);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,105) (105,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOnset_beyondNextBound()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOnset(2,115);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,110) (110,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOnset_beyondPreviousBound()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOnset(2,85);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,90) (90,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOnset_ofFirstBoundHasNoEffect()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOnset(0,10);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOnset_ofLastSegment()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOnset(3,114);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,114) (114,200) "), segmentation.boundsAsString());
		}
		void testDragOnset_ofLastBoundLimitIsLength()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.dragOnset(3,210);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,200) (200,200) "), segmentation.boundsAsString());
		}
		void testRemove()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());

			segmentation.remove(2);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,110) (110,200) "), segmentation.boundsAsString());
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

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) +(90,110) +(110,200) "), segmentation.boundsAsString());
		}
		void testRemove_removesRemovedSelection()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);

			segmentation.select(2);

			segmentation.remove(2);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,110) (110,200) "), segmentation.boundsAsString());
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

			CPPUNIT_ASSERT_EQUAL(std::string("+(0,100) (100,110) +(110,200) "), segmentation.boundsAsString());
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

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) +(90,100) (100,200) "), segmentation.boundsAsString());
		}
		void testRemove_singleSegmentHasNoEffect()
		{
			Segmentation segmentation(200.0);

			segmentation.remove(0);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,200) "), segmentation.boundsAsString());
		}
		void testConstructor_putsCurrentAtZero()
		{
			Segmentation segmentation(200.0);
			CPPUNIT_ASSERT_EQUAL(0u, segmentation.current());
		}
		void testInsert_keepsCurrentAtZero()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			CPPUNIT_ASSERT_EQUAL(0u, segmentation.current());
		}
		void testCurrent_changesCurrent()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);
			segmentation.current(2);
			CPPUNIT_ASSERT_EQUAL(2u, segmentation.current());
		}
		void testCurrent_aboveNSegmentsHasNoEffect()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);
			segmentation.current(2);
			segmentation.current(4);
			CPPUNIT_ASSERT_EQUAL(2u, segmentation.current());
		}
		void testInsert_beforeCurrentCorrectsIt()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);
			segmentation.current(2);
			segmentation.insert(95);
			CPPUNIT_ASSERT_EQUAL(3u, segmentation.current());
		}
		void testRemove_beforeCurrentCorrectsIt()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);
			segmentation.current(2);
			segmentation.remove(1);
			CPPUNIT_ASSERT_EQUAL(1u, segmentation.current());
		}
		void testRemove_afterCurrentKeepsIt()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);
			segmentation.current(2);
			segmentation.remove(3);
			CPPUNIT_ASSERT_EQUAL(2u, segmentation.current());
		}
		void testRemove_justTheCurrentChangesIt()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);
			segmentation.current(2);
			segmentation.remove(2);
			CPPUNIT_ASSERT_EQUAL(1u, segmentation.current());
		}
		void testRemove_justTheCurrentButItIsZeroKeepsAtZero()
		{
			Segmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(110);
			segmentation.current(0);
			segmentation.remove(0);
			CPPUNIT_ASSERT_EQUAL(0u, segmentation.current());
		}
	};


}

