#ifndef _WINDOW_GENERATOR_
#define _WINDOW_GENERATOR_


/**
* This class can be used for generating Windows and for zeropadding the 
* windowed data. The Do functions accept different types of data,  which 
* are simple Arrays, Audio and Spectrum.
* <p>
* If the window is smaller as the Data Array then the rest of the Array 
* is filled with Zeroes.
* <p>
* The Size of the window can be changed via a control, the Type 
* (Hamming,Kaiser, etc) is fixed.
* A configuration option is to generate the window everytime  the  
* Do() function is called. This is settable via the UseTable element
* of the configuration.
*
* Note: The last and the first element of the window always have the
* same value, therefore, if you want to use it for windowing an FFT,
* you have to create a window of size FFTSize+1. (if you care for correctness)
*/


namespace MTG {

  class EWindowNormalize : public Enum {
  public:
		
		static tEnumValue sEnumValues[];
		static tValue sDefault;
		EWindowNormalize() : Enum(sEnumValues, sDefault) {}
		EWindowNormalize(tValue v) : Enum(sEnumValues, v) {};
		EWindowNormalize(std::string s) : Enum(sEnumValues, s) {};
		
		typedef enum {
      eNone,
	    eAnalysis,
	    eEnergy
	  };
		
		virtual Component* Species() const
		{
			return (Component*) new EWindowNormalize(eAnalysis);
		};
	};




  class EWindowType : public Enum
  {
  public:
	
	static tEnumValue sEnumValues[];
	static tValue sDefault;
	EWindowType() : Enum(sEnumValues, sDefault) {}
	EWindowType(tValue v) : Enum(sEnumValues, v) {};
	EWindowType(std::string s) : Enum(sEnumValues, s) {};

	typedef enum {
	  eKaiserBessel17,
	  eKaiserBessel18,
	  eKaiserBessel19,
	  eKaiserBessel20,
	  eKaiserBessel25,
	  eKaiserBessel30,
	  eKaiserBessel35,
	  eBlackmanHarris74,
	  eBlackmanHarris62,
	  eBlackmanHarris70,
	  eBlackmanHarris92,
	  eHamming,
	  eTriangular,
	  eBlackmanHarris92TransMainLobe
	};

	virtual Component* Species() const
	{
		// TODO: This is a xapusa. I want a default constructor!
		return (Component*) new EWindowType(eHamming);
	};
};


	class WindowGeneratorConfig: public ProcessingObjectConfig
	{
	public:
		REG_NUM_ATTR_USING_INTERFACE (WindowGeneratorConfig, 6,ProcessingObjectConfig);
		REGISTER (0, Name, std::string);
		REGISTER (1, Type, EWindowType);
		REGISTER (2, Size, TSize);
		REGISTER (3, MaxSize, TSize);
		REGISTER (4, UseTable, bool);
		REGISTER (5, Normalize, EWindowNormalize);

		void DefaultInit();

	};


	class WindowGenerator: public ProcessingObject {
		WindowGeneratorConfig mConfig;

		const char *GetClassName() {return "WindowGenerator";}

		/** Config change method
		 * @throw
		 * bad_cast exception when the argument is not an SpecAdderConfig
		 * object.
		 */
		bool ConcreteConfigure(const ProcessingObjectConfig&) throw(std::bad_cast);

	public:
	  
		WindowGenerator();

		WindowGenerator(const WindowGeneratorConfig &c);

	        ~WindowGenerator();

		const ProcessingObjectConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(DataArray& output);

		bool Do(Spectrum& output);
		bool Do(Audio& output);

		// Port interfaces.

		bool SetPrototypes(const DataArray& out);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

		void StoreOn(Storage &s) {};

	  void SetSize(TSize size) { mSize.DoControl(size); }

	private:
		InControl    mSize;
	  DataArray  mTable;

		void CreateTable(DataArray& table,EWindowType windowType,long windowsize) const;
	        void CreateWindowFromTable(DataArray &window) const;
		double BesselFunction(double x) const;
		void KaiserBessel(long size,DataArray& window,
											double alpha) const;
		void BlackmanHarrisX(long size,DataArray& window,
												 double a0,double a1,double a2,double a3 = 0.) const;
		void BlackmanHarris62(long size,DataArray& window) const;
		void BlackmanHarris70(long size,DataArray& window) const;
		void BlackmanHarris74(long size,DataArray& window) const;
		void BlackmanHarris92(long size,DataArray& window) const;
		void Hamming(long size,DataArray& window) const;
		void Triangular(long size,DataArray& window) const;
		void BlackmanHarris92TransMainLobe(long size,DataArray& window) const;
		void InvertWindow(const DataArray& originalWindow,
											DataArray& invertedWindow) const;
		void InvertWindow(DataArray& window) const;
		void NormalizeWindow(DataArray& window) const;
		double Sinc(double x, short N) const;

	};

};//namespace MTG

#endif // _SPECTRUM_ADDER_
