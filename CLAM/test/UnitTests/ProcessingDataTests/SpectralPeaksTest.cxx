#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "SpectralPeakArray.hxx" // CLAM

namespace CLAMTest { class SpectralPeaksTest; }


CPPUNIT_TEST_SUITE_REGISTRATION( CLAMTest::SpectralPeaksTest );

class CLAMTest::SpectralPeaksTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( SpectralPeaksTest );

	CPPUNIT_TEST(test_asString_withEmptyArray);
	CPPUNIT_TEST(test_asString_withSingleElementArray);
	CPPUNIT_TEST(test_asString_withManyElementArray);
	CPPUNIT_TEST(test_asString_withEmptyPeaks);
	CPPUNIT_TEST(test_asString_withPeaks);
	CPPUNIT_TEST(test_peakAddition_whenDisjoint);
	CPPUNIT_TEST(test_peakAddition_whenInterleaved);
	CPPUNIT_TEST(test_peakAddition_whenCoincident);

	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	typedef CLAM::SpectralPeakArray SpectralPeaks;

	template <class T> std::string asString(const CLAM::Array<T> & array)
	{
		std::ostringstream os;
		os << "[";
		for (unsigned i=0; i<array.Size(); i++)
			os << " " << array[i];
		os << " ]";
		return os.str();
	}

	std::string asString(const SpectralPeaks & peaks)
	{
		std::ostringstream os;
		if (peaks.HasIndexArray())
			os << "Indices: " << asString(peaks.GetIndexArray()) << std::endl;
		if (peaks.HasFreqBuffer())
			os << "Frequencies: " << asString(peaks.GetFreqBuffer()) << std::endl;
		if (peaks.HasMagBuffer())
			os << "Magnitudes: " << asString(peaks.GetMagBuffer()) << std::endl;
		if (peaks.HasPhaseBuffer())
			os << "Phases: " << asString(peaks.GetPhaseBuffer()) << std::endl;
		return os.str();
	}

	SpectralPeaks createPeaks()
	{
		SpectralPeaks peaks;
		peaks.AddAll();
		peaks.UpdateData();
		return peaks;
	}

	void addPeak(SpectralPeaks & peaks,
		unsigned index,
		float frequency,
		float magnitude,
		float phase)
	{
		CLAM::SpectralPeak peak;
		peak.AddAll();
		peak.UpdateData();
		peak.SetFreq(frequency);
		peak.SetMag(magnitude);
		peak.SetPhase(phase);
		peaks.AddSpectralPeak(peak, true, index);
	}

	void test_asString_withEmptyArray()
	{
		CLAM::Array<float> data;
		CPPUNIT_ASSERT_EQUAL(
			std::string(
			"[ ]"
			), asString(data));
	}
	void test_asString_withSingleElementArray()
	{
		CLAM::Array<float> data;
		data.AddElem(4.2);
		CPPUNIT_ASSERT_EQUAL(
			std::string(
			"[ 4.2 ]"
			), asString(data));
	}

	void test_asString_withManyElementArray()
	{
		CLAM::Array<float> data;
		data.AddElem(4.2);
		data.AddElem(5.6);
		CPPUNIT_ASSERT_EQUAL(
			std::string(
			"[ 4.2 5.6 ]"
			), asString(data));
	}

	void test_asString_withEmptyPeaks()
	{
		SpectralPeaks peaks;
		peaks.AddAll();
		peaks.UpdateData();
		CPPUNIT_ASSERT_EQUAL(
			std::string(
			"Indices: [ ]\n"
			"Frequencies: [ ]\n"
			"Magnitudes: [ ]\n"
			"Phases: [ ]\n"
			), asString(peaks));
	}
	void test_asString_withPeaks()
	{
		SpectralPeaks peaks = createPeaks();
		addPeak(peaks, 1, 10, 1000, .1);
		addPeak(peaks, 2, 20, 2000, .2);
		CPPUNIT_ASSERT_EQUAL(
			std::string(
			"Indices: [ 1 2 ]\n"
			"Frequencies: [ 10 20 ]\n"
			"Magnitudes: [ 1000 2000 ]\n"
			"Phases: [ 0.1 0.2 ]\n"
			), asString(peaks));
	}
	void test_peakAddition_whenDisjoint()
	{
		SpectralPeaks peaks1 = createPeaks();
		addPeak(peaks1, 1, 10, 1000, .1);
		addPeak(peaks1, 2, 20, 2000, .2);

		SpectralPeaks peaks2 = createPeaks();
		addPeak(peaks2, 3, 30, 3000, .3);
		addPeak(peaks2, 4, 40, 4000, .4);

		SpectralPeaks result = peaks1 + peaks2;
		CPPUNIT_ASSERT_EQUAL(
			std::string(
			"Indices: [ 2 4 7 9 ]\n"
			"Frequencies: [ 10 20 30 40 ]\n"
			"Magnitudes: [ 1000 2000 3000 4000 ]\n"
			"Phases: [ 0.1 0.2 0.3 0.4 ]\n"
			), asString(result));
	}
	void test_peakAddition_whenInterleaved()
	{
		SpectralPeaks peaks1 = createPeaks();
		addPeak(peaks1, 1, 10, 1000, .1);
		addPeak(peaks1, 3, 30, 3000, .3);

		SpectralPeaks peaks2 = createPeaks();
		addPeak(peaks2, 2, 20, 2000, .2);
		addPeak(peaks2, 4, 40, 4000, .4);

		SpectralPeaks result = peaks1 + peaks2;
		CPPUNIT_ASSERT_EQUAL(
			std::string(
			"Indices: [ 2 5 6 9 ]\n"
			"Frequencies: [ 10 20 30 40 ]\n"
			"Magnitudes: [ 1000 2000 3000 4000 ]\n"
			"Phases: [ 0.1 0.2 0.3 0.4 ]\n"
			), asString(result));
	}
	void test_peakAddition_whenCoincident()
	{
		SpectralPeaks peaks1 = createPeaks();
		addPeak(peaks1, 1, 10, 1000, .1);
		addPeak(peaks1, 2, 20, 2000, .2);

		SpectralPeaks peaks2 = createPeaks();
		addPeak(peaks2, 1, 10, 1001, .11);
		addPeak(peaks2, 3, 30, 3000, .3);

		SpectralPeaks result = peaks1 + peaks2;
		CPPUNIT_ASSERT_EQUAL(
			std::string(
			"Indices: [ 2 3 4 7 ]\n"
			"Frequencies: [ 10 10 20 30 ]\n"
			"Magnitudes: [ 1000 1001 2000 3000 ]\n"
			"Phases: [ 0.1 0.11 0.2 0.3 ]\n"
			), asString(result));
	}
};


