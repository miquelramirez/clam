/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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


#ifndef _AutocorrelationEvaluator_
#define _AutocorrelationEvaluator_

#include "Processing.hxx"
#include "Audio.hxx"
#include "Correlation.hxx"

namespace CLAM
{

	class AutocorrelationEvaluatorConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (AutocorrelationEvaluatorConfig, 2, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, int, CorrelationSize);
	protected:
		/** Dynamic type initialization: All attributes are
		 *   instantiated.
		 */
		void DefaultInit(void);
	};

	class AutocorrelationEvaluator: public Processing
	{
	protected:
		/** Autocorrelation Configuration */
		AutocorrelationEvaluatorConfig mConfig;
		/** Autocorrelation size */
		int mSize;

		const char *GetClassName() const {return "Autocorrelation Evaluator";}

		int GetSize() {return mSize;}

		inline void CheckTypes(const Audio& in, const Correlation &out) const;

		bool ConcreteConfigure(const ProcessingConfig&);

		bool ConcreteStart();

		bool ConcreteStop();

	public:

		AutocorrelationEvaluator();

		AutocorrelationEvaluator( const AutocorrelationEvaluatorConfig& cfg );

		~AutocorrelationEvaluator();

		/** Configuration access:
		 */
		const ProcessingConfig &GetConfig() const { return mConfig;}

		/** Supervised-mode Do function.
		 */
		bool Do(void) { return true; }

		/** Standard Autocorrelation Do function
		*/
		bool Do( const Audio& in, Audio& out);//Correlation &out);

	};
}

#endif
