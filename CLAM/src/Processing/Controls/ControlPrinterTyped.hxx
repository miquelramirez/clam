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

#ifndef _ControlPrinterTyped_
#define _ControlPrinterTyped_


#include <CLAM/Enum.hxx>

#include "DataTypes.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "ControlArray.hxx"
#include "InControl.hxx"

namespace CLAM
{

class EnumTypedTypes: public CLAM::Enum {
public:
        EnumTypedTypes() : CLAM::Enum(ValueTable(), eFloat) {}
        EnumTypedTypes(tValue v) : CLAM::Enum(ValueTable(), v) {};
        EnumTypedTypes(const std::string & s) : CLAM::Enum(ValueTable(), s) {};

        virtual CLAM::Component * Species() const {return new EnumTypedTypes();}

        typedef enum {
                eString=0,
                eFloat=1,
                eDouble=2,
                eInt=3,
                eMidi=4,
        } tEnum;
        static tEnumValue * ValueTable()
        {
                static tEnumValue sValueTable[] =
                {
                        {eString,"s"},
                        {eFloat,"f"},
                        {eDouble,"d"},
                        {eInt,"i"},
                        {eMidi,"M"},
                        {0,NULL}
                };
                return sValueTable;
        }
};


class ControlPrinterTypedConfig : public ProcessingConfig
{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (ControlPrinterTypedConfig, 3, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Identifier);
		DYN_ATTRIBUTE (1, public, std::string, TypesMask);
		DYN_ATTRIBUTE (2, public, bool, GuiOnly );


	private:
		void DefaultInit();
};

class ControlPrinterTyped : public Processing
{
	typedef std::vector<InControlBase*> InControls;
	ControlPrinterTypedConfig _config;
	InControls mInControls;

	public:
		const char *GetClassName() const { return "ControlPrinterTyped"; }
		ControlPrinterTyped();
		ControlPrinterTyped( const ControlPrinterTypedConfig& cfg ); 
		~ControlPrinterTyped();
		bool ConcreteConfigure( const ProcessingConfig& cfg ); 
		const ProcessingConfig& GetConfig() const { return _config; }
		bool Do();
	protected:
		void RemoveOldControls();
	private:
		void ResizeControls(unsigned nInputs, const std::string & baseName);
		void ResizeControls(unsigned nInputs, const std::list<std::string> & names);
		
		const unsigned int GetInputsNumber() const;

		InControlBase * createControl(const std::string & type, const std::string & name);
		void ClearControls();
};

}

#endif

