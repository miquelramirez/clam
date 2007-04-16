/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef _SegmentatorConfig_
#define _SegmentatorConfig_

#include "SearchArray.hxx"
#include "ProcessingConfig.hxx"
#include <iosfwd>

namespace CLAM {

typedef struct //may need further additions
{
	TIndex id;
	TData threshold;
	TData percentil;
	TData nPreviousFrames;
} TDescriptorsParams;

inline bool operator<(const TDescriptorsParams& param1,const TDescriptorsParams& param2)
{
	if (param1.id<param2.id) return true;
	else return false;
}

inline bool operator>=(const TDescriptorsParams& param1,const TDescriptorsParams& param2)
{
	if (param1.id>=param2.id) return true;
	else return false;
}

inline bool operator==(const TDescriptorsParams& param1,const TDescriptorsParams& param2)
{
	if (param1.id==param2.id) return true;
	else return false;
}

inline bool operator==(const TDescriptorsParams& param1,TIndex id)
{
	if (param1.id==id) return true;
	else return false;
}

std::ostream& operator << (std::ostream& myStream, const TDescriptorsParams& a);

std::istream& operator >> (std::istream& myStream, const TDescriptorsParams& a);

	/**
	 * Config class for the Segmentator Processing
	 */
class SegmentatorConfig : public ProcessingConfig
{
friend class Segmentator;
public:
	DYNAMIC_TYPE_USING_INTERFACE(SegmentatorConfig,2,ProcessingConfig);
	DYN_ATTRIBUTE (0, public, int, MinSegmentLength);
	DYN_ATTRIBUTE (1, public, Array<TDescriptorsParams>, DescriptorsParams);
protected:
	void DefaultInit();
/*Public Interface*/
public:
	void AddDescParams(const TDescriptorsParams& descParams);
	bool FindDescParams(TDescriptorsParams& descParams);
	void ClearDescParams();

	void SetDescriptorsSearch (const SearchArray<TDescriptorsParams> & frame) {
		mDescriptorsSearch=frame;
	}
	const SearchArray<TDescriptorsParams> & GetDescriptorsSearch () const {
		return mDescriptorsSearch;
	}
	SearchArray<TDescriptorsParams> & GetDescriptorsSearch () {
		return mDescriptorsSearch;
	}
	~SegmentatorConfig(){};
private:
	SearchArray<TDescriptorsParams> mDescriptorsSearch;
};

} // namespace CLAM

#endif

