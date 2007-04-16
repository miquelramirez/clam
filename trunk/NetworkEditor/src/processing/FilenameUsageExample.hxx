
#ifndef _FilenameUsageExample_hxx_
#define _FilenameUsageExample_hxx_

#include <CLAM/DataTypes.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/Filename.hxx> 
#include <string>

namespace CLAM
{

class InImageFilename : public InFilename
{
public:
	InImageFilename(const std::string & s="") : InFilename(s) {}
	InImageFilename(const char * s) : InFilename(s) {}
	virtual const char * TypeFamily() const { return "Image"; }
	virtual const Filter * Filters() const
	{
		static const Filter filters[] = {
			{"Portable Network Graphics", "*.png"},
			{"JPEG Image", "*.jpg"},
			{"Scalable Vector Graphics", "*.svg"},
			{0,0}
		};
		return filters;
	}
};

class OutImageFilename : public OutFilename
{
public:
	OutImageFilename(const std::string & s="") : OutFilename(s) {}
	OutImageFilename(const char * s) : OutFilename(s) {}
	virtual const char * TypeFamily() const { return "Image"; }
	virtual const Filter * Filters() const
	{
		static const Filter filters[] = {
			{"Portable Network Graphics", "*.png"},
			{"JPEG Image", "*.jpg"},
			{0,0}
		};
		return filters;
	}
};
CLAM_TYPEINFOGROUP(BasicCTypeInfo, OutImageFilename);
CLAM_TYPEINFOGROUP(BasicCTypeInfo, InImageFilename);


class FilenameUsageExampleConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (FilenameUsageExampleConfig, 2, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, CLAM::InImageFilename, ImageInput);
	DYN_ATTRIBUTE (1, public, CLAM::OutImageFilename, ImageOutput);

protected:
	void DefaultInit(void);
};

class FilenameUsageExample : public Processing
{
private:
	
	FilenameUsageExampleConfig mConfig;

public:
	FilenameUsageExample();
	FilenameUsageExample( const FilenameUsageExampleConfig & );

	bool Do();
	virtual ~FilenameUsageExample(){}
	const char * GetClassName() const {return "FilenameUsageExample";}
	
	inline const ProcessingConfig &GetConfig() const { return mConfig;}
	bool ConcreteConfigure(const ProcessingConfig& c);

};

}

#endif 

