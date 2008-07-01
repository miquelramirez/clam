#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "DiscontinuousSegmentation.hxx"
#include "XMLTestHelper.hxx"
#include <sstream>

using CLAM::DiscontinuousSegmentation;
using CLAM::XmlStorage;

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
		CPPUNIT_TEST( testInit_withASortedListsOfPoints );
		CPPUNIT_TEST( testInit_withAnOverlap );
		CPPUNIT_TEST( testInit_withUnsortedOnsetOffsetPair );
		CPPUNIT_TEST( testInit_withOutOfBoundsOffset );
		CPPUNIT_TEST( testInit_withOddNumberOfBounds );
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
		CPPUNIT_TEST( testPickSegmentBody_atGap );
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
		CPPUNIT_TEST( testInsert_onGapInBetweenSegments );
		CPPUNIT_TEST( testStoreOn);	// TODO: This test crashes in some boxes
		CPPUNIT_TEST( testtakeArray);
		CPPUNIT_TEST(testLoadFromWithoutMaxPos);
		CPPUNIT_TEST(testLoadFromWithMaxPos);
		CPPUNIT_TEST(testStoreOn_withinAnArray);
		CPPUNIT_TEST(testLoadFromWithoutMaxPos_withinAnArray);
		CPPUNIT_TEST(testLoadFromWithMaxPos_withinAnArray);
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
		void testInit_withASortedListsOfPoints()
		{
			const TData bounds[] = {50,90,100,150};

			DiscontinuousSegmentation segmentation(200.0, bounds, bounds+4);

			CPPUNIT_ASSERT_EQUAL(std::string("(50,90) (100,150) "), segmentation.boundsAsString());
		}
		void testInit_withAnOverlap()
		{
			try
			{
				const TData bounds[] = {50,100,90,150};
				DiscontinuousSegmentation segmentation(200.0, bounds, bounds+4);
				CPPUNIT_FAIL("Should have thrown an exception");
			}
			catch (DiscontinuousSegmentation::MissplacedOnset & e)
			{
				CPPUNIT_ASSERT_EQUAL(std::string("Segment 1 starts at 90 overlapping previous segment which ends at 100"), std::string(e.what()));
			}
		}
		void testInit_withUnsortedOnsetOffsetPair()
		{
			try
			{
				const TData bounds[] = {50,90,150,100};
				DiscontinuousSegmentation segmentation(200.0, bounds, bounds+4);
				CPPUNIT_FAIL("Should have thrown an exception");
			}
			catch (DiscontinuousSegmentation::MissplacedOffset & e)
			{
				CPPUNIT_ASSERT_EQUAL(std::string("Segment 1 starts at 150 but ends before that, at 100"), std::string(e.what()));
			}
		}
		void testInit_withOutOfBoundsOffset()
		{
			try
			{
				const TData bounds[] = {50,90,150,201};
				DiscontinuousSegmentation segmentation(200.0, bounds, bounds+4);
				CPPUNIT_FAIL("Should have thrown an exception");
			}
			catch (DiscontinuousSegmentation::InsertedOutOfBounds & e)
			{
				CPPUNIT_ASSERT_EQUAL(std::string("Segmentation point inserted out of limits"), std::string(e.what()));
			}
		}
		void testInit_withOddNumberOfBounds()
		{
			try
			{
				const TData bounds[] = {50,90,100,150};
				DiscontinuousSegmentation segmentation(200.0, bounds, bounds+3);
				CPPUNIT_FAIL("Should have thrown an exception");
			}
			catch (DiscontinuousSegmentation::OffsetMissing & e)
			{
				CPPUNIT_ASSERT_EQUAL(std::string("Odd number of segmentation points, every segment beggining must be followed by its ending"), std::string(e.what()));
			}
		}
		void testPickOffset_withExactValue()
		{
			const TData divisions[]={0,50,50,100,100,150,150,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			unsigned position= segmentation.pickOffset(100,0.5);

			CPPUNIT_ASSERT_EQUAL(1u, position);
		}
		void testPickOffset_withNonMatchingValue()
		{
			const TData divisions[]={0,50,50,100,100,150,150,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			unsigned position= segmentation.pickOffset(125,0.5);

			CPPUNIT_ASSERT_EQUAL(4u, position);
		}
		void testPickOffset_withinTolerance()
		{
			const TData divisions[]={0,50,50,100,100,150,150,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			unsigned position= segmentation.pickOffset(100.2,0.5);

			CPPUNIT_ASSERT_EQUAL(1u, position);
		}
		void testPickOffset_withSeveralPointsWithinTolerance()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			unsigned position= segmentation.pickOffset(101,20);

			CPPUNIT_ASSERT_EQUAL(1u, position);
		}
		void testPickOffset_outsideTheRange()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			unsigned position= segmentation.pickOffset(221,20);

			CPPUNIT_ASSERT_EQUAL(4u, position);
		}
		void testPickOnset_withExactValue()
		{
			const TData divisions[]={0,50,50,100,100,150,150,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			unsigned position= segmentation.pickOnset(100,0.5);

			CPPUNIT_ASSERT_EQUAL(2u, position);
		}
		void testPickOnset_withNonMatchingValue()
		{
			const TData divisions[]={0,50,50,100,100,150,150,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			unsigned position= segmentation.pickOnset(125,0.5);

			CPPUNIT_ASSERT_EQUAL(4u, position);
		}
		void testPickOnset_withinTolerance()
		{
			const TData divisions[]={0,50,50,100,100,150,150,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			unsigned position= segmentation.pickOnset(100.2,0.5);

			CPPUNIT_ASSERT_EQUAL(2u, position);
		}
		void testPickOnset_withSeveralPointsWithinTolerance()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			unsigned position= segmentation.pickOnset(101,20);

			CPPUNIT_ASSERT_EQUAL(2u, position);
		}
		void testPickOnset_outsideTheRange()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			unsigned position= segmentation.pickOnset(221,20);

			CPPUNIT_ASSERT_EQUAL(4u, position);
		}
		void testPickSegmentBody()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			unsigned position= segmentation.pickSegmentBody(102);

			CPPUNIT_ASSERT_EQUAL(2u, position);
		}
		void testPickSegmentBody_behindRange()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			unsigned position= segmentation.pickSegmentBody(-1);

			CPPUNIT_ASSERT_EQUAL(4u, position);
		}
		void testPickSegmentBody_afterRange()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			unsigned position= segmentation.pickSegmentBody(201);

			CPPUNIT_ASSERT_EQUAL(4u, position);
		}
		void testPickSegmentBody_atABound()
		{
			// This behaviour is not specified just to know what happens
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			unsigned position= segmentation.pickSegmentBody(100);

			CPPUNIT_ASSERT_EQUAL(1u, position);
		}
		void testPickSegmentBody_atZeroBound()
		{
			// This behaviour is not specified just to know what happens
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			unsigned position= segmentation.pickSegmentBody(0);

			CPPUNIT_ASSERT_EQUAL(0u, position);
		}
		void testPickSegmentBody_atGap()
		{
			const TData divisions[]={90,100,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+4);

			unsigned position= segmentation.pickSegmentBody(105);

			CPPUNIT_ASSERT_EQUAL(2u, position);
		}
		void testSelection()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.select(2);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) +(100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testSelection_multiple()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.select(2);
			segmentation.select(1);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) +(90,100) +(100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDeselection()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.select(2);
			segmentation.select(1);
			segmentation.deselect(2);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) +(90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testInsert_movesSelection()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.select(1);
			segmentation.select(2);

			segmentation.insert(95);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) +(90,95) (95,100) +(100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testClearSelection()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.select(1);
			segmentation.select(2);

			segmentation.clearSelection();

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOffset_ofLastBoundHasEffect()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.dragOffset(3,190);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,190) "), segmentation.boundsAsString());
		}
		void testDragOnset_ofFirstBoundHasEffect()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.dragOnset(0,10);

			CPPUNIT_ASSERT_EQUAL(std::string("(10,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOffset()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.dragOffset(1,95);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,95) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOnset()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.dragOnset(2,105);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (105,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOnset_thatDoesNotExist()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.dragOnset(4,150);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOffset_thatDoesNotExist()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.dragOffset(4,150);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOnset_toLeftLimitedByPreviousOffset()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.dragOnset(2,95);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOnset_toLeftLimitedByZeroWhenFirstOne()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.dragOnset(0,-10);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOnset_toRightLimitedByOwnOffset()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.dragOnset(2,115);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (110,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOffset_toRightLimitedByNextOnset()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.dragOffset(2,115);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOffset_toRightLimitedByMaxWhenLastOne()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.dragOffset(3,115);
			segmentation.dragOffset(3,205);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());
		}
		void testDragOffset_toLeftLimitedByOwnOnset()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.dragOffset(2,95);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,100) (110,200) "), segmentation.boundsAsString());
		}
		void testRemove()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (100,110) (110,200) "), segmentation.boundsAsString());

			segmentation.remove(2);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (110,200) "), segmentation.boundsAsString());
		}
		void testRemove_movesSelection()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.select(1);
			segmentation.select(3);

			segmentation.remove(2);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) +(90,100) +(110,200) "), segmentation.boundsAsString());
		}
		void testRemove_removesRemovedSelection()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.select(2);

			segmentation.remove(2);

			CPPUNIT_ASSERT_EQUAL(std::string("(0,90) (90,100) (110,200) "), segmentation.boundsAsString());
		}
		void testRemove_firstSegment()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.select(1);
			segmentation.select(3);

			segmentation.remove(0);

			CPPUNIT_ASSERT_EQUAL(std::string("+(90,100) (100,110) +(110,200) "), segmentation.boundsAsString());
		}
		void testRemove_lastSegmentExpandsPrevious()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

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
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

			segmentation.current(2);

			CPPUNIT_ASSERT_EQUAL(2u, segmentation.current());
		}
		void testCurrent_aboveNSegmentsHasNoEffect()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);
			segmentation.current(2);

			segmentation.current(4);

			CPPUNIT_ASSERT_EQUAL(2u, segmentation.current());
		}
		void testInsert_beforeCurrentCorrectsIt()
		{
			const TData divisions[]={0,90,90,100,100,110,110,200};
			DiscontinuousSegmentation segmentation(200.0, divisions, divisions+8);

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
			segmentation.insert(0);
			CPPUNIT_ASSERT_EQUAL(1u, segmentation.current());
		}
		void testInsert_onGapInBetweenSegments()
		{
			DiscontinuousSegmentation segmentation(200.0);
			segmentation.insert(90);
			segmentation.insert(100);
			segmentation.insert(150);
			segmentation.remove(1);
			segmentation.current(0);
			unsigned pos = segmentation.insert(120);
			CPPUNIT_ASSERT_EQUAL(1u, pos);
			CPPUNIT_ASSERT_EQUAL(std::string("(90,100) (120,150) (150,200) "), segmentation.boundsAsString());
		}

		void testStoreOn()
		{
			const TData bounds[]={90, 100, 110, 120};
			DiscontinuousSegmentation segmentationEven(200, bounds, bounds+4);
			std::ostringstream stream;
			XmlStorage::Dump(segmentationEven, "Segmentation", stream);
			CLAMTEST_ASSERT_XML_EQUAL(
				"<Segmentation max=\"200\" size=\"4\">90 100 110 120</Segmentation>"
				, stream.str());
		}

		void testtakeArray()
		{
			const TData bounds[]={90, 100, 110, 120};
			DiscontinuousSegmentation segmentationEven(200);
			segmentationEven.takeArray(bounds, bounds+4);
			CPPUNIT_ASSERT_EQUAL(std::string("(90,100) (110,120) "), segmentationEven.boundsAsString());
		}
		void testLoadFromWithoutMaxPos()
		{
			DiscontinuousSegmentation segmentation(200);
			std::istringstream stream("<Segmentation size=\"4\">90 100 110 120</Segmentation>");
			XmlStorage::Restore(segmentation, stream);
			CPPUNIT_ASSERT_EQUAL(std::string("(90,100) (110,120) "), segmentation.boundsAsString());
		}
		void testLoadFromWithMaxPos()
		{
			DiscontinuousSegmentation segmentation;
			std::istringstream stream("<Segmentation max=\"200\" size=\"4\">90 100 110 120</Segmentation>");
			XmlStorage::Restore(segmentation, stream);
			CPPUNIT_ASSERT_EQUAL(std::string("(90,100) (110,120) "), segmentation.boundsAsString());
		}
		void testStoreOn_withinAnArray()
		{
			const TData onsets1[]={90, 100, 110, 120};
			const TData onsets2[]={9, 10, 11, 12, 16, 18};
			CLAM::Array<DiscontinuousSegmentation> segmentations(2);
			segmentations[0].maxPosition(200);
			segmentations[0].takeArray(onsets1, onsets1+4);
			segmentations[1].maxPosition(20);
			segmentations[1].takeArray(onsets2, onsets2+6);
			std::ostringstream stream;
			XmlStorage::Dump(segmentations, "Segmentations", stream);
			CLAMTEST_ASSERT_XML_EQUAL(
				"<Segmentations>"
					"<DiscontinuousSegmentation max=\"200\" size=\"4\">90 100 110 120</DiscontinuousSegmentation>"
					"<DiscontinuousSegmentation max=\"20\" size=\"6\">9 10 11 12 16 18</DiscontinuousSegmentation>"
				"</Segmentations>"
				, stream.str());
		}
		void testLoadFromWithoutMaxPos_withinAnArray()
		{
			CLAM::Array<DiscontinuousSegmentation> segmentations;
			std::istringstream stream(
				"<Segmentations>"
					"<DiscontinuousSegmentation size=\"4\">90 100 110 120</DiscontinuousSegmentation>"
					"<DiscontinuousSegmentation size=\"6\">9 10 11 12 16 18</DiscontinuousSegmentation>"
				"</Segmentations>");
			XmlStorage::Restore(segmentations, stream);
			CPPUNIT_ASSERT_EQUAL(std::string("(90,100) (110,120) "), segmentations[0].boundsAsString());
			CPPUNIT_ASSERT_EQUAL(std::string("(9,10) (11,12) (16,18) "), segmentations[1].boundsAsString());
		}
		void testLoadFromWithMaxPos_withinAnArray()
		{
			CLAM::Array<DiscontinuousSegmentation> segmentations;
			std::istringstream stream(
				"<Segmentations>"
					"<DiscontinuousSegmentation max=\"200\" size=\"4\">90 100 110 120</DiscontinuousSegmentation>"
					"<DiscontinuousSegmentation max=\"20\" size=\"6\">9 10 11 12 16 18</DiscontinuousSegmentation>"
				"</Segmentations>");
			XmlStorage::Restore(segmentations, stream);
			CPPUNIT_ASSERT_EQUAL(std::string("(90,100) (110,120) "), segmentations[0].boundsAsString());
			CPPUNIT_ASSERT_EQUAL(std::string("(9,10) (11,12) (16,18) "), segmentations[1].boundsAsString());
		}
	};


}

