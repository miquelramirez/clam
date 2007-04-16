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

#include "MyProcessingWithSimpleControls.hxx"

namespace CLAM {

void MyProcessingWithSimpleControls::InitChildrenPOs()
{
	for(int i=0; i!=NControlsArray; i++) {
		subPOsTransformation.push_back(
			new TransformationDummy () );
	}
}

void MyProcessingWithSimpleControls::InitAInControlArray()
{
	// init in controls array:
	aInControlArray.push_back( new VFxInControl (
			"A_In_Control_0",
			this,
			&MyProcessingWithSimpleControls::ACallback< eVoice0 > ) );

	aInControlArray.push_back( new VFxInControl (
			"A_In_Control_1",
			this,
			&MyProcessingWithSimpleControls::ACallback< eVoice1 > ) );

	aInControlArray.push_back( new VFxInControl (
			"A_In_Control_2",
			this,
			&MyProcessingWithSimpleControls::ACallback< eVoice2 > ) );

	aInControlArray.push_back( new VFxInControl (
			"A_In_Control_3",
			this,
			&MyProcessingWithSimpleControls::ACallback< eVoice3 > ) );

	aInControlArray.push_back( new VFxInControl (
			"A_In_Control_4",
			this,
			&MyProcessingWithSimpleControls::ACallback< eVoice4 > ) );

}





MyProcessingWithSimpleControls::MyProcessingWithSimpleControls() :
	// init single in controls
	cSingleControl ("CSingleControl", this, &MyProcessingWithSimpleControls::SimpleCallback),
	dSingleControl ("CSingleControl", this,	&MyProcessingWithSimpleControls::SimpleCallback)
{
	
	// init sub POs transformation
	InitChildrenPOs();
	// init in controls array:
	InitAInControlArray();
}

void MyProcessingWithSimpleControls::DeleteSubPOs()
{
	Transformations::iterator it;
	for(it=subPOsTransformation.begin(); it!=subPOsTransformation.end(); it++)
		delete (*it); // (it) points to a PO pointer
}

void MyProcessingWithSimpleControls::DeleteControls( VFxInControls& c )
{
	VFxInControls::iterator it;
	for(it=c.begin(); it!=c.end(); it++)
		delete (*it); // (it) points to a control pointer
}

MyProcessingWithSimpleControls::~MyProcessingWithSimpleControls()
{
	DeleteControls(aInControlArray);
	DeleteControls(bInControlArray);
	DeleteSubPOs();

}

}; //namespace

